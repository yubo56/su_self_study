using Toybox.Background;
using Toybox.System;
using Toybox.Communications;
using Toybox.Math;
using Toybox.Position;
using Toybox.Time;

const appid = "";
const lat = 42.4522; // default location, cornell
const lon = -76.4804;
const url = "https://api.openweathermap.org/data/3.0/onecall";
const headers = {
    :method => Communications.HTTP_REQUEST_METHOD_GET,
    :responseType => Communications.HTTP_RESPONSE_CONTENT_TYPE_JSON
};

const key = "";
const ccurl = "https://api.tomorrow.io/v4/timelines";

(:background)
class YuboWatchSDelegate extends System.ServiceDelegate {
    var bglat = lat;
    var bglon = lon;
    var ret = new[30];
    function initialize() {
        System.ServiceDelegate.initialize();
    }
	function timeToHHMM(ts) {
	    var tinfo = Time.Gregorian.info(new Time.Moment(ts), Time.FORMAT_SHORT);
	    return Lang.format(
	        "$1$$2$",
	        [tinfo.hour.format("%02d"), tinfo.min.format("%02d")]
	    );
	}
    function weatherStr(w) {
        var id = w.get("id");
        return Lang.format("$1$$2$", [
	        id == 800 ? "O" : w.get("main").substring(0, 1),
	        (id % 100).format("%02d")]);
    }
    function onTemporalEvent() {
        if (!System.getDeviceSettings().phoneConnected) {
            Background.exit([[], false, -11]);
        }

        var positionInfo = Position.getInfo();
        if (positionInfo has :position && positionInfo.position != null) {
            bglat = positionInfo.position.toDegrees()[0];
            bglon = positionInfo.position.toDegrees()[1];
        }
        Communications.makeWebRequest(
            url,
            {
                "lat" => bglat,
                "lon" => bglon,
                "appid" => appid,
                "units" => "metric",
                "exclude" => "daily,hourly,minutely,alerts"
            },
            headers,
            new Lang.Method(self, :currentCb)
        );
    }

    function currentCb(responseCode, data) {
        if (responseCode != 200) {
            Background.exit([[], false, responseCode]);
        }
        var current = data.get("current");

        // parse weather into "TDRSAOC" (thunderstorm, drizzle, rain, snow, "atmosphere", clear, cloudy) + code
        var weather = current.get("weather")[0];
        var wsymb = weather.get("id") == 800 ? "O" : weather.get("main").substring(0, 1);

        ret[0] = current.get("temp");
        ret[1] = current.get("wind_speed") * 3.6;
        ret[2] = current.get("humidity");
        ret[3] = Math.round(current.get("dew_point"));
        ret[4] = timeToHHMM(current.get("sunrise"));
        ret[5] = timeToHHMM(current.get("sunset"));
        ret[6] = Math.round(current.get("uvi"));
        ret[7] = "O00";
        ret[8] = "O00";
        ret[9] = "O00";
        ret[10] = wsymb;
        ret[11] = (weather.get("id") % 100).format("%02d");

        Communications.makeWebRequest(
            ccurl, 
            {
               "location" => Lang.format("$1$,$2$", [bglat, bglon]),
               "units" => "metric",
               "apikey" => key,
               "fields" => "temperatureMax,temperatureMin,dewPointMax",
               "timesteps" => "1d",
               "endtime" => "nowPlus6d"
            }, 
            headers, 
            new Lang.Method(self, :dailyCb)
        );
    }
    function dailyCb(responseCode, data) {
        if (responseCode != 200) {
            Background.exit([[], false, responseCode]);
        }
        var intervals = data.get("data").get("timelines")[0].get("intervals");
        for (var i = 0; i < 6; i++) {
            ret[12 + i] = intervals[i].get("values").get("temperatureMin");
            ret[18 + i] = intervals[i].get("values").get("temperatureMax");
            ret[24 + i] = intervals[i].get("values").get("dewPointMax");
        }
        Background.exit([ret, true, 200]);
    }
}

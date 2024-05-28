using Toybox.Background;
using Toybox.System;
using Toybox.Communications;
using Toybox.Math;
using Toybox.Position;
using Toybox.Time;

const appid = "b11355762659b99427b7f2b394f8c3b5";
const lat = 42.4522; // default location, cornell
const lon = -76.4804;
const url = "https://api.openweathermap.org/data/3.0/onecall";
const headers = {
    :method => Communications.HTTP_REQUEST_METHOD_GET,
    :responseType => Communications.HTTP_RESPONSE_CONTENT_TYPE_JSON
};

(:background)
class YuboWatchSDelegate extends System.ServiceDelegate {
    var bglat = lat;
    var bglon = lon;
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
                "exclude" => "hourly,minutely,alerts"
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

        var daily = data.get("daily");

        var ret = new[30];
        for (var i = 0; i < 6; i++) {
            ret[12 + i] = daily[i].get("temp").get("min");
            ret[18 + i] = daily[i].get("temp").get("max");
            ret[24 + i] = daily[i].get("dew_point");
        }

        ret[0] = current.get("temp");
        ret[1] = current.get("wind_speed") * 3.6;
        ret[2] = current.get("humidity");
        ret[3] = Math.round(current.get("dew_point"));
        ret[4] = timeToHHMM(current.get("sunrise"));
        ret[5] = timeToHHMM(current.get("sunset"));
        ret[6] = Math.round(current.get("uvi"));
        ret[7] = weatherStr(daily[0].get("weather")[0]);
        ret[8] = weatherStr(daily[1].get("weather")[0]);
        ret[9] = weatherStr(daily[2].get("weather")[0]);
        ret[10] = wsymb;
        ret[11] = (weather.get("id") % 100).format("%02d");
        Background.exit([ret, true, 200]);
    }
}

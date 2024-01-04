//  0 temp
//  1 wspeed
//  2 humid
//  3 tdewp
//  4 sunrise
//  5 sunset
//  6 uvi
//  7 tlo
//  8 thi
//  9 ttlo
// 10 tthi
// 11 ttdewp
// 12 wtoday
// 13 wtomm
// 14 wovmm
// 15 wsymb
// 16 wcode
// 17 dhis
// 18 dlos
// 19 ddews

using Toybox.Background;
using Toybox.System;
using Toybox.Communications;
using Toybox.Math;
using Toybox.Position;
using Toybox.Time;

const appid = "";
const lat = 42.4522; // default location, cornell
const lon = -76.4804;
const url = "https://api.openweathermap.org/data/2.5/onecall";
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

        var his = new [7];
        var lows = new [7];
        var dews = new [7];
        for (var i = 0; i < 7; i++) {
            lows[i] = daily[i].get("temp").get("min");
            his[i] = daily[i].get("temp").get("max");
            dews[i] = daily[i].get("dew_point");
        }
        Background.exit([[
            current.get("temp"),
            current.get("wind_speed") * 3.6,
            current.get("humidity"),
            Math.round(current.get("dew_point")),
            timeToHHMM(current.get("sunrise")),
            timeToHHMM(current.get("sunset")),
            Math.round(current.get("uvi")),
            Math.round(lows[0]),
            Math.round(his[0]),
            Math.round(lows[1]),
            Math.round(his[1]),
            Math.round(dews[1]),
            weatherStr(daily[0].get("weather")[0]),
            weatherStr(daily[1].get("weather")[0]),
            weatherStr(daily[2].get("weather")[0]),
            wsymb,
            (weather.get("id") % 100).format("%02d"),
            his,
            lows,
            dews,
        ], true, 200]);
    }
}

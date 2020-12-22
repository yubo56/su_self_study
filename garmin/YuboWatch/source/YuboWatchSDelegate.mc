using Toybox.Background;
using Toybox.System;
using Toybox.Communications;
using Toybox.Math;
using Toybox.Position;
using Toybox.Sensor;

var lat = 42.4522; // default location, cornell
var lon = -76.4804;
var appid = "";

(:background)
class YuboWatchSDelegate extends System.ServiceDelegate {
    var precips = [];
    var dat = [];
    var bglat = lat;
    var bglon = lon;

    function initialize() {
        System.ServiceDelegate.initialize();
    }

	function timeToHHMM(ts) {
	    var m = new Time.Moment(ts);
	    var tinfo = Time.Gregorian.info(m, Time.FORMAT_SHORT);
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
        var positionInfo = Position.getInfo();
        if (positionInfo has :position && positionInfo.position != null) {
            bglat = positionInfo.position.toDegrees()[0];
            bglon = positionInfo.position.toDegrees()[1];
        }
        if (!System.getDeviceSettings().phoneConnected) {
            Background.exit([dat, false, precips]);
        }
        else {
            Communications.makeWebRequest(
	            "https://api.openweathermap.org/data/2.5/onecall",
	            {
	                "lat" => bglat,
	                "lon" => bglon,
	                "appid" => appid,
	                "units" => "metric",
	                "exclude" => "daily,current,hourly,alerts"
	            },
	            {
	                :method => Communications.HTTP_REQUEST_METHOD_GET,
	                :responseType => Communications.HTTP_RESPONSE_CONTENT_TYPE_JSON
	            },
	            method(:minutelyCb)
	        );
	    }
    }
    function minutelyCb(responseCode, data) {
        if (responseCode != 200) {
            Background.exit([dat, false, precips]);
            return;
        }
        if (data.hasKey("minutely")) {
            precips = new [61];
            var minutely = data.get("minutely");
            for (var i = 0; i < 61; i++) {
                precips[i] = Math.ln(minutely[i].get("precipitation"));
            }
        }
        Communications.makeWebRequest(
            "https://api.openweathermap.org/data/2.5/onecall",
            {
                "lat" => bglat,
                "lon" => bglon,
                "appid" => appid,
                "units" => "metric",
                "exclude" => "hourly,minutely,alerts"
            },
            {
                :method => Communications.HTTP_REQUEST_METHOD_GET,
                :responseType => Communications.HTTP_RESPONSE_CONTENT_TYPE_JSON
            },
            method(:currentCb)
        );
    }

    function currentCb(responseCode, data) {
        if (responseCode != 200) {
            Background.exit([dat, false, precips]);
            return;
        }
        var current = data.get("current");

        // parse weather into "TDRSAOC" (thunderstorm, drizzle, rain, snow, "atmosphere", clear, cloudy) + code
        var weather = current.get("weather")[0];
        var wsymb = "O";
        if (weather.get("id") != 800) {
            wsymb = weather.get("main").substring(0, 1);
        }

        var daily = data.get("daily");

        var numDays = daily.size();
        var his = new [numDays];
        var lows = new [numDays];
        var dews = new [numDays];
        for (var i = 0; i < numDays; i++) {
            lows[i] = daily[i].get("temp").get("min");
            his[i] = daily[i].get("temp").get("max");
            dews[i] = daily[i].get("dew_point");
        }
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
        dat = [
            current.get("temp"),
            current.get("wind_speed") * 3.6,
            current.get("humidity"),
            Math.round(current.get("dew_point")),
            timeToHHMM(current.get("sunrise")),
            timeToHHMM(current.get("sunset")),
            Math.round(current.get("uvi")),
            Math.round(daily[0].get("temp").get("min")),
            Math.round(daily[0].get("temp").get("max")),
            Math.round(daily[1].get("temp").get("min")),
            Math.round(daily[1].get("temp").get("max")),
            Math.round(daily[1].get("dew_point")),
            weatherStr(daily[0].get("weather")[0]),
            weatherStr(daily[1].get("weather")[0]),
            weatherStr(daily[2].get("weather")[0]),
            wsymb,
            (weather.get("id") % 100).format("%02d"),
            his,
            lows,
            dews,
        ];
        Background.exit([dat, true, precips]);
    }
}

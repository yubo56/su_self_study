using Toybox.Background;
using Toybox.System;
using Toybox.Communications;
using Toybox.Math;
using Toybox.Timer;
using Toybox.Position;
using Toybox.Sensor;

var lat = 42.4522; // default location, cornell
var lon = -76.4804;
var appid = "";

(:background)
class YuboWatchSDelegate extends System.ServiceDelegate {

    var infoDict = {};
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
            Background.exit([infoDict, false]);
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
	                :responseType => Communications.HTTP_RESPONSE_CONTENT_TYPE_TEXT_PLAIN
	            },
	            method(:minutelyCb)
	        );
	    }
    }

    // 1hr precipitation amounts; parse by hand instead of JSON to save memory
    function minutelyCb(responseCode, data) {
        if (responseCode != 200) {
            Background.exit([infoDict, false]);
            return;
        }
        var precipStr = "precipitation\":";
        var str2 = "";
        var start_idx = data.find(precipStr);
        if (start_idx) {
            var precips = new [31];
            for (var i = 0; i < 61; i++) {
                if (start_idx == null) {
                    precips[i] = precipMin - 1;
                }
	            str2 = data.substring(start_idx + precipStr.length(), data.length());
	            var offset = str2.find("}");
	            if (i % 2 == 0) {
	               precips[Math.round(i / 2)] = Math.ln(str2.substring(0, offset).toFloat());
	            }
	            data = str2.substring(offset, str2.length());
	            start_idx = data.find(precipStr);
	        }
	        infoDict.put("precips", precips);
	    }
        System.println(Lang.format("pre-other, $1$", [System.getSystemStats().freeMemory]));
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
        System.println(Lang.format("Current, $1$, $2$", [responseCode, System.getSystemStats().freeMemory]));
        if (responseCode != 200) {
            Background.exit([infoDict, false]);
            return;
        }
        var current = data.get("current");
        infoDict.put("temp", current.get("temp"));
        infoDict.put("wspeed", current.get("wind_speed") * 3.6);
        infoDict.put("humid", current.get("humidity"));
        infoDict.put("tdewp", Math.round(current.get("dew_point")));
        infoDict.put("sunrise", timeToHHMM(current.get("sunrise")));
        infoDict.put("sunset", timeToHHMM(current.get("sunset")));
        infoDict.put("uvi", Math.round(current.get("uvi")));

        // parse weather into "TDRSAOC" (thunderstorm, drizzle, rain, snow, "atmosphere", clear, cloudy) + code
        var weather = current.get("weather")[0];
        if (weather.get("id") == 800) {
            infoDict.put("wsymb", "O");
        } else {
            infoDict.put("wsymb", weather.get("main").substring(0, 1));
        }
        infoDict.put("wcode", (weather.get("id") % 100).format("%02d"));

        var daily = data.get("daily");
        infoDict.put("tlo", Math.round(daily[0].get("temp").get("min")));
        infoDict.put("thi", Math.round(daily[0].get("temp").get("max")));
        infoDict.put("ttlo", Math.round(daily[1].get("temp").get("min")));
        infoDict.put("tthi", Math.round(daily[1].get("temp").get("max")));
        infoDict.put("ttdewp", Math.round(daily[1].get("dew_point")));

        // parse weather into "TDRSAOC" (thunderstorm, drizzle, rain, snow, "atmosphere", clear, cloudy) + code
        var wtoday = daily[0].get("weather")[0];
        infoDict.put("wtoday", weatherStr(wtoday));
        var wtomm = daily[1].get("weather")[0];
        infoDict.put("wtomm", weatherStr(wtomm));
        var wovmm = daily[2].get("weather")[0];
        infoDict.put("wovmm", weatherStr(wovmm));

        var numDays = daily.size();
        var his = new [numDays];
        var lows = new [numDays];
        var dews = new [numDays];
        for (var i = 0; i < numDays; i++) {
            lows[i] = daily[i].get("temp").get("min");
            his[i] = daily[i].get("temp").get("max");
            dews[i] = daily[i].get("dew_point");
        }
        infoDict.put("dhis", his);
        infoDict.put("dlos", lows);
        infoDict.put("ddews", dews);
        Background.exit([infoDict, true]);
    }
}

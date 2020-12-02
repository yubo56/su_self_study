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

    var infoDict;

    function initialize() {
        infoDict = Application.getApp().getProperty(BGDATA); // don't override old state if watch restart (edge case)
        if (infoDict == null) {
            infoDict = {
                lat => lat,
                lon => lon,
            };
        }
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

    function onTemporalEvent() {
        var positionInfo = Position.getInfo();
        if (positionInfo has :position && positionInfo.position != null) {
            infoDict.put("lat", positionInfo.position.toDegrees()[0]);
            infoDict.put("lon", positionInfo.position.toDegrees()[1]);
        }
        if (!System.getDeviceSettings().phoneConnected) {
            Background.exit([infoDict, false]);
        }
        else {
	        Communications.makeWebRequest(
	            "https://api.openweathermap.org/data/2.5/onecall",
	            {
	                "lat" => infoDict.get("lat").format("%.6f"),
	                "lon" => infoDict.get("lon").format("%.6f"),
	                "appid" => appid,
	                "units" => "metric",
	                "exclude" => "hourly,minutely,alerts"
	            },
	            {
	                :method => Communications.HTTP_REQUEST_METHOD_GET,
	                :responseType => Communications.HTTP_RESPONSE_CONTENT_TYPE_JSON
	            },
	            method(:dailyCurrentCb)
	        );
        }
    }

    function dailyCurrentCb(responseCode, data) {
        if (responseCode != 200) {
            var myStats = System.getSystemStats();
            var now = Time.Gregorian.info(Time.now(), Time.FORMAT_MEDIUM);
            var clockTime = System.getClockTime();
            System.println(Lang.format(
                "DailyCurrentCode ($1$). Free/Used/Tot (B): ($2$/$3$/$4$). [$5$ $6$, $7$:$8$:$9$]",
                [responseCode, myStats.freeMemory, myStats.usedMemory, myStats.totalMemory,
                    now.month,now.day.format("%02d"), clockTime.hour.format("%02d"), clockTime.min.format("%02d"), clockTime.sec.format("%02d")]
            ));
            Background.exit([infoDict, false]);
            return;
        }
        var current = data.get("current");
        var daily = data.get("daily");
        infoDict.put("temp", current.get("temp"));
        infoDict.put("wspeed", current.get("wind_speed") * 3.6);
        infoDict.put("humid", current.get("humidity"));
        infoDict.put("tlo", Math.round(daily[0].get("temp").get("min")));
        infoDict.put("thi", Math.round(daily[0].get("temp").get("max")));
        infoDict.put("tdewp", Math.round(current.get("dew_point")));
        infoDict.put("ttlo", Math.round(daily[1].get("temp").get("min")));
        infoDict.put("tthi", Math.round(daily[1].get("temp").get("max")));
        infoDict.put("ttdewp", Math.round(daily[1].get("dew_point")));
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
        var wtoday = daily[0].get("weather")[0];
        var wtodayId = wtoday.get("id");
        infoDict.put("wtoday", Lang.format("$1$$2$", [
            wtodayId == 800 ? "O" : wtoday.get("main").substring(0, 1),
            (wtodayId % 100).format("%02d")]));
        var wtomm = daily[1].get("weather")[0];
        var wtommId = wtomm.get("id");
        infoDict.put("wtomm", Lang.format("$1$$2$", [
            wtommId == 800 ? "O" : wtomm.get("main").substring(0, 1),
            (wtommId % 100).format("%02d")]));
        var wovmm = daily[2].get("weather")[0];
        var wovmmId = wovmm.get("id");
        infoDict.put("wovmm", Lang.format("$1$$2$", [
            wovmmId == 800 ? "O" : wovmm.get("main").substring(0, 1),
            (wovmmId % 100).format("%02d")]));
        
        infoDict.remove("dhis");
        infoDict.remove("dlos");
        infoDict.remove("ddews");
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

        Communications.makeWebRequest(
            "https://api.openweathermap.org/data/2.5/onecall",
            {
                "lat" => infoDict.get("lat").format("%.6f"),
                "lon" => infoDict.get("lon").format("%.6f"),
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

    // 1hr precipitation amounts
    function minutelyCb(responseCode, data) {
        if (responseCode != 200) {
            var myStats = System.getSystemStats();
            var now = Time.Gregorian.info(Time.now(), Time.FORMAT_MEDIUM);
            var clockTime = System.getClockTime();
            System.println(Lang.format(
                "MinutelyCode ($1$). Free/Used/Tot (B): ($2$/$3$/$4$). [$5$ $6$, $7$:$8$:$9$]",
                [responseCode, myStats.freeMemory, myStats.usedMemory, myStats.totalMemory,
                    now.month,now.day.format("%02d"), clockTime.hour.format("%02d"), clockTime.min.format("%02d"), clockTime.sec.format("%02d")]
            ));
            Background.exit([infoDict, false]);
            return;
        }
        if (data.hasKey("minutely")) {
            infoDict.remove("precips");
            var minutely = data.get("minutely");
            var numMinutes = minutely.size();
            var precips = new [numMinutes];
            for (var i = 0; i < numMinutes; i++) {
                if (minutely == null) {
                    precips[i] = precipMin - 1;
                    continue;
                }
                precips[i] = Math.ln(minutely[i].get("precipitation"));
            }
            infoDict.put("precips", precips);
        }

        Background.exit([infoDict, true]);
    }
}

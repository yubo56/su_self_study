using Toybox.Background;
using Toybox.System;
using Toybox.Communications;
using Toybox.Math;
using Toybox.Timer;
using Toybox.Position;
using Toybox.Sensor;

var lat = 42.377410; // default location, Somerville
var lon = -71.103310;
var appid = ""; // TODO add

(:background)
class YuboWatchSDelegate extends System.ServiceDelegate {

    var infoDict;

    function initialize() {
        infoDict = Application.getApp().getProperty(BGDATA); // don't override old state if watch restart (edge case)
        if (infoDict == null) {
            infoDict = {
                "lat" => lat,
                "lon" => lon,
                "alt" => 0
            };
        }
        System.ServiceDelegate.initialize();
    }

    function onTemporalEvent() {
        var positionInfo = Position.getInfo();
        if (positionInfo has :position && positionInfo.position != null) {
            infoDict.put("lat", positionInfo.position.toDegrees()[0]);
            infoDict.put("lon", positionInfo.position.toDegrees()[1]);
        }
        if (positionInfo has :altitude && positionInfo.altitude != null) {
            infoDict.put("alt", positionInfo.altitude);
        }
        Communications.makeWebRequest(
            "https://api.openweathermap.org/data/2.5/onecall",
            {
                "lat" => infoDict.get("lat").format("%.6f"),
                "lon" => infoDict.get("lon").format("%.6f"),
                "appid" => appid,
                "units" => "metric",
                "exclude" => "hourly,minutely"
            },
            {
                :method => Communications.HTTP_REQUEST_METHOD_GET,
                :responseType => Communications.HTTP_RESPONSE_CONTENT_TYPE_JSON
            },
            method(:dailyCurrentCb)
        );
    }

    function timeToHHMM(ts) {
        var m = new Time.Moment(ts);
        var tinfo = Time.Gregorian.info(m, Time.FORMAT_SHORT);
        return Lang.format(
            "$1$$2$",
            [tinfo.hour.format("%02d"), tinfo.min.format("%02d")]
        );
    }

    function dailyCurrentCb(responseCode, data) {
        if (responseCode != 200) {
            System.println("DailyCurrentCode " + responseCode);
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
                "exclude" => "daily,current,hourly"
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
            System.println("Minutely code " + responseCode);
            Background.exit([infoDict, false]);
            return;
        }
        if (data.hasKey("minutely")) {
            var minutely = data.get("minutely");
            var numMinutes = minutely.size();
            var precips = new [numMinutes];
            for (var i = 0; i < numMinutes; i++) {
                if (minutely == null) {
                    precips[i] = precipMin - 1;
                    continue;
                }
                precips[i] = Math.ln(minutely[i].get("precipitation"));
//                precips[i] = Math.ln(0.2 * 20 * i / 60);
            }
            infoDict.put("precips", precips);
        }

        Background.exit([infoDict, true]);
    }
}

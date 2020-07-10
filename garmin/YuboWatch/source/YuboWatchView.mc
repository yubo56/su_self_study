// add weather status for today/tomm (right of graph), not just current
// consider log scaling precipitation graph? 0.1mm/h = very light drizzle
// Could plot daily forecast instead of hourly for now
using Toybox.WatchUi;
using Toybox.Graphics;
using Toybox.System;
using Toybox.Lang;
using Toybox.Application;
using Toybox.ActivityMonitor;
using Toybox.Time;
using Toybox.Background;
using Toybox.Math;

function setText(view, text, defaultText) {
    if (text == null) {
        view.setText(defaultText);
    } else {
        view.setText(text);
    }
}

function textFromBg(bgInfoDict, propName, fmt) {
    if (bgInfoDict == null || bgInfoDict.get(propName) == null) {
        return 0.format(fmt);
    } else {
        var val = bgInfoDict.get(propName);
        return val.format(fmt);
    }
}

function setAndColorIfNegative(view, bgInfoDict, propName, fmt) {
    var str = textFromBg(bgInfoDict, propName, fmt);
    if (str.find("-") != null) { // color instead of negative sign
        setText(view, str.substring(1, str.length()), "--");
        view.setColor(Graphics.COLOR_RED);
    } else {
        setText(view, str, "--");
    }
}

function bound(val, min, max) {
    if (val < min) { return min; }
    if (val > max) { return max; }
    return val;
}
var precipMax = Math.ln(50.0); // max 50mm/hr
var precipMin = Math.ln(0.1); // min 0.1mm/hr

class YuboWatchView extends WatchUi.WatchFace {

    function initialize() {
        WatchFace.initialize();
    }

    // Load your resources here
    function onLayout(dc) {
        setLayout(Rez.Layouts.WatchFace(dc));
    }

    // Called when this View is brought to the foreground. Restore
    // the state of this View and prepare it to be shown. This includes
    // loading resources into memory.
    function onShow() {
    }

    function setTimeView() {
        var timeView = View.findDrawableById("TimeLabel");
        var timeFormat = "$1$$2$";
        var clockTime = System.getClockTime();
        var timeString = Lang.format(
            timeFormat,
            [clockTime.hour.format("%02d"), clockTime.min.format("%02d")]
        );
        timeView.setText(timeString);

        var now = Time.Gregorian.info(Time.now(), Time.FORMAT_MEDIUM);
        var dateView = View.findDrawableById("DateLabel");
        var dateFormat = "$1$, $2$ $3$";
        var dateString = Lang.format(
            dateFormat,
            [now.day_of_week, now.month, now.day.format("%02d")]
        );
        dateView.setText(dateString);
    }

    function setBattView() {
        var myStats = System.getSystemStats();
        var battPerc = myStats.battery;
        var battStr = battPerc.format("%.1f");
        var battView = View.findDrawableById("BattLabel");
        if (battPerc < 20) {
            battView.setColor(Graphics.COLOR_RED);
        } else {
            battView.setColor(Graphics.COLOR_GREEN);
        }
        battView.setText(battStr);
    }

    function setStepView() {
        var stepView = View.findDrawableById("StepLabel");
        var calView = View.findDrawableById("CalLabel");

        var info = ActivityMonitor.getInfo();
        var steps = info.steps;
        var stepGoal = info.stepGoal;
        var calories = info.calories;
        if (steps == null) {
            stepView.setText("-----");
            return;
        } else {
            if (steps < stepGoal) {
                stepView.setColor(Graphics.COLOR_WHITE);
            } else {
                stepView.setColor(Graphics.COLOR_GREEN);
            }
            stepView.setText(steps.format("%05d"));
        }

        if (calories == null) {
            calView.setText("----");
        } else {
            calView.setText(calories.format("%04d"));
        }
    }

    function setHRView() {
        var hrView = View.findDrawableById("HRLabel");
        var lastHRSample = ActivityMonitor.getHeartRateHistory(1, true).next();
        if (lastHRSample == null) {
            hrView.setText("N/A");
        } else {
            var hrate = lastHRSample.heartRate;
            hrView.setText(hrate.format("%03d"));
        }
    }

    function setWeatherView() {
        var bgInfoDict = Application.getApp().getProperty(BGDATA);
        if (bgInfoDict == null) {
           bgInfoDict = {};
        }

        setAndColorIfNegative(View.findDrawableById("TempLabel"), bgInfoDict, "temp", "%04.1f"); // 4 total digits
        setAndColorIfNegative(View.findDrawableById("TempLoLabel"), bgInfoDict, "tlo", "%02d");
        setAndColorIfNegative(View.findDrawableById("TempDewLabel"), bgInfoDict, "tdewp", "%02d");
        setAndColorIfNegative(View.findDrawableById("TempHiLabel"), bgInfoDict, "thi", "%02d");
        setAndColorIfNegative(View.findDrawableById("TempTommLoLabel"), bgInfoDict, "ttlo", "%02d");
        setAndColorIfNegative(View.findDrawableById("TempTommDewLabel"), bgInfoDict, "ttdewp", "%02d");
        setAndColorIfNegative(View.findDrawableById("TempTommHiLabel"), bgInfoDict, "tthi", "%02d");
        setText(View.findDrawableById("WindLabel"), textFromBg(bgInfoDict, "wspeed", "%02d"), "--");
        setText(View.findDrawableById("HumidLabel"), textFromBg(bgInfoDict, "humid", "%02d") + "%", "--%");
        setText(View.findDrawableById("UVLabel"), textFromBg(bgInfoDict, "uvi", "%02d"), "--");
        setText(View.findDrawableById("SunriseLabel"), bgInfoDict.get("sunrise"), "----");
        setText(View.findDrawableById("SunsetLabel"), bgInfoDict.get("sunset"), "----");
        setText(View.findDrawableById("WeatherSymbol"), bgInfoDict.get("wsymb"), "-");
        setText(View.findDrawableById("WeatherCode"), bgInfoDict.get("wcode"), "--");
        
        setText(View.findDrawableById("TodayWeather"), bgInfoDict.get("wtoday"), "C00");
        setText(View.findDrawableById("TommWeather"), bgInfoDict.get("wtomm"), "C00");
        
        var bglat = bgInfoDict.get("lat");
        if (bglat == null) {
            bglat = lat;
        }
        var bglon = bgInfoDict.get("lon");
        if (bglon == null) {
            bglon = lon;
        }
        var alt = bgInfoDict.get("alt");
        if (alt == null) {
            alt = 0;
        }
        var latLonStr = Lang.format("$1$/$2$/$3$", [bglat.format("%.2f"), bglon.format("%.2f"), alt.format("%04d")]);
        setText(View.findDrawableById("LatLabel"), latLonStr, "-----/-----/----");
    }

    function setBluetooth() {
        var btView = View.findDrawableById("BTLabel");
        if (System.getDeviceSettings().phoneConnected) {
            btView.setText("(B)");
            btView.setColor(Graphics.COLOR_BLUE);
        } else {
            btView.setText("(X)");
            btView.setColor(Graphics.COLOR_DK_GRAY);
        }
    }
    
    function setSuccessLabel() {
        var successView = View.findDrawableById("SuccessLabel");
        var numFailures = Application.getApp().getProperty(NUMFAILED);
        if (numFailures == null) {
            successView.setText("O");
            successView.setColor(Graphics.COLOR_DK_GRAY);
        } else if (numFailures == 0) {
            successView.setText("Y");
            successView.setColor(Graphics.COLOR_GREEN);
        } else {
            successView.setText(numFailures.toString());
            successView.setColor(Graphics.COLOR_RED);
        }
    }
    
    function setDebugView() {
        var last = Background.getLastTemporalEventTime();
        var timeSinceLast = 999;
        if (last != null) {
            timeSinceLast = Time.now().value() - last.value();
        }
        View.findDrawableById("DebugLabel").setText(timeSinceLast.format("%03d"));
    }
    
    function drawGraphs(dc) {
        // 214 x 180 according to simulator?
        var xmax = 214;
        var ymax = 180;
        var left = 48;
        var top = 120;
        var dx = xmax - 2 * left;
        var dy = ymax - top;
        
        dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_TRANSPARENT);
        dc.drawRectangle(left, top, dx, dy);
        
        dc.setColor(Graphics.COLOR_BLUE, Graphics.COLOR_TRANSPARENT);
        // precipitation graph  
        var bgInfoDict = Application.getApp().getProperty(BGDATA);
        if (bgInfoDict != null && bgInfoDict.get("precips") != null) {
            var precips = bgInfoDict.get("precips");
            var len = precips.size();
            for (var i = 0; i < len; i++) {
                if (precips[i] < precipMin) { continue; }
                var precip = bound(precips[i], precipMin, precipMax);
                var px = Math.round(1.0 * (i + 1) / (len + 1) * dx + left);
                var py = Math.round((1.0 - (precip - precipMin) / (precipMax - precipMin)) * dy + top); // y=0 is top of screen
                dc.fillRectangle(px - 3, py - 1, 2, 2);
            }
        }
        
        // hi/lo graph
        if (bgInfoDict != null && bgInfoDict.get("dhis") != null && bgInfoDict.get("dlos") != null) {
            var lows = bgInfoDict.get("dlos");
            var his = bgInfoDict.get("dhis");
            var dews = bgInfoDict.get("ddews");
            var numDays = lows.size();

	        for (var i = 0; i < numDays; i++) {
	            // generate plots, assume temp in (-10, 30)
	            var px = Math.round(1.0 * (i + 1) / (numDays + 1) * dx + left);

                // null check for backcompat
                dc.setColor(Graphics.COLOR_ORANGE, Graphics.COLOR_TRANSPARENT);
                if (lows != null) {
	               var low = bound(lows[i], -10.0, 35.0);
	               var lowpy = Math.round((1.0 - (low + 12.0) / 49) * dy + top);
	               dc.fillRectangle(px - 1, lowpy - 1, 3, 3);
	            }
	            
	            if (his != null) {
	               var hi = bound(his[i], -10.0, 35.0);
	               var hipy = Math.round((1.0 - (hi + 12.0) / 49) * dy + top);
	               dc.fillRectangle(px - 1, hipy - 1, 3, 3);
	            }

	            dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_TRANSPARENT);
	            if (dews != null) {
	               var dew = bound(dews[i], -10.0, 35.0);
	               var dewpy = Math.round((1.0 - (dew + 12.0) / 49) * dy + top);
	               dc.fillRectangle(px - 1, dewpy - 1, 3, 3);
	            }
	        }
	    }
    }

    function onUpdate(dc) {
        View.onUpdate(dc);
        setTimeView();
        setBattView();
        setStepView();
        setHRView();
        setWeatherView();
        setBluetooth();
        setSuccessLabel();
        setDebugView();
        
        drawGraphs(dc);
        
        // (iii) 2 plots, (iv) weather status (S, PS, etc.)
    }

    // Called when this View is removed from the screen. Save the
    // state of this View here. This includes freeing resources from
    // memory.
    function onHide() {
    }

    // The user has just looked at their watch. Timers and animations may be started here.
    function onExitSleep() {
    }

    // Terminate any active timers and prepare for slow updates.
    function onEnterSleep() {
    }
}


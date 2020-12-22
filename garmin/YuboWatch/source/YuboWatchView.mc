using Toybox.WatchUi;
using Toybox.Graphics;
using Toybox.System;
using Toybox.Lang;
using Toybox.Application;
using Toybox.ActivityMonitor;
using Toybox.Time;
using Toybox.Background;
using Toybox.Math;

function textFromBg(dat, idx, fmt) {
    if (idx >= dat.size()) {
        return "--";
    } else {
        return dat[idx].format(fmt);
    }
}
function getFromBg(dat, idx) {
    if (idx >= dat.size()) {
        return "--";
    } else {
        return dat[idx];
    }
}

function colorDraw(dc, x, y, dat, idx, fmt, colorPos, colorNeg, size) {
    var val;
    if (idx >= dat.size()) {
        val = 0;
    } else {
        val = dat[idx];
    }
    if (val < 0) {
        dc.setColor(colorNeg, Graphics.COLOR_TRANSPARENT);
        val = -val;
    } else {
        dc.setColor(colorPos, Graphics.COLOR_TRANSPARENT);
    }
    dc.drawText(x, y, size, val.format(fmt), Graphics.TEXT_JUSTIFY_LEFT);
}

function min(a, b) {
    return a > b ? b : a;
}
function max(a, b) {
    return a > b ? a : b;
}

var precipMax = Math.ln(50.0); // max 50mm/hr
var precipMin = Math.ln(0.1); // min 0.1mm/hr

class YuboWatchView extends WatchUi.WatchFace {
    function initialize() {
        if (Application.getApp().getProperty(BGDATA) == null) {
            Application.getApp().setProperty(BGDATA, []);
        }
        if (Application.getApp().getProperty(PRECIPS) == null) {
            Application.getApp().setProperty(PRECIPS, []);
        }
        WatchFace.initialize();
    }

    // Load your resources here
    function onLayout(dc) {}

    // Called when this View is brought to the foreground. Restore
    // the state of this View and prepare it to be shown. This includes
    // loading resources into memory.
    function onShow() {
    }

    function drawGraphs(dc) {
        // 214 x 180 according to simulator?
        var xmax = 214;
        var ymax = 180;
        var left = 48;
        var top = 120;
        var dx = xmax - 2 * left;
        var dy = ymax - top;
        var r = 2.0;
        var d = 2 * r + 1;

        dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_TRANSPARENT);
        dc.drawRectangle(left, top, dx, dy);

        dc.setColor(Graphics.COLOR_BLUE, Graphics.COLOR_TRANSPARENT);
        // precipitation graph
        var precips = Application.getApp().getProperty(PRECIPS);
        if (precips.size() > 0) {
            var len = precips.size();
            for (var i = 0; i < len; i++) {
                if (precips[i] < precipMin) { continue; }
                var precip = min(precips[i], precipMax);
                var px = Math.round(1.0 * (i + 1) / (len + 1) * dx + left);
                var py = Math.round((1.0 - (precip - precipMin) / (precipMax - precipMin)) * dy + top); // y=0 is top of screen
                dc.fillRectangle(px - 1, py - 1, 2, 2);
            }
        }

        var bgdat = Application.getApp().getProperty(BGDATA);
        // hi/lo graph
        if (bgdat.size() > 19) {
            var his = bgdat[17];
	        var lows = bgdat[18];
	        var dews = bgdat[19];
            var numDays = lows.size();
            var TEMP_MIN = lows[0];
            var TEMP_MAX = his[0];
            for (var i = 0; i < numDays; i++) {
                TEMP_MIN = min(min(TEMP_MIN, lows[i]), dews[i]);
                TEMP_MAX = max(TEMP_MAX, his[i]);
            }
            var mid5 = Math.round((TEMP_MIN + TEMP_MAX) / 10) * 5;

            var linepy = Math.round((1.0 - (mid5 - TEMP_MIN + 2.0) / (TEMP_MAX - TEMP_MIN + 4.0)) * dy + top);
            dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
            if (mid5 < 0) {
                mid5 = -mid5;
                dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
            }
            dc.drawText(46, linepy - 10, Graphics.FONT_SMALL, mid5.format("%02d"), Graphics.TEXT_JUSTIFY_RIGHT);

            dc.setColor(Graphics.COLOR_LT_GRAY, Graphics.COLOR_TRANSPARENT);
            dc.drawRectangle(left, linepy, dx, 2);
            dc.setColor(Graphics.COLOR_DK_GRAY, Graphics.COLOR_TRANSPARENT);
            for (var i = 1; mid5 + 5 * i < TEMP_MAX; i++) {
	            if (mid5 + 5 * i < TEMP_MAX) {
	                var line1py = Math.round((1.0 - (mid5 + 5 * i - TEMP_MIN + r + 1) / (TEMP_MAX - TEMP_MIN + 2 * r + 2)) * dy + top);
	                dc.drawLine(left + 1, line1py, left+dx - 2, line1py);
	            }
            }
            for (var i = 1; mid5 - 5 * i > TEMP_MIN; i++) {
	            if (mid5 - 5 > TEMP_MIN) {
	                var line2py = Math.round((1.0 - (mid5 - 5 * i - TEMP_MIN + r + 1) / (TEMP_MAX - TEMP_MIN + 2 * r + 2)) * dy + top);
	                dc.drawLine(left + 1, line2py, left+dx - 2, line2py);
	            }
	        }

	        for (var i = 0; i < numDays; i++) {
	            // generate plots, assume temp in (-10, 30)
	            var px = Math.round(1.0 * (i + 1) / (numDays + 1) * dx + left);

                dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
                var low = lows[i];
                var lowpy = Math.round((1.0 - (low - TEMP_MIN + r + 1) / (TEMP_MAX - TEMP_MIN + 2 * r + 2)) * dy + top);
                dc.fillRectangle(px - r, lowpy - r, d, d);

                dc.setColor(Graphics.COLOR_ORANGE, Graphics.COLOR_TRANSPARENT);
                var hi = his[i];
                var hipy = Math.round((1.0 - (hi - TEMP_MIN + 2.0) / (TEMP_MAX - TEMP_MIN + 4.0)) * dy + top);
                dc.fillRectangle(px - r, hipy - r, d, d);

	            dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_TRANSPARENT);
                var dew = dews[i];
                var dewpy = Math.round((1.0 - (dew - TEMP_MIN + 2.0) / (TEMP_MAX - TEMP_MIN + 4.0)) * dy + top);
                dc.fillRectangle(px - r, dewpy - r, d, d);
	        }
	    }
    }

    function onUpdate(dc) {
        View.onUpdate(dc);

        // set time view
        var timeFormat = "$1$$2$";
        var clockTime = System.getClockTime();
        var timeString = Lang.format(
            timeFormat,
            [clockTime.hour.format("%02d"), clockTime.min.format("%02d")]
        );
        dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(10, 62, Graphics.FONT_NUMBER_HOT, timeString, Graphics.TEXT_JUSTIFY_LEFT);

        var now = Time.Gregorian.info(Time.now(), Time.FORMAT_MEDIUM);
        var dateView = View.findDrawableById("DateLabel");
        var dateFormat = "$1$, $2$ $3$";
        var dateString = Lang.format(
            dateFormat,
            [now.day_of_week, now.month, now.day.format("%02d")]
        );
        dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(10, 46, Graphics.FONT_MEDIUM, dateString, Graphics.TEXT_JUSTIFY_LEFT);

        // set battery
        var myStats = System.getSystemStats();
        var battPerc = myStats.battery;
        var battStr = battPerc.format("%.1f");
        var battView = View.findDrawableById("BattLabel");
        dc.setColor(battPerc > 20 ? Graphics.COLOR_GREEN : Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
        dc.drawText(164, 9, Graphics.FONT_MEDIUM, battStr, Graphics.TEXT_JUSTIFY_RIGHT);

        // set memory
        var memPerc = (100.0 * myStats.usedMemory / myStats.totalMemory).format("%02d");
        dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(45, 116, Graphics.FONT_SMALL, Lang.format("($1$%)", [memPerc]), Graphics.TEXT_JUSTIFY_RIGHT);

        // set steps & cals
        var info = ActivityMonitor.getInfo();
        var steps = info.steps;
        var stepGoal = info.stepGoal;
        var calories = info.calories;
        dc.setColor(steps > stepGoal ? Graphics.COLOR_GREEN : Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(96, 9, Graphics.FONT_MEDIUM, steps.format("%05d"), Graphics.TEXT_JUSTIFY_CENTER);
        dc.setColor(Graphics.COLOR_ORANGE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(32, 28, Graphics.FONT_MEDIUM, calories.format("%04d"), Graphics.TEXT_JUSTIFY_LEFT);

        // set HR view
        var lastHRSample = ActivityMonitor.getHeartRateHistory(1, true).next();
        dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
        dc.drawText(32, 9, Graphics.FONT_MEDIUM,
            lastHRSample == null ? "N/A" : lastHRSample.heartRate.format("%03d"),
            Graphics.TEXT_JUSTIFY_LEFT);

        // set bluetooth view
        if (System.getDeviceSettings().phoneConnected) {
            dc.setColor(Graphics.COLOR_BLUE, Graphics.COLOR_TRANSPARENT);
            dc.drawText(102, 48, Graphics.FONT_SMALL, "(B)", Graphics.TEXT_JUSTIFY_LEFT);
        } else {
            dc.setColor(Graphics.COLOR_DK_GRAY, Graphics.COLOR_TRANSPARENT);
            dc.drawText(102, 48, Graphics.FONT_SMALL, "(X)", Graphics.TEXT_JUSTIFY_LEFT);
        }

        // set success view
        var numFailures = Application.getApp().getProperty(NUMFAILED);
        numFailures = numFailures ? numFailures : -1;
        if (numFailures >= 0) {
            dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
            dc.drawText(121, 48, Graphics.FONT_SMALL, numFailures.toString(), Graphics.TEXT_JUSTIFY_LEFT);
        } else {
            dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_TRANSPARENT);
            dc.drawText(121, 48, Graphics.FONT_SMALL, "Y", Graphics.TEXT_JUSTIFY_LEFT);
        }

        // set debug view
        var last = Background.getLastTemporalEventTime();
        var timeSinceLast = 999;
        if (last != null) {
            timeSinceLast = Time.now().value() - last.value();
        }
        dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
        dc.drawText(102, 82, Graphics.FONT_SMALL, timeSinceLast.format("%03d"), Graphics.TEXT_JUSTIFY_LEFT);

        // set all weather
        var bgdat = Application.getApp().getProperty(BGDATA);

        colorDraw(dc, 139, 75, bgdat, 0, "%04.1f", Graphics.COLOR_WHITE, Graphics.COLOR_RED, Graphics.FONT_LARGE);
        colorDraw(dc, 139, 61, bgdat, 7, "%02d", Graphics.COLOR_WHITE, Graphics.COLOR_RED, Graphics.FONT_SMALL);
        colorDraw(dc, 158, 61, bgdat, 3, "%02d", Graphics.COLOR_BLUE, Graphics.COLOR_PURPLE, Graphics.FONT_SMALL);
        colorDraw(dc, 177, 61, bgdat, 8, "%02d", Graphics.COLOR_WHITE, Graphics.COLOR_RED, Graphics.FONT_SMALL);
        colorDraw(dc, 139, 47, bgdat, 9, "%02d", Graphics.COLOR_LT_GRAY, Graphics.COLOR_RED, Graphics.FONT_SMALL);
        colorDraw(dc, 158, 47, bgdat, 11, "%02d", Graphics.COLOR_BLUE, Graphics.COLOR_PURPLE, Graphics.FONT_SMALL);
        colorDraw(dc, 177, 47, bgdat, 10, "%02d", Graphics.COLOR_LT_GRAY, Graphics.COLOR_RED, Graphics.FONT_SMALL);

        dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_TRANSPARENT);
        dc.drawText(139, 101, Graphics.FONT_SMALL, textFromBg(bgdat, 1, "%02d"), Graphics.TEXT_JUSTIFY_LEFT);
        dc.setColor(Graphics.COLOR_BLUE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(171, 101, Graphics.FONT_SMALL, textFromBg(bgdat, 2, "%02d") + "%", Graphics.TEXT_JUSTIFY_LEFT);
        dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
        dc.drawText(156, 101, Graphics.FONT_SMALL, textFromBg(bgdat, 6, "%02d"), Graphics.TEXT_JUSTIFY_LEFT);
        dc.setColor(Graphics.COLOR_YELLOW, Graphics.COLOR_TRANSPARENT);
        dc.drawText(103, 67, Graphics.FONT_SMALL, getFromBg(bgdat, 4), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawText(103, 100, Graphics.FONT_SMALL, getFromBg(bgdat, 5), Graphics.TEXT_JUSTIFY_LEFT);
        dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(187, 75, Graphics.FONT_SMALL, getFromBg(bgdat, 15), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawText(187, 87, Graphics.FONT_SMALL, getFromBg(bgdat, 16), Graphics.TEXT_JUSTIFY_LEFT);

        dc.drawText(168, 118, Graphics.FONT_SMALL, getFromBg(bgdat, 12), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawText(168, 130, Graphics.FONT_SMALL, getFromBg(bgdat, 13), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawText(168, 142, Graphics.FONT_SMALL, getFromBg(bgdat, 14), Graphics.TEXT_JUSTIFY_LEFT);

        var positionInfo = Position.getInfo();
        var bglat = lat;
        var bglon = lon;
        var alt = 0;
        if (positionInfo has :position && positionInfo.position != null) {
            bglat = positionInfo.position.toDegrees()[0];
            bglon = positionInfo.position.toDegrees()[1];
        }
        if (positionInfo has :altitude && positionInfo.altitude != null) {
            alt = positionInfo.altitude;
        }
        var latLonStr = Lang.format("$1$/$2$/$3$", [bglat.format("%05.2f"), bglon.format("%05.2f"), alt.format("%04d")]);
        dc.drawText(73, 31, Graphics.FONT_SMALL, latLonStr, Graphics.TEXT_JUSTIFY_LEFT);
        drawGraphs(dc);
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


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

const precipMax = Math.ln(50.0); // max 50mm/hr
const precipMin = Math.ln(0.1); // min 0.1mm/hr

class YuboWatchView extends WatchUi.WatchFace {
    function initialize() {
        if (Application.getApp().getProperty(BGDATA) == null) {
            Application.getApp().setProperty(BGDATA, []);
        }
        WatchFace.initialize();
    }

    // Load your resources here
    function onLayout(dc) {}
    function onShow() {}

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
        var i = 0;

        dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_BLACK);
        dc.drawRectangle(left, top, dx, dy);

        dc.setColor(Graphics.COLOR_BLUE, Graphics.COLOR_TRANSPARENT);

        var bgdat = Application.getApp().getProperty(BGDATA);
        // hi/lo graph
        if (bgdat.size() == 26) {
            var TEMP_MIN = bgdat[12];
            var TEMP_MAX = bgdat[19];
            for (i = 1; i < 7; i++) {
                TEMP_MIN = Math.floor(TEMP_MIN > bgdat[12 + i] ? bgdat[12 + i] : TEMP_MIN);
                TEMP_MAX = Math.ceil(TEMP_MAX > bgdat[19 + i] ? TEMP_MAX : bgdat[19 + i]);
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
            for (i = 1; mid5 + 5 * i <= TEMP_MAX; i++) {
                linepy = Math.round((1.0 - (mid5 + 5 * i - TEMP_MIN + r + 1) / (TEMP_MAX - TEMP_MIN + 2 * r + 2)) * dy + top);
                dc.drawLine(left + 1, linepy, left+dx - 2, linepy);
            }
            for (i = 1; mid5 - 5 * i >= TEMP_MIN; i++) {
                linepy = Math.round((1.0 - (mid5 - 5 * i - TEMP_MIN + r + 1) / (TEMP_MAX - TEMP_MIN + 2 * r + 2)) * dy + top);
                dc.drawLine(left + 1, linepy, left+dx - 2, linepy);
	        }

            dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
	        for (i = 0; i < 7; i++) {
	            // generate plots, assume temp in (-10, 30)
	            var px = Math.round(1.0 * (i + 1) / 8 * dx + left);
                var y = Math.round((1.0 - (bgdat[19 + i] - TEMP_MIN + r + 1) / (TEMP_MAX - TEMP_MIN + 2 * r + 2)) * dy + top);
                dc.fillRectangle(px - r, y - r, d, d);
            }
            dc.setColor(Graphics.COLOR_ORANGE, Graphics.COLOR_TRANSPARENT);
            for (i = 0; i < 7; i++) {
                // generate plots, assume temp in (-10, 30)
                var px = Math.round(1.0 * (i + 1) / 8 * dx + left);
                var y = Math.round((1.0 - (bgdat[12 + i] - TEMP_MIN + 2.0) / (TEMP_MAX - TEMP_MIN + 4.0)) * dy + top);
                dc.fillRectangle(px - r, y - r, d, d);
	        }
	    }
    }

    public function onUpdate(dc) {
        // empty screen out
        dc.setColor(Graphics.COLOR_BLACK, Graphics.COLOR_BLACK);
        dc.clear();

        // set time view
        var time = System.getClockTime();
        dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(10, 62, Graphics.FONT_NUMBER_HOT, Lang.format(
            "$1$$2$",
            [time.hour.format("%02d"), time.min.format("%02d")]
        ), Graphics.TEXT_JUSTIFY_LEFT);

        time = Time.Gregorian.info(Time.now(), Time.FORMAT_MEDIUM);
        dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(10, 46, Graphics.FONT_MEDIUM, Lang.format(
            "$1$, $2$ $3$",
            [time.day_of_week, time.month, time.day.format("%02d")]
        ), Graphics.TEXT_JUSTIFY_LEFT);

        // set battery
        var v = System.getSystemStats();
        dc.setColor(v.battery > 20 ? Graphics.COLOR_GREEN : Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
        dc.drawText(164, 9, Graphics.FONT_MEDIUM, v.battery.format("%.1f"), Graphics.TEXT_JUSTIFY_RIGHT);
        
        // draw last response code
        v = Application.getApp().getProperty(LASTCODE1);
        if (v != null) {
            dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
            dc.drawText(45, 116, Graphics.FONT_SMALL, Lang.format("$1$", [v]), Graphics.TEXT_JUSTIFY_RIGHT);
        }

        // set steps & cals
        v = ActivityMonitor.getInfo();
        dc.setColor(v.steps > v.stepGoal ? Graphics.COLOR_GREEN : Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(96, 12, Graphics.FONT_MEDIUM, v.steps.format("%05d"), Graphics.TEXT_JUSTIFY_CENTER);
        dc.setColor(Graphics.COLOR_DK_GRAY, Graphics.COLOR_TRANSPARENT);
        dc.drawText(96, -2, Graphics.FONT_TINY, v.stepGoal.format("%05d"), Graphics.TEXT_JUSTIFY_CENTER);
        dc.setColor(Graphics.COLOR_ORANGE, Graphics.COLOR_TRANSPARENT);
        dc.drawText(32, 28, Graphics.FONT_MEDIUM, v.calories.format("%04d"), Graphics.TEXT_JUSTIFY_LEFT);

        // set HR view
        v = ActivityMonitor.getHeartRateHistory(1, true).next();
        dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
        dc.drawText(32, 9, Graphics.FONT_MEDIUM,
            v == null ? "N/A" : v.heartRate.format("%03d"),
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
        var nf = Application.getApp().getProperty(NUMFAILED);
        nf = nf ? nf : -1;
        if (nf > 0) {
            dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
            dc.drawText(121, 48, Graphics.FONT_SMALL, nf.toString(), Graphics.TEXT_JUSTIFY_LEFT);
        } else {
            dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_TRANSPARENT);
            dc.drawText(121, 48, Graphics.FONT_SMALL, "Y", Graphics.TEXT_JUSTIFY_LEFT);
        }

        // set debug view
        v = Background.getLastTemporalEventTime();
        var timeSinceLast = v == null ? 999 : Time.now().value() - v.value();

        dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
        dc.drawText(102, 82, Graphics.FONT_SMALL, timeSinceLast.format("%03d"), Graphics.TEXT_JUSTIFY_LEFT);

        // set all weather
        var bgdat = Application.getApp().getProperty(BGDATA);

        colorDraw(dc, 139, 75, bgdat, 0, "%04.1f", Graphics.COLOR_WHITE, Graphics.COLOR_RED, Graphics.FONT_LARGE);
        colorDraw(dc, 139, 61, bgdat, 12, "%02d", Graphics.COLOR_WHITE, Graphics.COLOR_RED, Graphics.FONT_SMALL);
        colorDraw(dc, 158, 61, bgdat, 3, "%02d", Graphics.COLOR_BLUE, Graphics.COLOR_PURPLE, Graphics.FONT_SMALL);
        colorDraw(dc, 177, 61, bgdat, 19, "%02d", Graphics.COLOR_WHITE, Graphics.COLOR_RED, Graphics.FONT_SMALL);
        colorDraw(dc, 139, 47, bgdat, 13, "%02d", Graphics.COLOR_LT_GRAY, Graphics.COLOR_RED, Graphics.FONT_SMALL);
        colorDraw(dc, 158, 47, bgdat, 27, "%02d", Graphics.COLOR_BLUE, Graphics.COLOR_PURPLE, Graphics.FONT_SMALL);
        colorDraw(dc, 177, 47, bgdat, 20, "%02d", Graphics.COLOR_LT_GRAY, Graphics.COLOR_RED, Graphics.FONT_SMALL);

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
        dc.drawText(187, 75, Graphics.FONT_SMALL, getFromBg(bgdat, 10), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawText(187, 87, Graphics.FONT_SMALL, getFromBg(bgdat, 11), Graphics.TEXT_JUSTIFY_LEFT);

        dc.drawText(168, 118, Graphics.FONT_SMALL, getFromBg(bgdat, 7), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawText(168, 130, Graphics.FONT_SMALL, getFromBg(bgdat, 8), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawText(168, 142, Graphics.FONT_SMALL, getFromBg(bgdat, 9), Graphics.TEXT_JUSTIFY_LEFT);

        v = Position.getInfo();
        var bglat = lat;
        var bglon = lon;
        var alt = 0;
        if (v has :position && v.position != null) {
            bglat = v.position.toDegrees()[0];
            bglon = v.position.toDegrees()[1];
        }
        if (v has :altitude && v.altitude != null) {
            alt = v.altitude;
        }
        dc.drawText(73, 31, Graphics.FONT_SMALL, Lang.format("$1$/$2$/$3$", [bglat.format("%05.2f"), bglon.format("%05.2f"), alt.format("%04d")]), Graphics.TEXT_JUSTIFY_LEFT);
        drawGraphs(dc);
    }
    function onHide() {
    }
    function onExitSleep() {
    }
    function onEnterSleep() {
    }
}


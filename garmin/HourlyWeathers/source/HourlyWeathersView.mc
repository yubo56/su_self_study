using Toybox.WatchUi;
using Toybox.Communications;
using Toybox.Time;
using Toybox.Math;

function min(a, b) {
    return a > b ? b : a;
}
function max(a, b) {
    return a > b ? a : b;
}

var lat = 42.4522; // default location, cornell
var lon = -76.4804;
var key = "3D7nJJiv5g1xQmBvyIkjdAyJPRzjvCnv";
var site = "https://api.tomorrow.io/v4/timelines";

var precipMax = 50.0; // max 50mm/hr
var precipMin = 0.1; // min 0.1mm/hr

var xmax = 214;
var ymax = 180;
var left = 26;
var top = 26;
var dx = xmax - 2 * left;
var dy = ymax - 2 * top;
var r = 1.0;
var d = 2 * r + 1;
var numdays = 3;
var numhours = 72;

class HourlyWeathersView extends WatchUi.View {
    var dc;
    var temps = [];
    var dews = [];
    var precipsHr = [];

    function initialize() {
        View.initialize();
    }

    // Load your resources here
    function onLayout(dc) {}

    // Called when this View is brought to the foreground. Restore
    // the state of this View and prepare it to be shown. This includes
    // loading resources into memory.
    function onShow() {
        var now = Time.now();
        var twoDays = new Time.Duration(numdays * Time.Gregorian.SECONDS_PER_DAY);
        var today = Time.Gregorian.utcInfo(now, Time.FORMAT_SHORT);
        var later = Time.Gregorian.utcInfo(now.add(twoDays), Time.FORMAT_SHORT);
        var fmtstr = "$1$-$2$-$3$T$4$:$5$:$6$Z";
        var startstr = Lang.format(
            fmtstr,
            [
                today.year,
                today.month.format("%02d"),
                today.day.format("%02d"),
                today.hour.format("%02d"),
                today.min.format("%02d"),
                today.sec.format("%02d")
            ]
        );
        var endstr = Lang.format(
            fmtstr,
            [
                later.year,
                later.month.format("%02d"),
                later.day.format("%02d"),
                later.hour.format("%02d"),
                later.min.format("%02d"),
                later.sec.format("%02d")
            ]
        );
        var positionInfo = Position.getInfo();
        var bglat = lat;
        var bglon = lon;
        if (positionInfo has :position && positionInfo.position != null) {
            bglat = positionInfo.position.toDegrees()[0];
            bglon = positionInfo.position.toDegrees()[1];
        }
        var locstr = Lang.format("$1$,$2$", [bglat.format("%.2f"), bglon.format("%.2f")]);

        Communications.makeWebRequest(
            site,
            {
                "location" => locstr,
                "fields" => "temperature,dewPoint,precipitationIntensity",
                "startTime" => startstr,
                "endTime" => endstr,
                "timesteps" => "1h",
                "apikey" => key
            },
            {
                :method => Communications.HTTP_REQUEST_METHOD_GET,
                :responseType => Communications.HTTP_RESPONSE_CONTENT_TYPE_JSON
            },
            new Lang.Method(self, :climacellHourlyCb)
        );
    }
    function climacellHourlyCb(responseCode, responseBody) {
        if (responseCode != 200) {
            System.println(responseCode);
            return;
        }
        var responseIntervals = responseBody.get("data").get("timelines")[0].get("intervals");
        temps = new [numhours];
        dews = new [numhours];
        precipsHr = new [numhours];
        for (var i = 0; i < numhours; i++) {
            temps[i] = responseIntervals[i].get("values").get("temperature");
            dews[i] = responseIntervals[i].get("values").get("dewPoint");
            var precip = responseIntervals[i].get("values").get("precipitationIntensity");
            precipsHr[i] = Math.ln(max(precipMin, min(precipMax, precip)));
        }
        WatchUi.requestUpdate();
    }
    
    // Updates the view
    function onUpdate(dc) {
        dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_BLACK);
        dc.clear();
        dc.drawRectangle(left, top, dx, dy);
        
        var now = Time.now();
        for (var i = 0; i < numdays; i++) {
            var fracdayRemaining = (24.0 - System.getClockTime().hour) / 24;
            dc.setColor(Graphics.COLOR_LT_GRAY, Graphics.COLOR_BLACK);
            dc.drawLine(left + (i + fracdayRemaining) * dx / 3, top, left + (i + fracdayRemaining) * dx / 3, top + dy);
            
            var timeDelta = new Time.Duration((i + 1) * Time.Gregorian.SECONDS_PER_DAY);
	        var later = Time.Gregorian.info(now.add(timeDelta), Time.FORMAT_SHORT);
            dc.drawText(left + (i + 0.5 + fracdayRemaining) * dx / 3, 6, Graphics.FONT_TINY, later.day.format("%02d"), Graphics.TEXT_JUSTIFY_CENTER);
        }
        
        if (temps.size() == 0) { return; }
        
        var TEMP_MIN = temps[0];
        var TEMP_MAX = temps[0];
        for (var i = 1; i < numhours; i++) {
            TEMP_MIN = min(min(TEMP_MIN, temps[i]), dews[i]);
            TEMP_MAX = max(max(TEMP_MAX, temps[i]), dews[i]);
        }
        var mid5 = Math.round((TEMP_MIN + TEMP_MAX) / 10) * 5;
    
        var linepy = Math.round((1.0 - (mid5 - TEMP_MIN + 2.0) / (TEMP_MAX - TEMP_MIN + 4.0)) * dy + top);
        dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_TRANSPARENT);
        if (mid5 < 0) {
            mid5 = -mid5;
            dc.setColor(Graphics.COLOR_RED, Graphics.COLOR_TRANSPARENT);
        }
        dc.drawText(20, linepy - 10, Graphics.FONT_SMALL, mid5.format("%02d"), Graphics.TEXT_JUSTIFY_RIGHT);
    
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

        dc.setColor(Graphics.COLOR_ORANGE, Graphics.COLOR_TRANSPARENT);
        for (var i = 0; i < numhours; i++) {
            // generate plots, assume temp in (-10, 30)
            var px = Math.round(1.0 * (i + 1) / (numhours + 1) * dx + left);

            dc.setColor(Graphics.COLOR_ORANGE, Graphics.COLOR_TRANSPARENT);
            var temppy = Math.round((1.0 - (temps[i] - TEMP_MIN + 2.0) / (TEMP_MAX - TEMP_MIN + 4.0)) * dy + top);
            dc.fillRectangle(px - r, temppy - r, d, d);
            
            dc.setColor(Graphics.COLOR_GREEN, Graphics.COLOR_TRANSPARENT);
            var dewpy = Math.round((1.0 - (dews[i] - TEMP_MIN + 2.0) / (TEMP_MAX - TEMP_MIN + 4.0)) * dy + top);
            dc.fillRectangle(px - r, dewpy - r, d, d);
            
            dc.setColor(Graphics.COLOR_BLUE, Graphics.COLOR_TRANSPARENT);
            var limmax = Math.ln(precipMax);
            var limmin = Math.ln(precipMin);
            var py = Math.round((1.0 - (precipsHr[i] - limmin) / (limmax - limmin)) * dy + top); // y=0 is top of screen
            dc.fillRectangle(px - r, py - r, d, d);
        }
    }

    // Called when this View is removed from the screen. Save the
    // state of this View here. This includes freeing resources from
    // memory.
    function onHide() {
    }

}

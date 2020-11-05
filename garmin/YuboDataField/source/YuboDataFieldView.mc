using Toybox.WatchUi;
using Toybox.Graphics;
using Toybox.AntPlus;

class YuboDataFieldView extends WatchUi.DataField {

    var infoDict;
    var rd;
    var lastSplitTime = 0;
    var numSplits = 0;

    function initialize() {
        DataField.initialize();
        var rdlistener = new AntPlus.RunningDynamicsListener();
        rd = new AntPlus.RunningDynamics(rdlistener);
        
        // time elapsed, dist elapsed, hrate, cadence (RD), L/R balance, pace, current split, power(?), cals,  
        infoDict = {
            "telapsed" => "0:00:00", // h:mm:ss (input ms)
            "delapsed" => "00000", // m
            "hrate" => "000",
            "maxhrate" => "00",
            "pace" => "0:00", // min/km (input m/s)
            "power" => "000", // W
            "cals" => "0000",
            "tsplit" => "0:00", // m:ss / km
            "effect" => "0.0", // training effect
            
            "cadence" => "00", // revolutions cadence, = running cadence / 2
            "gctb" => "00.0",
            "vert" => "00.0", // vertical oscillation, mm
            "stance" => "00.0", // fraction of time in stance phase
        };
    }

    function onLayout(dc) {
        return true;
    }


    function compute(info) {
        if (info has :elapsedTime && info.elapsedTime != null) {
            var seconds = Math.round(info.elapsedTime / 1000);
            infoDict.put("telapsed", Lang.format("$1$:$2$:$3$", [
                (seconds / 3600).toNumber().toString(),
                ((seconds % 3600) / 60).toNumber().format("%02d"),
                (seconds % 60).format("%02d")
            ]));
        }
        if (info has :elapsedDistance && info.elapsedDistance != null) {
            infoDict.put("delapsed", (info.elapsedDistance).format("%05d"));
        }
        if (info has :currentHeartRate && info.currentHeartRate != null) {
            infoDict.put("hrate", (info.currentHeartRate % 100).format("%02d"));
        }
        if (info has :currentSpeed && info.currentSpeed != null && info.currentSpeed > 0) {
            var pace = 1000.0 / (60 * info.currentSpeed); // min/km, frac
            infoDict.put("pace", Lang.format("$1$:$2$", [
                (pace.toNumber() % 100).format("%02d"),
                ((pace - pace.toNumber()) * 60).format("%02d")
            ]));
        }
        if (info has :calories && info.calories != null) {
            infoDict.put("cals", info.calories.format("%04d"));
        }
        if (info has :elapsedDistance && info.elapsedDistance != null &&
                info has :elapsedTime && info.elapsedTime != null) {
            var splitsRan = (info.elapsedDistance / 1000).toNumber();
            if (splitsRan > numSplits) {
                lastSplitTime = info.elapsedTime;
                numSplits = splitsRan;
            }
            var timeSplit = ((info.elapsedTime - lastSplitTime) / 1000).toNumber();
            infoDict.put("tsplit", Lang.format("$1$:$2$", [
                ((timeSplit / 60) % 100).toNumber().format("%01d"),
                (timeSplit % 60).format("%02d")
            ]));
        }
        if (info has :energyExpenditure && info.energyExpenditure != null) {
            infoDict.put("power", (info.energyExpenditure * 69.78 * 0.239).format("%03d"));
        }
        if (info has :trainingEffect && info.trainingEffect != null) {
            infoDict.put("effect", info.trainingEffect.format("%03.1f")); 
        }
        if (info has :maxHeartRate && info.maxHeartRate != null) {
            infoDict.put("maxhrate", (info.maxHeartRate % 100).format("%02d"));
        }
        
        var data = rd.getRunningDynamics();
        if (data == null) { return; }
        if (data has :cadence && data.cadence != null) {
            infoDict.put("cadence", data.cadence.format("%02d"));
        }
        if (data has :groundContactBalance && data.groundContactBalance != null) {
            infoDict.put("gctb", data.groundContactBalance.format("%04.1f"));
        }
        if (data has :verticalOscillation && data.verticalOscillation != null) {
            infoDict.put("vert", data.verticalOscillation.format("%04.1f"));
        }
        if (data has :stanceTime && data.stanceTime != null) {
            infoDict.put("stance", data.stanceTime.format("%04.1f"));
        }
    }

    // Display the value you computed here. This will be called
    // once a second when the data field is visible.
    function onUpdate(dc) {
        // Set the background color
        // Call parent's onUpdate(dc) to redraw the layout
        View.onUpdate(dc);
        var width = dc.getWidth();
        var height = dc.getHeight();
        
        var fs = Graphics.FONT_NUMBER_MILD;
        var fs2 = Graphics.FONT_NUMBER_HOT;
        var fh = Graphics.getFontHeight(fs);
        var fh2 = Graphics.getFontHeight(fs2);
        var cbg = Graphics.COLOR_TRANSPARENT;
        var h1 = 0.19 * height,
            h2 = 0.44 * height,
            h3 = 0.69 * height,
            h4 = h3 + fh - 2; // y locs of horizontal lines
        var y1 = h1 / 2 - fh / 2.5,
            y2 = (h2 + h1) / 2 - fh2 / 2 - 2, // slightly poorly centered?
            y3 = (h3 + h2) / 2 - fh2 / 2 - 2,
            y4 = (height + h3) / 2 - fh / 2;
        var xl = 0.48 * width,
            xr = 0.52 * width;
        
        dc.setColor(Graphics.COLOR_WHITE, cbg);
        dc.drawLine(0.5 * width, 0, 0.5 * width, h3);
        dc.drawLine(0, h1, width, h1);
        dc.drawLine(0, h2, width, h2);
        dc.drawLine(0, h3, width, h3);
        dc.drawLine(0, h4, width, h4);

        dc.drawText(xl, y1, fs, infoDict.get("telapsed"), Graphics.TEXT_JUSTIFY_RIGHT);
        dc.drawText(xr, y1, fs, infoDict.get("delapsed"), Graphics.TEXT_JUSTIFY_LEFT);
            
        dc.drawText(xl, y2, fs2, infoDict.get("hrate"), Graphics.TEXT_JUSTIFY_RIGHT);
        dc.drawText(xr, y2, fs2, infoDict.get("cadence"), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawText(0.76 * width, y2 + 8, Graphics.FONT_NUMBER_MEDIUM, infoDict.get("effect"), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawLine(0.74 * width, h1, 0.74 * width, h2);
        
        dc.drawText(xl, y3, fs2, infoDict.get("pace"), Graphics.TEXT_JUSTIFY_RIGHT);
        dc.drawText(xr, y3, fs2, infoDict.get("gctb"), Graphics.TEXT_JUSTIFY_LEFT);
        
        dc.drawText(0.21 * width, h3 - 2, fs, infoDict.get("tsplit"), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawLine(0.4 * width, h3 - 2, 0.4 * width, h4);
        dc.drawText(0.42 * width, h3 - 2, fs, infoDict.get("cals"), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawLine(0.63 * width, h3 - 2, 0.63 * width, h4);
        dc.drawText(0.65 * width, h3 - 2, fs, infoDict.get("power"), Graphics.TEXT_JUSTIFY_LEFT);
        
        dc.drawText(xl, h4 - 3, fs, infoDict.get("stance"), Graphics.TEXT_JUSTIFY_RIGHT);
        dc.drawText(xr, h4 - 3, fs, infoDict.get("vert"), Graphics.TEXT_JUSTIFY_LEFT);
        dc.drawLine(0.5 * width, h4, 0.5 * width, height);
        
        dc.setColor(Graphics.COLOR_ORANGE, cbg);
        dc.drawText(12, y2, fs2, infoDict.get("maxhrate"), Graphics.TEXT_JUSTIFY_LEFT);
    }

}

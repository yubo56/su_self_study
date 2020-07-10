using Toybox.Application;
using Toybox.Background;
using Toybox.WatchUi;
using Toybox.Time;
using Toybox.System;

var BGDATA = "bgdata";
var NUMFAILED = "numfailed";
var BKGD_MINS = 5;

(:background)
class YuboWatchApp extends Application.AppBase {

    function initialize() {
        AppBase.initialize();
    }

    // onStart() is called on application start up
    function onStart(state) {
    }

    // onStop() is called when your application is exiting
    function onStop(state) {
    }

    // 15m recurring background
    function getInitialView() { 
        Background.registerForTemporalEvent(new Time.Duration(BKGD_MINS * 60));
        return [ new YuboWatchView() ];
    }

    // New app settings have been received so trigger a UI update
    function onSettingsChanged() {
        WatchUi.requestUpdate();
    }

    function getServiceDelegate() {
        return [ new YuboWatchSDelegate() ];
    }

    function onBackgroundData(data) {   
        if (data[0] != null) {
            System.println("Updating bgdata");
            Application.getApp().setProperty(BGDATA, data[0]);
        }
        if (data[1]) {
            Application.getApp().setProperty(NUMFAILED, 0);
        } else {
            var numFailed = Application.getApp().getProperty(NUMFAILED);
            if (numFailed == null) {
                numFailed = 0;
            } 
            Application.getApp().setProperty(NUMFAILED, numFailed + 1);
        }
    }
}

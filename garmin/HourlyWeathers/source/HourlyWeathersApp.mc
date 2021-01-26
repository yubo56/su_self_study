using Toybox.Application;

var BGDATA = "bgdata";
var NUMFAILED = "numfailed";
var BKGD_MINS = 5;

class HourlyWeathersApp extends Application.AppBase {

    function initialize() {
        AppBase.initialize();
    }

    // onStart() is called on application start up
    function onStart(state) {
    }

    // onStop() is called when your application is exiting
    function onStop(state) {
    }

    // Return the initial view of your application here
    function getInitialView() {
        return [ new HourlyWeathersView() ];
    }

    function onBackgroundData(data) {
        if (data[0].size() > 0) {
            Application.getApp().setProperty(BGDATA, data[0]);
        }
        if (data[1]) {
            Application.getApp().deleteProperty(NUMFAILED);
        } else {
            var numFailed = Application.getApp().getProperty(NUMFAILED);
            if (numFailed == null) {
                numFailed = 0;
            }
            Application.getApp().setProperty(NUMFAILED, numFailed + 1);
        }
    }
}
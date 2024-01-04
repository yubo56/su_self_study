using Toybox.Application;
using Toybox.Background;
using Toybox.WatchUi;
using Toybox.Time;

enum {
    BGDATA,
    PRECIPS, // UNUSED
    NUMFAILED,
    LASTBATT,
    LASTCODE1,
}
const BKGD_MINS = 5;

(:background)
class YuboWatchApp extends Application.AppBase {

    function initialize() {
        AppBase.initialize();
    }

    function onStart(state) {
    }

    function onStop(state) {
    }

    // 5m recurring background
    function getInitialView() {
        Background.registerForTemporalEvent(new Time.Duration(BKGD_MINS * 60));
        return [ new YuboWatchView() ];
    }

    function onSettingsChanged() {
        WatchUi.requestUpdate();
    }

    function getServiceDelegate() {
        return [ new YuboWatchSDelegate() ];
    }

    function onBackgroundData(data) {
        if (data[0].size() > 0) {
            Application.getApp().setProperty(BGDATA, data[0]);
        }
        Application.getApp().setProperty(LASTCODE1, data[2]);
        if (data[1]) {
            Application.getApp().setProperty(NUMFAILED, 0);
        } else {
            var numFailed = Application.getApp().getProperty(NUMFAILED);
            Application.getApp().setProperty(NUMFAILED, numFailed ? numFailed + 1 : 1);
        }
    }
}

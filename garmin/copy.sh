#!/bin/bash
rsync -a ~/garmin-workspace/* . --exclude=bin
sed -i '' 's/var appid.*/var appid = "";/g' YuboWatch/source/YuboWatchSDelegate.mc
sed -i '' 's/var key.*/var key = "";/g' HourlyWeathers/source/HourlyWeathersView.mc

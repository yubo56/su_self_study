#!/bin/bash
rsync -a ~/garmin-workspace/* . --exclude=bin
sed -i '' 's/const appid.*/const appid = "";/g' YuboWatch/source/YuboWatchSDelegate.mc
sed -i '' 's/const key.*/const key = "";/g' YuboWatch/source/YuboWatchSDelegate.mc
sed -i '' 's/const appid.*/const appid = "";/g' YuboWatch_OWM/source/YuboOWMWatchSDelegate.mc
sed -i '' 's/const key.*/const key = "";/g' YuboWatch_OWM/source/YuboOWMWatchSDelegate.mc
sed -i '' 's/const appid.*/const appid = "";/g' HourlyWeathers/source/HourlyWeathersView.mc
sed -i '' 's/const key.*/const key = "";/g' HourlyWeathers/source/HourlyWeathersView.mc

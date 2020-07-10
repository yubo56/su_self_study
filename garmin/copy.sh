#!/bin/bash
rsync -a ~/eclipse-workspace/Yubo* . --exclude=bin
sed -i '' 's/var appid.*/var appid = "";/g' YuboWatch/source/YuboWatchSDelegate.mc

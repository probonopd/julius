#!/usr/bin/env bash

docker exec appimage /bin/bash -c "cd build
apt-get update
apt-get install -y wget libcairo2
wget https://github.com/TheAssassin/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy*.AppImage

./linuxdeploy*.AppImage --appimage-extract-and-run --appdir AppDir/ --output appimage
"

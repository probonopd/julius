#!/usr/bin/env bash

case "$BUILD_TARGET" in
"appimage")
    # At the time of writing this code this image corresponds to CentOS 6
    # The idea of AppImage is that you have to build on the oldest distribution that you want to support
    # https://github.com/AppImage/AppImageKit/blob/ee71440ded7c450decb5643fbcfcc9e4c618ec7b/.travis.yml#L9
	docker run -d --name appimage --device /dev/fuse:mrw --workdir /build/git -v "${PWD}:/build/git" quay.io/appimage/appimagebuild tail -f /dev/null
	;;
"vita")
	docker run -d --name vitasdk --workdir /build/git -v "${PWD}:/build/git" gnuton/vitasdk-docker tail -f /dev/null
	;;
"switch")
	docker run -d --name switchdev --workdir /build/git -v "${PWD}:/build/git" rsn8887/switchdev tail -f /dev/null
	;;
esac

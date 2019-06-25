#!/usr/bin/env bash

case "$BUILD_TARGET" in
"appimage")
	docker exec appimage /bin/bash -c "cd build && make && make install DESTDIR=AppDir"
	bash package_appimage.sh
	;;
"vita")
	docker exec vitasdk /bin/bash -c "cd build && make"
	;;
"switch")
	docker exec switchdev /bin/bash -c "cd build && make"
	;;
"mac")
	cd build && make && make test && make install
	echo "Creating disk image"
	hdiutil create -volname Julius -srcfolder julius.app -ov -format UDZO julius.dmg
	;;
*)
	cd build && make && make test
	;;
esac

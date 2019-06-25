#!/usr/bin/env bash

case "$BUILD_TARGET" in
"appimage")
    docker exec -e BUILD_TARGET="$BUILD_TARGET" -e SDL_LIB="$SDL_LIB" -e SDL_MIXER_LIB="$SDL_MIXER_LIB" appimage /bin/bash ".ci_scripts/install_sdl.sh"
	docker exec appimage /bin/bash -c "mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr .."
	;;
"vita")
	docker exec vitasdk /bin/bash -c "mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DVITA_BUILD=ON .."
	;;
"switch")
	docker exec switchdev /bin/bash -c "mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DSWITCH_BUILD=ON .."
	;;
"mac")
	mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release ..
	;;
*)
	mkdir build && cd build && cmake ..
	;;
esac

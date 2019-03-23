if [ "$VITA_BUILD" = "true" ];
then
  docker exec vitasdk /bin/bash -c "mkdir build && cd build && cmake -DVITA_BUILD=ON .."
elif [ "$SWITCH_BUILD" = "true" ];
then
  docker exec switchdev /bin/bash -c "mkdir build && cd build && cmake -DSWITCH_BUILD=ON .."
else
  mkdir build && cd build
  if [ $MUSL_LIBC ];
  then
    # Static build using musl which is a portable libc
    export CC=musl-gcc
    cmake -DCMAKE_EXE_LINKER_FLAGS="-static" -DSDL2_USE_STATIC_LIBS=ON ..
  else
    cmake ..
  fi
fi;

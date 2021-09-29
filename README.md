# AndroidAudioHalHelper

This is helper (utility) library to implement android audio hal with audioframework (https://github.com/hidenorly/audioframework).

Before building, you need to export ANDROID_HOME env.

The following is an example to export.

```
export ANDROID_HOME=~/work/android/r
```

# How to build

## Build with stub

```
$ cd ~/work/audioframework
$ make -j 4 afwshared
$ cd ~/work/AndroidAudioHalHelper
$ make clean; make -j 4
```

## Build for real android hal

```
$ cd $ANDROID_HOME
$ source build/envsetup.sh
$ lunch
$ make

$ cd ~/work/audioframework
$ make -j 4 afwshared

$ cd ~/work/AndroidAudioHalHelper
$ make clean; make -j 4 hal
```

You need to build Android itself in advance since the helper depends on the built library such as libfmq.so

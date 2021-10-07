/*
Copyright (C) 2021 hidenorly

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __ANDROID_HAL_TYPES_HPP__
#define __ANDROID_HAL_TYPES_HPP__

#include <string>
#include <vector>
#include <system/audio.h>
#include "Buffer.hpp"

enum HalResult
{
  OK,
  NOT_INITIALIZED,
  INVALID_ARGUMENTS,
  INVALID_STATE,
  NOT_SUPPORTED
};

typedef uint8_t MacAddress[6];

struct DeviceAddress
{
  uint32_t device;
  union Address {
    MacAddress mac;
    uint8_t ipv4[4];
    struct Alsa {
      int32_t card;
      int32_t device;
    } alsa;
  } address;
  std::string busAddress;
  std::string rSubmixAddress;
  DeviceAddress():device(0){
    address.alsa.card = 0;
    address.alsa.device = 0;
  };
};

struct ParameterValue
{
  std::string key;
  std::string value;
  ParameterValue(std::string key, std::string value):key(key),value(value){};
};

typedef uint32_t AudioHwSync;
typedef int32_t AudioUsage;
typedef uint32_t AudioContentType;
typedef int32_t AudioDrain;
typedef int32_t DualMonoMode;
typedef int32_t AudioSource;
typedef audio_channel_mask_t AudioChannelMask;
typedef audio_format_t AndroidAudioFormat;
typedef audio_devices_t AudioDevice;
typedef int32_t AudioMode;

typedef audio_port AudioPort;
typedef audio_port_config AudioPortConfig;
typedef audio_port_handle_t AudioPortHandle;
typedef int32_t AudioIoHandle;

typedef audio_config AudioConfig;
typedef audio_gain_config AudioGainConfig;

typedef audio_module_handle_t AudioModuleHandle;

typedef audio_patch_handle_t AudioPatchHandle;
typedef int32_t AudioPatchHandle;

typedef audio_microphone_characteristic_t AudioMicrophoneCharacteristic;
typedef audio_microphone_direction_t AudioMicrophoneDirection;

typedef audio_output_flags_t AudioOutputFlags;
typedef audio_input_flags_t AudioInputFlags;


struct PlaybackTrackMetadata
{
  AudioUsage usage;
  AudioContentType contentType;
  float gain;
};

struct SourceMetadata
{
  std::vector<PlaybackTrackMetadata> tracks;
};


/** Metadatas of the sink of a StreamIn. */
struct SinkMetadata {
    std::vector<record_track_metadata> tracks;
};

struct TimeSpec
{
  uint64_t tvSec;   // seconds
  uint64_t tvNSec;  // nanoseconds
  TimeSpec(uint64_t tvSec = 0, uint64_t tvNSec = 0):tvSec(tvSec), tvNSec(tvNSec){};
  TimeSpec(const TimeSpec& timeSpec):tvSec(timeSpec.tvSec), tvNSec(timeSpec.tvNSec){};
};

struct PresentationPosition
{
  uint64_t frames;
  TimeSpec timeStamp;
  PresentationPosition(uint64_t frames = 0, TimeSpec timeStamp = TimeSpec()):frames(frames), timeStamp(timeStamp){};
};


enum TimestretchMode
{
  DEFAULT = 0,
  VOICE = 1,
};

enum TimestretchFallbackMode
{
  MUTE = 1,
  FAIL = 2,
};

struct PlaybackRate
{
  float speed;
  float pitch;
  TimestretchMode timestretchMode;
  TimestretchFallbackMode fallbackMode;
  PlaybackRate():speed(1.0), pitch(1.0), timestretchMode(TimestretchMode::DEFAULT), fallbackMode(TimestretchFallbackMode::MUTE){};
};

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif /* PAGE_SIZE */


enum MessageQueueFlagBits
{
  NOT_EMPTY = 1 << 0,
  NOT_FULL = 1 << 1
};

typedef std::string Uuid;
typedef int32_t AudioSession;
typedef int32_t EffectFlags;

enum EffectFlag
{
  TYPE_SHIFT = 0,
  TYPE_SIZE = 3,
  TYPE_MASK = ((1 << TYPE_SIZE) -1) << TYPE_SHIFT,
  TYPE_INSERT = 0 << TYPE_SHIFT,
  TYPE_AUXILIARY = 1 << TYPE_SHIFT,
  TYPE_REPLACE = 2 << TYPE_SHIFT,
  TYPE_PRE_PROC = 3 << TYPE_SHIFT,
  TYPE_POST_PROC = 4 << TYPE_SHIFT,

  INSERT_SHIFT = TYPE_SHIFT + TYPE_SIZE,
  INSERT_SIZE = 3,
  INSERT_MASK = ((1 << INSERT_SIZE) -1) << INSERT_SHIFT,
  INSERT_ANY = 0 << INSERT_SHIFT,
  INSERT_FIRST = 1 << INSERT_SHIFT,
  INSERT_LAST = 2 << INSERT_SHIFT,
  INSERT_EXCLUSIVE = 3 << INSERT_SHIFT,

  VOLUME_SHIFT = INSERT_SHIFT + INSERT_SIZE,
  VOLUME_SIZE = 3,
  VOLUME_MASK = ((1 << VOLUME_SIZE) -1) << VOLUME_SHIFT,
  VOLUME_CTRL = 1 << VOLUME_SHIFT,
  VOLUME_IND = 2 << VOLUME_SHIFT,
  VOLUME_MONITOR = 3 << VOLUME_SHIFT,
  VOLUME_NONE = 0 << VOLUME_SHIFT,

  DEVICE_SHIFT = VOLUME_SHIFT + VOLUME_SIZE,
  DEVICE_SIZE = 3,
  DEVICE_MASK = ((1 << DEVICE_SIZE) -1) << DEVICE_SHIFT,
  DEVICE_IND = 1 << DEVICE_SHIFT,
  DEVICE_NONE = 0 << DEVICE_SHIFT,

  INPUT_SHIFT = DEVICE_SHIFT + DEVICE_SIZE,
  INPUT_SIZE = 2,
  INPUT_MASK = ((1 << INPUT_SIZE) -1) << INPUT_SHIFT,
  INPUT_DIRECT = 1 << INPUT_SHIFT,
  INPUT_PROVIDER = 2 << INPUT_SHIFT,
  INPUT_BOTH = 3 << INPUT_SHIFT,

  OUTPUT_SHIFT = INPUT_SHIFT + INPUT_SIZE,
  OUTPUT_SIZE = 2,
  OUTPUT_MASK = ((1 << OUTPUT_SIZE) -1) << OUTPUT_SHIFT,
  OUTPUT_DIRECT = 1 << OUTPUT_SHIFT,
  OUTPUT_PROVIDER = 2 << OUTPUT_SHIFT,
  OUTPUT_BOTH = 3 << OUTPUT_SHIFT,

  HW_ACC_SHIFT = OUTPUT_SHIFT + OUTPUT_SIZE,
  HW_ACC_SIZE = 2,
  HW_ACC_MASK = ((1 << HW_ACC_SIZE) -1) << HW_ACC_SHIFT,
  HW_ACC_SIMPLE = 1 << HW_ACC_SHIFT,
  HW_ACC_TUNNEL = 2 << HW_ACC_SHIFT,

  AUDIO_MODE_SHIFT = HW_ACC_SHIFT + HW_ACC_SIZE,
  AUDIO_MODE_SIZE = 2,
  AUDIO_MODE_MASK = ((1 << AUDIO_MODE_SIZE) -1) << AUDIO_MODE_SHIFT,
  AUDIO_MODE_IND = 1 << AUDIO_MODE_SHIFT,
  AUDIO_MODE_NONE = 0 << AUDIO_MODE_SHIFT,

  AUDIO_SOURCE_SHIFT = AUDIO_MODE_SHIFT + AUDIO_MODE_SIZE,
  AUDIO_SOURCE_SIZE = 2,
  AUDIO_SOURCE_MASK = ((1 << AUDIO_SOURCE_SIZE) -1) << AUDIO_SOURCE_SHIFT,
  AUDIO_SOURCE_IND = 1 << AUDIO_SOURCE_SHIFT,
  AUDIO_SOURCE_NONE = 0 << AUDIO_SOURCE_SHIFT,

  OFFLOAD_SHIFT = AUDIO_SOURCE_SHIFT + AUDIO_SOURCE_SIZE,
  OFFLOAD_SIZE = 1,
  OFFLOAD_MASK = ((1 << OFFLOAD_SIZE) -1) << OFFLOAD_SHIFT,
  OFFLOAD_SUPPORTED = 1 << OFFLOAD_SHIFT,

  NO_PROCESS_SHIFT = OFFLOAD_SHIFT + OFFLOAD_SIZE,
  NO_PROCESS_SIZE = 1,
  NO_PROCESS_MASK = ((1 << NO_PROCESS_SIZE) -1) << NO_PROCESS_SHIFT,
  NO_PROCESS = 1 << NO_PROCESS_SHIFT
};


struct EffectDescriptor
{
  Uuid type;
  Uuid uuid;
  EffectFlags flags;
  uint16_t cpuLoad;
  uint16_t memoryUsage;
  uint8_t name[64];
  uint8_t implementor[64];
  EffectDescriptor():type(""), uuid(""), flags(0), cpuLoad(0), memoryUsage(0){
    memset(name, 0, sizeof(name));
    memset(implementor, 0, sizeof(implementor));
  };
};

struct AndroidAudioBuffer {
  uint64_t id;
  uint32_t frameCount;
  ByteBuffer buf;
  AndroidAudioBuffer():id(0), frameCount(0){};
};

enum EffectBufferAccess
{
  ACCESS_WRITE,
  ACCESS_READ,
  ACCESS_ACCUMULATE
};

enum EffectConfigParameter
{
  BUFFER = 0x0001,
  SMP_RATE = 0x0002,
  CHANNELS = 0x0004,
  FORMAT = 0x0008,
  ACC_MODE = 0x0010,
};
typedef int32_t EffectConfigParameters;

struct EffectBufferConfig
{
  AndroidAudioBuffer buffer;
  uint32_t samplingRateHz;
  AudioChannelMask channels;
  AndroidAudioFormat format;
  EffectBufferAccess accessMode;
  EffectConfigParameters mask;

  EffectBufferConfig():buffer(), samplingRateHz(0), channels(AUDIO_CHANNEL_IN_STEREO), format(AUDIO_FORMAT_DEFAULT), accessMode(EffectBufferAccess::ACCESS_ACCUMULATE), mask(EffectConfigParameter::BUFFER | EffectConfigParameter::SMP_RATE | EffectConfigParameter::CHANNELS | EffectConfigParameter::FORMAT | EffectConfigParameter::ACC_MODE){};
};

struct EffectConfig
{
  EffectBufferConfig inputCfg;
  EffectBufferConfig outputCfg;
};

struct EffectAuxChannelsConfig
{
  AudioChannelMask mainChannels;
  AudioChannelMask auxChannels;
  EffectAuxChannelsConfig():mainChannels(AUDIO_CHANNEL_IN_STEREO), auxChannels(AUDIO_CHANNEL_IN_STEREO){};
};

struct EffectOffloadParameter
{
  bool isOffload;
  AudioIoHandle ioHandle;
  EffectOffloadParameter():isOffload(false), ioHandle(0){};
};


#endif /* __ANDROID_HAL_TYPES_HPP__ */

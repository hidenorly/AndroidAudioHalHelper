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
typedef uint32_t AudioChannelMask;
typedef uint32_t AndroidAudioFormat;
typedef uint32_t AudioDevice;
typedef int32_t AudioMode;

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

typedef int32_t AudioPatchHandle;
typedef int32_t AudioPortHandle;
typedef int32_t AudioIoHandle;

enum MessageQueueFlagBits
{
  NOT_EMPTY = 1 << 0,
  NOT_FULL = 1 << 1
};

typedef std::string Uuid;
typedef int32_t AudioSession;
typedef int32_t EffectFlags;

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

  EffectBufferConfig():buffer(), samplingRateHz(0), channels(0), format(0), accessMode(EffectBufferAccess::ACCESS_ACCUMULATE), mask(EffectConfigParameter::BUFFER | EffectConfigParameter::SMP_RATE | EffectConfigParameter::CHANNELS | EffectConfigParameter::FORMAT | EffectConfigParameter::ACC_MODE){};
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
  EffectAuxChannelsConfig():mainChannels(0), auxChannels(0){};
};

struct EffectOffloadParameter
{
  bool isOffload;
  AudioIoHandle ioHandle;
  EffectOffloadParameter():isOffload(false), ioHandle(0){};
};


#endif /* __ANDROID_HAL_TYPES_HPP__ */

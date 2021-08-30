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

#ifndef __ANDROID_HAL_STREAM_HPP__
#define __ANDROID_HAL_STREAM_HPP__

#include "AudioFormat.hpp"
#include <system/audio.h>
#include <stdint.h>
#include "AndroidHalTypes.hpp"


class IStream
{
public:
  virtual uint64_t getFrameSize(void);
  virtual uint64_t getFrameCount(void);

  virtual uint64_t getBufferSize(void);

  virtual uint32_t getSampleRate(void);
  virtual std::vector<uint32_t> getSupportedSampleRates(audio_format_t androidEncoding);
  virtual HalResult setSampleRate(uint32_t sampleRateHz);

  virtual audio_channel_mask_t getChannelMask(void);
  virtual std::vector<audio_channel_mask_t> getSupportedChannelMasks(AudioFormat format);
  virtual HalResult setChannelMask(audio_channel_mask_t mask);

  virtual audio_format_t getFormat(void);
  virtual std::vector<audio_format_t> getSupportedFormats(void);
  virtual HalResult setFormat(audio_format_t format);

  virtual AudioFormat getAudioProperties(void);

  virtual HalResult addEffect(uint64_t effectId);
  virtual HalResult removeEffect(uint64_t effectId);

  virtual HalResult standby(void);

  // getDevices(), setDevices() are not supported on patch panel supported devices
  virtual HalResult getDevices(std::vector<DeviceAddress>& devices);
  virtual HalResult setDevices(std::vector<DeviceAddress> devices);

  virtual HalResult setHwAvSync(AudioHwSync hwAvSync);

  virtual HalResult getParameters(std::vector<std::string> keys, std::vector<ParameterValue>& values);
  virtual HalResult setParameters(std::vector<ParameterValue> values);

  virtual HalResult start(void);
  virtual HalResult stop(void);
  virtual HalResult close(void);
};

#endif /* __ANDROID_HAL_STREAM_HPP__ */

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

#ifndef __ANDROID_HAL_EFFECT_HPP__
#define __ANDROID_HAL_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include <vector>
#include <memory>
#include <stdint.h>
#include <fmq/MessageQueue.h>

class IEffectBufferProviderCallback;

class IEffect
{
protected:
  uint64_t mEffectId;

public:
  IEffect(uint64_t effectId = 0);
  virtual ~IEffect();

  uint64_t getEffectId(void){ return mEffectId; };
  EffectDescriptor getDescriptor(void);

  HalResult init(void);
  HalResult reset(void);

  HalResult enable(void);
  HalResult disable(void);

  HalResult close(void);

  HalResult setDevice(AudioDevice device);  // setOutputDevice()
  HalResult setInputDevice(AudioDevice device);

  HalResult setAudioSource(AudioSource source);

  HalResult setAudioMode(AudioMode mode);

  std::vector<uint32_t> setAndGetVolume(std::vector<uint32_t> channelVolumes);
  HalResult volumeChangeNotification(std::vector<uint32_t> channelVolumes);

  HalResult setConfig(const EffectConfig& config, std::shared_ptr<IEffectBufferProviderCallback> inputBufferProvider, std::shared_ptr<IEffectBufferProviderCallback> outputBufferProvider);
  EffectConfig getConfig(void);
  HalResult setConfigReverse(const EffectConfig& config, std::shared_ptr<IEffectBufferProviderCallback> inputBufferProvider, std::shared_ptr<IEffectBufferProviderCallback> outputBufferProvider);
  EffectConfig getConfigReverse(void);

  std::vector<EffectAuxChannelsConfig> getSupportedAuxChannelsConfigs(uint32_t maxConfigs);
  HalResult setAuxChannelsConfig(EffectAuxChannelsConfig& config);
  EffectAuxChannelsConfig getAuxChannelsConfig(void);

  HalResult offload(EffectOffloadParameter param);

  typedef android::hardware::MessageQueue<HalResult, android::hardware::kSynchronizedReadWrite> StatusMQ;

  std::shared_ptr<StatusMQ> prepareForProcessing(void);
  HalResult setProcessBuffers(std::shared_ptr<AndroidAudioBuffer> inBuffer, std::shared_ptr<AndroidAudioBuffer> outBuffer);

  uint32_t command(uint32_t commandId, const std::vector<uint8_t>& inData, std::vector<uint8_t>& outData, const uint32_t resultMaxSize = UINT_MAX);
  HalResult setParameter(const std::vector<uint8_t>& parameter, const std::vector<uint8_t>& value);
  HalResult getParameter(const std::vector<uint8_t>& parameter, std::vector<uint8_t>& outValue, const uint32_t valueMaxSize = UINT_MAX);

  std::vector<uint8_t> getSupportedConfigsForFeature(uint32_t featureId, uint32_t maxConfigs = UINT_MAX, uint32_t configSize = UINT_MAX);
  HalResult setCurrentConfigForFeature(uint32_t featureId, const std::vector<uint8_t>& configData);
  HalResult getCurrentConfigForFeature(uint32_t featureId, std::vector<uint8_t>& outConfigData, uint32_t configSize = UINT_MAX);
};

#endif /* __ANDROID_HAL_EFFECT_HPP__ */

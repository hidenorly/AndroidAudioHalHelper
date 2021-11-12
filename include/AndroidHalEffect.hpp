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
#include "AndroidHalEffectBufferProvider.hpp"
#include <vector>
#include <memory>
#include <stdint.h>
#include <fmq/MessageQueue.h>

#include "Filter.hpp"
#include "Pipe.hpp"
#include <tuple>
#include "Source.hpp"
#include "Sink.hpp"
#include "FifoBuffer.hpp"

class EffectSource : public ISource
{
protected:
  FifoBuffer mFifoBuffer;
  std::shared_ptr<IEffectBufferProviderCallback> mBufferProvider;

protected:
  void setAudioFormatPrimitive(AudioFormat format);
  virtual void readPrimitive(IAudioBuffer& buf);

public:
  EffectSource();
  virtual ~EffectSource();
  virtual std::string toString(void){return "EffectSource";};
  void enqueueSourceBuffer(std::shared_ptr<AndroidAudioBuffer> pBuffer);
  void resetSourceBuffers(void);
  void setBufferProvider(std::shared_ptr<IEffectBufferProviderCallback> pBufferProvider);
  virtual bool isAvailableFormat(AudioFormat format){ return true; };
};

class EffectSink : public ISink // TODO: InterPipeBridge might be enough?
{
protected:
  FifoBuffer mFifoBuffer;
  std::vector<std::shared_ptr<AndroidAudioBuffer>> mEffectBuffers;
  std::shared_ptr<IEffectBufferProviderCallback> mBufferProvider;

protected:
  void setAudioFormatPrimitive(AudioFormat format);
  virtual void writePrimitive(IAudioBuffer& buf);

public:
  EffectSink();
  virtual ~EffectSink();
  virtual std::string toString(void){return "EffectSink";};
  void enqueSinkBuffer(std::shared_ptr<AndroidAudioBuffer> pBuffer);
  void resetSinkBuffers(void);
  virtual void dump(void);
  virtual AudioFormat getAudioFormat(void);
  virtual bool isAvailableFormat(AudioFormat format){ return true; };
  void setBufferProvider(std::shared_ptr<IEffectBufferProviderCallback> pBufferProvider);
};

class IEffect : public std::enable_shared_from_this<IEffect>
{
public:
  typedef android::hardware::MessageQueue<HalResult, android::hardware::kSynchronizedReadWrite> StatusMQ;
  const int DEFAUT_SIZE_OF_MESSAGE_QUEUE=256;
  typedef std::tuple<std::shared_ptr<AndroidAudioBuffer>, std::shared_ptr<AndroidAudioBuffer>> EffectBuffer;

protected:
  std::string mUuid;
  std::shared_ptr<IFilter> mFilter;
  std::shared_ptr<IPipe> mPipe;
  EffectConfig mEffectConfig;
  EffectAuxChannelsConfig mEffectAuxChannelConfig;
  AudioFormat mInputFormat;
  AudioFormat mOutputFormat;
  std::shared_ptr<IEffectBufferProviderCallback> mInputBufferProvider;
  std::shared_ptr<IEffectBufferProviderCallback> mOutputBufferProvider;
  std::shared_ptr<StatusMQ> mStatusMQ;

  std::map<uint32_t, std::vector<uint8_t>> mFeatureConfigData;

protected:
  std::string getStringFromVector(const std::vector<uint8_t>& value);

public:
  IEffect(std::string uuid = "", std::shared_ptr<IFilter> pFilter = nullptr);
  virtual ~IEffect();

  virtual uint64_t getEffectId(void);
  std::string getUuid(void);
  virtual EffectDescriptor getDescriptor(void);
  std::shared_ptr<IFilter> getFilter(void);
  void associateFilter(std::shared_ptr<IFilter> pFilter){ mFilter = pFilter; };

  static std::shared_ptr<IEffect> getInstanceIfUuidMatch(std::string uuid){ return nullptr; };

  virtual HalResult init(void);
  virtual HalResult reset(void);

  virtual HalResult enable(void);
  virtual HalResult disable(void);

  virtual HalResult close(void);

  virtual HalResult setDevice(AudioDevice device);  // setOutputDevice()
  virtual HalResult setInputDevice(AudioDevice device);

  virtual HalResult setAudioSource(AudioSource source);

  virtual HalResult setAudioMode(AudioMode mode);

  virtual std::vector<uint32_t> setAndGetVolume(const std::vector<uint32_t>& channelVolumes);
  virtual HalResult volumeChangeNotification(const std::vector<uint32_t>& channelVolumes);

  virtual HalResult setConfig(const EffectConfig& config, std::shared_ptr<IEffectBufferProviderCallback> inputBufferProvider, std::shared_ptr<IEffectBufferProviderCallback> outputBufferProvider);
  virtual EffectConfig getConfig(void);
  virtual HalResult setConfigReverse(const EffectConfig& config, std::shared_ptr<IEffectBufferProviderCallback> inputBufferProvider, std::shared_ptr<IEffectBufferProviderCallback> outputBufferProvider);
  virtual EffectConfig getConfigReverse(void);

  virtual std::vector<EffectAuxChannelsConfig> getSupportedAuxChannelsConfigs(uint32_t maxConfigs);
  virtual HalResult setAuxChannelsConfig(const EffectAuxChannelsConfig& config);
  virtual EffectAuxChannelsConfig getAuxChannelsConfig(void);

  virtual HalResult offload(const EffectOffloadParameter& param);

  virtual std::shared_ptr<StatusMQ> prepareForProcessing(void);
  virtual HalResult setProcessBuffers(std::shared_ptr<AndroidAudioBuffer> inBuffer, std::shared_ptr<AndroidAudioBuffer> outBuffer);

  // vendor specific command & parameters
  virtual uint32_t command(uint32_t commandId, const std::vector<uint8_t>& inData, std::vector<uint8_t>& outData, const uint32_t resultMaxSize = UINT_MAX);
  virtual HalResult setParameter(const std::vector<uint8_t>& parameter, const std::vector<uint8_t>& value);
  virtual HalResult getParameter(const std::vector<uint8_t>& parameter, std::vector<uint8_t>& outValue, const uint32_t valueMaxSize = UINT_MAX);
  virtual std::vector<uint8_t> getSupportedConfigsForFeature(uint32_t featureId, uint32_t maxConfigs = UINT_MAX, const uint32_t configSize = UINT_MAX);
  virtual HalResult setCurrentConfigForFeature(uint32_t featureId, const std::vector<uint8_t>& configData);
  virtual HalResult getCurrentConfigForFeature(uint32_t featureId, std::vector<uint8_t>& outConfigData, uint32_t configSize = UINT_MAX);
};

#endif /* __ANDROID_HAL_EFFECT_HPP__ */

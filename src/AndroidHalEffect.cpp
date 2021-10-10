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

#include "AndroidHalEffect.hpp"
#include "AudioFormatHelper.hpp"
#include <iostream>
#include "AndroidHalDevice.hpp"

IEffect::IEffect(std::string uuid, std::shared_ptr<IFilter> pFilter):mUuid(uuid), mFilter(pFilter)
{

}

IEffect::~IEffect()
{

}

uint64_t IEffect::getEffectId(void)
{
  return reinterpret_cast<uint64_t>( mFilter.get() );
}

std::string IEffect::getUuidId(void)
{
  return mUuid;
}

EffectFlags IEffect::getDefaultEffectFlags(void)
{
  EffectFlags result =
    EffectFlag::TYPE_POST_PROC |
    EffectFlag::INSERT_ANY |
    EffectFlag::VOLUME_NONE |
    EffectFlag::DEVICE_IND |
    EffectFlag::INPUT_DIRECT |
    EffectFlag::OUTPUT_DIRECT |
    EffectFlag::HW_ACC_TUNNEL |
    EffectFlag::AUDIO_MODE_IND |
    EffectFlag::AUDIO_SOURCE_IND |
    EffectFlag::OFFLOAD_SUPPORTED;

  return result;
}

EffectDescriptor IEffect::getDescriptor(void)
{
  EffectDescriptor result;
  result.uuid = mUuid;

  if( mFilter ){
    result.flags = getDefaultEffectFlags();
    result.cpuLoad = mFilter->getExpectedProcessingUSec();
    std::shared_ptr<FilterPlugIn> pPlugIn = std::dynamic_pointer_cast<FilterPlugIn>(mFilter);
    if( pPlugIn ){
      strncpy( (char*)result.name, pPlugIn->toString().c_str(), sizeof( result.name ) );
      strncpy( (char*)result.implementor, pPlugIn->toString().c_str(), sizeof( result.implementor ) );
    }
  }

  return result;
}

HalResult IEffect::init(void)
{
  HalResult result = HalResult::OK;

  mEffectConfig = EffectConfig();
  mInputFormat = AudioFormat();
  mOutputFormat = AudioFormat();
  mInputBufferProvider.reset();
  mOutputBufferProvider.reset();


  return result;
}

HalResult IEffect::reset(void)
{
  HalResult result = HalResult::OK;

  return result;
}

HalResult IEffect::enable(void)
{
  HalResult result = HalResult::OK;

  if( mPipe ){
    mPipe->run();
  }

  return result;
}

HalResult IEffect::disable(void)
{
  HalResult result = HalResult::OK;

  if( mPipe ){
    mPipe->stop();
  }

  return result;
}

HalResult IEffect::close(void)
{
  HalResult result = HalResult::OK;

  if( mPipe ){
    mPipe->stop();
    mPipe.reset();
  }

  return result;
}

// for output
HalResult IEffect::setDevice(AudioDevice device)
{
  HalResult result = HalResult::OK;

  return result;
}

// for input chain
HalResult IEffect::setInputDevice(AudioDevice device)
{
  HalResult result = HalResult::OK;

  return result;
}

// for input chain
HalResult IEffect::setAudioSource(AudioSource source)
{
  HalResult result = HalResult::OK;

  return result;
}

HalResult IEffect::setAudioMode(AudioMode mode)
{
  HalResult result = HalResult::OK;

  enum AudioModeVal
  {
    NORMAL           = 0,
    RINGTONE         = 1,
    IN_CALL          = 2,
    IN_COMMUNICATION = 3,
    CALL_SCREEN      = 4,
  };

  std::string modeString;
  switch( mode )
  {
    case AudioModeVal::NORMAL:
      modeString = "NORMAL";
      break;
    case AudioModeVal::RINGTONE:
      modeString = "RINGTONE";
      break;
    case AudioModeVal::IN_CALL:
      modeString = "IN_CALL";
      break;
    case AudioModeVal::IN_COMMUNICATION:
      modeString = "IN_COMMUNICATION";
      break;
    case AudioModeVal::CALL_SCREEN:
      modeString = "CALL_SCREEN";
      break;
  }

  std::cout << "setMode( " << modeString << " )" << std::endl;

  return result;
}


std::vector<uint32_t> IEffect::setAndGetVolume(std::vector<uint32_t> channelVolumes)
{
  std::vector<uint32_t> result;

  return result;
}

HalResult IEffect::volumeChangeNotification(std::vector<uint32_t> channelVolumes)
{
  HalResult result = HalResult::OK;

  return result;
}

HalResult IEffect::setConfig(const EffectConfig& config, std::shared_ptr<IEffectBufferProviderCallback> inputBufferProvider, std::shared_ptr<IEffectBufferProviderCallback> outputBufferProvider)
{
  HalResult result = HalResult::OK;

  mEffectConfig = config;
  mInputFormat = AndroidFormatHelper::getAudioFormatFromAndroidEffectConfig( config.inputCfg );
  mOutputFormat = AndroidFormatHelper::getAudioFormatFromAndroidEffectConfig( config.outputCfg );
  mInputBufferProvider = inputBufferProvider;
  mOutputBufferProvider = outputBufferProvider;

  return result;
}

EffectConfig IEffect::getConfig(void)
{
  return mEffectConfig;
}

HalResult IEffect::setConfigReverse(const EffectConfig& config, std::shared_ptr<IEffectBufferProviderCallback> inputBufferProvider, std::shared_ptr<IEffectBufferProviderCallback> outputBufferProvider)
{
  return setConfig(config, inputBufferProvider, outputBufferProvider);
}

EffectConfig IEffect::getConfigReverse(void)
{
  return getConfig();
}

std::vector<EffectAuxChannelsConfig> IEffect::getSupportedAuxChannelsConfigs(uint32_t maxConfigs)
{
  std::vector<EffectAuxChannelsConfig> result;

  return result;
}

HalResult IEffect::setAuxChannelsConfig(EffectAuxChannelsConfig& config)
{
  HalResult result = HalResult::OK;

  mEffectAuxChannelConfig = config;

  return result;
}

EffectAuxChannelsConfig IEffect::getAuxChannelsConfig(void)
{
  return mEffectAuxChannelConfig;
}

HalResult IEffect::offload(EffectOffloadParameter param)
{
  HalResult result = HalResult::OK;
  if( param.isOffload ){
    std::shared_ptr<IStream> pStream = IDevice::getStreamByIoHandle( param.ioHandle );
    if( pStream ){
      pStream->addEffect( getEffectId() );
    }
  }

  return result;
}

std::shared_ptr<IEffect::StatusMQ> IEffect::prepareForProcessing(void)
{
  if( !mStatusMQ ){
    mStatusMQ = std::make_shared<IEffect::StatusMQ>(IEffect::DEFAUT_SIZE_OF_MESSAGE_QUEUE);
  }
  return mStatusMQ;

}

HalResult IEffect::setProcessBuffers(std::shared_ptr<AndroidAudioBuffer> inBuffer, std::shared_ptr<AndroidAudioBuffer> outBuffer)
{
  HalResult result = HalResult::OK;

  return result;
}

uint32_t IEffect::command(uint32_t commandId, const std::vector<uint8_t>& inData, std::vector<uint8_t>& outData, const uint32_t resultMaxSize)
{
  return 0;
}

HalResult IEffect::setParameter(const std::vector<uint8_t>& parameter, const std::vector<uint8_t>& value)
{
  HalResult result = HalResult::OK;

  return result;
}

HalResult IEffect::getParameter(const std::vector<uint8_t>& parameter, std::vector<uint8_t>& outValue, const uint32_t valueMaxSize)
{
  HalResult result = HalResult::OK;

  return result;
}

std::vector<uint8_t> IEffect::getSupportedConfigsForFeature(uint32_t featureId, uint32_t maxConfigs, uint32_t configSize)
{
  std::vector<uint8_t> result;

  return result;
}

HalResult IEffect::setCurrentConfigForFeature(uint32_t featureId, const std::vector<uint8_t>& configData)
{
  HalResult result = HalResult::OK;

  return result;
}

HalResult IEffect::getCurrentConfigForFeature(uint32_t featureId, std::vector<uint8_t>& outConfigData, uint32_t configSize)
{
  HalResult result = HalResult::OK;

  return result;
}

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
#include "AudioEffectHelper.hpp"
#include "SourceSinkManager.hpp"

IEffect::IEffect(std::string uuid, std::shared_ptr<IFilter> pFilter):mUuid(uuid), mFilter(pFilter)
{
  AudioEffectHelper::associateEffect( shared_from_this() );
}

IEffect::~IEffect()
{
  AudioEffectHelper::unassociateEffect( shared_from_this() );
}

uint64_t IEffect::getEffectId(void)
{
  return reinterpret_cast<uint64_t>( mFilter.get() );
}

std::string IEffect::getUuid(void)
{
  return mUuid;
}


EffectDescriptor IEffect::getDescriptor(void)
{
  return AudioEffectHelper::getDescriptor( mUuid, mFilter );
}

std::shared_ptr<IFilter> IEffect::getFilter(void)
{
  return mFilter;
}

HalResult IEffect::init(void)
{
  HalResult result = HalResult::OK;

  mEffectConfig = EffectConfig();
  mInputFormat = AudioFormat();
  mOutputFormat = AudioFormat();
  mInputBufferProvider.reset();
  mOutputBufferProvider.reset();
  mAudioBuffers.clear();
  mFeatureConfigData.clear();

  return result;
}

HalResult IEffect::reset(void)
{
  HalResult result = HalResult::OK;

  mAudioBuffers.clear();

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
  HalResult result = HalResult::INVALID_ARGUMENTS;

  std::shared_ptr<ISink> pSink = SourceSinkManager::getSink( device );
  if( pSink ){
    if( mPipe ){
      mPipe->attachSink( pSink );
      result = HalResult::OK;
    }
  }

  return result;
}

// for input chain
HalResult IEffect::setInputDevice(AudioDevice device)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;

  std::shared_ptr<ISource> pSource = SourceSinkManager::getSource( device );
  if( pSource ){
    if( mPipe ){
      mPipe->attachSource( pSource );
      result = HalResult::OK;
    }
  }

  return result;
}

// for input chain
HalResult IEffect::setAudioSource(AudioSource source)
{
  HalResult result = HalResult::OK;

  std::string sourceDesc;

  switch( source ){
    case AUDIO_SOURCE_MIC:
      sourceDesc = "AUDIO_SOURCE_MIC";
      break;
    case AUDIO_SOURCE_VOICE_UPLINK:
      sourceDesc = "AUDIO_SOURCE_VOICE_UPLINK";
      break;
    case AUDIO_SOURCE_VOICE_DOWNLINK:
      sourceDesc = "AUDIO_SOURCE_VOICE_DOWNLINK";
      break;
    case AUDIO_SOURCE_VOICE_CALL:
      sourceDesc = "AUDIO_SOURCE_VOICE_CALL";
      break;
    case AUDIO_SOURCE_CAMCORDER:
      sourceDesc = "AUDIO_SOURCE_CAMCORDER";
      break;
    case AUDIO_SOURCE_VOICE_RECOGNITION:
      sourceDesc = "AUDIO_SOURCE_VOICE_RECOGNITION";
      break;
    case AUDIO_SOURCE_VOICE_COMMUNICATION:
      sourceDesc = "AUDIO_SOURCE_VOICE_COMMUNICATION";
      break;
    case AUDIO_SOURCE_REMOTE_SUBMIX:
      sourceDesc = "AUDIO_SOURCE_REMOTE_SUBMIX";
      break;
    case AUDIO_SOURCE_UNPROCESSED:
      sourceDesc = "AUDIO_SOURCE_UNPROCESSED";
      break;
    case AUDIO_SOURCE_VOICE_PERFORMANCE:
      sourceDesc = "AUDIO_SOURCE_VOICE_PERFORMANCE";
      break;
    case AUDIO_SOURCE_ECHO_REFERENCE:
      sourceDesc = "AUDIO_SOURCE_ECHO_REFERENCE";
      break;
    case AUDIO_SOURCE_FM_TUNER:
      sourceDesc = "AUDIO_SOURCE_FM_TUNER";
      break;
    case AUDIO_SOURCE_HOTWORD:
      sourceDesc = "AUDIO_SOURCE_HOTWORD";
      break;
    case AUDIO_SOURCE_DEFAULT:
    default:
      sourceDesc = "AUDIO_SOURCE_DEFAULT";
      break;
  }
  std::cout << "setAudioSource( " << sourceDesc << " )" << std::endl;

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


std::vector<uint32_t> IEffect::setAndGetVolume(const std::vector<uint32_t>& channelVolumes)
{
  std::vector<uint32_t> result;

  return result;
}

HalResult IEffect::volumeChangeNotification(const std::vector<uint32_t>& channelVolumes)
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

HalResult IEffect::setAuxChannelsConfig(const EffectAuxChannelsConfig& config)
{
  HalResult result = HalResult::OK;

  mEffectAuxChannelConfig = config;

  return result;
}

EffectAuxChannelsConfig IEffect::getAuxChannelsConfig(void)
{
  return mEffectAuxChannelConfig;
}

HalResult IEffect::offload(const EffectOffloadParameter& param)
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
  mAudioBuffers.push_back( std::make_tuple(inBuffer, outBuffer) );

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

std::vector<uint8_t> IEffect::getSupportedConfigsForFeature(uint32_t featureId, uint32_t maxConfigs, const uint32_t configSize)
{
  std::vector<uint8_t> result;

  return result;
}

HalResult IEffect::setCurrentConfigForFeature(uint32_t featureId, const std::vector<uint8_t>& configData)
{
  HalResult result = HalResult::OK;
  mFeatureConfigData.insert_or_assign( featureId, configData );

  return result;
}

HalResult IEffect::getCurrentConfigForFeature(uint32_t featureId, std::vector<uint8_t>& outConfigData, uint32_t configSize)
{
  HalResult result = HalResult::OK;

  if( mFeatureConfigData.contains( featureId ) ){
    outConfigData = mFeatureConfigData[ featureId ];
    if( ( configSize != UINT_MAX ) && ( outConfigData.size() > configSize ) ){
      outConfigData.resize( configSize );
    }
  }

  return result;
}

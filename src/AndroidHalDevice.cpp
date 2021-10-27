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

#include "AndroidHalDevice.hpp"
#include "AudioPortHelper.hpp"
#include "AudioFormatHelper.hpp"

#include "SourceSinkManager.hpp"
#include "PatchPanel.hpp"
#include "AudioEffectHelper.hpp"
#include "ParameterHelper.hpp"

// TODO: the devices support

IDevice::IDevice(AudioModuleHandle hwModule, std::string filterPlugInPath):mMasterVolume(100.0f), mHwModule(hwModule), mPatchHandleCount(0)
{
  AudioEffectHelper::initialize( filterPlugInPath );
}

IDevice::~IDevice()
{
  AudioEffectHelper::terminate();
}

std::shared_ptr<IStream> IDevice::getStreamByIoHandle(AudioIoHandle ioHandle)
{
  std::shared_ptr<IStream> result;

  if( mStreams.contains( ioHandle) ){
    result = mStreams[ ioHandle ];
  }

  return result;
}


HalResult IDevice::initCheck(void)
{
  return SourceSinkManager::getSourceSinkCount() ? HalResult::OK : HalResult::NOT_INITIALIZED;
}

HalResult IDevice::close(void)
{
  mPatchPanels.clear();
  return HalResult::OK;
}


HalResult IDevice::openOutputStream(
  AudioIoHandle ioHandle,
    DeviceAddress deviceAddr,
    AudioConfig config,
    AudioOutputFlags flags,
    SourceMetadata sourceMetadata,
  std::shared_ptr<IStreamOut>& pOutStream,
    AudioConfig& outSuggestedConfig)
{
  pOutStream = std::make_shared<IStreamOut>( ioHandle, deviceAddr, config, flags, sourceMetadata, shared_from_this(), SourceSinkManager::getSink(deviceAddr) );
  outSuggestedConfig = pOutStream->getSuggestedConfig();

  mStreams.insert_or_assign( ioHandle, pOutStream );

  return HalResult::OK;
}

HalResult IDevice::openInputStream(
  AudioIoHandle ioHandle,
    DeviceAddress deviceAddr,
    AudioConfig config,
    AudioInputFlags flags,
    SinkMetadata sinkMetadata,
  std::shared_ptr<IStreamIn>& pOutInStream,
    AudioConfig& outSuggestedConfig)
{
  pOutInStream = std::make_shared<IStreamIn>( ioHandle, deviceAddr, config, flags, sinkMetadata, shared_from_this(), SourceSinkManager::getSource(deviceAddr) );
  outSuggestedConfig = pOutInStream->getSuggestedConfig();
  return HalResult::OK;
}

void IDevice::onCloseStream(std::shared_ptr<IStream> pStream)
{
  if( pStream ){
    AudioIoHandle ioHandle = pStream->getAudioIoHandle();
    if( mStreams.contains( ioHandle ) ){
      mStreams.erase( ioHandle );
    }
  }
}


HalResult IDevice::getParameters(std::vector<std::string> keys, std::vector<ParameterValue>& values)
{
  return ParameterManagerHelper::getParameters( keys, values );
}

HalResult IDevice::setParameters(std::vector<ParameterValue> values)
{
  return ParameterManagerHelper::setParameters( values );
}


bool IDevice::supportsAudioPatches(void)
{
  return true;
}

std::vector<std::shared_ptr<ISource>> IDevice::getSources(std::vector<AudioPortConfig> sources)
{
  std::vector<std::shared_ptr<ISource>> pSources;

  for(auto& aSourceAudioPort : sources ){
    SourceSinkManager::associateByAudioPortConfig( aSourceAudioPort );
    std::shared_ptr<ISource> pSource = SourceSinkManager::getSource( aSourceAudioPort );
    if( pSource ){
      pSource->setAudioFormat( AndroidFormatHelper::getAudioFormatFromAndroidPortConfig( aSourceAudioPort ) );
      pSources.push_back( pSource );
    }
  }

  return pSources;
}

std::vector<std::shared_ptr<ISink>> IDevice::getSinks(std::vector<AudioPortConfig> sinks)
{
  std::vector<std::shared_ptr<ISink>> pSinks;

  for(auto& aSinkAudioPort : sinks ){
    SourceSinkManager::associateByAudioPortConfig( aSinkAudioPort );
    std::shared_ptr<ISink> pSink = SourceSinkManager::getSink( aSinkAudioPort );
    if( pSink ){
      pSink->setAudioFormat( AndroidFormatHelper::getAudioFormatFromAndroidPortConfig( aSinkAudioPort ) );
      pSinks.push_back( pSink );
    }
  }

  return pSinks;
}


AudioPatchHandle IDevice::createAudioPatch(std::vector<AudioPortConfig> sources, std::vector<AudioPortConfig> sinks)
{
  AudioPatchHandle result = 0;

  std::vector<std::shared_ptr<ISource>> pSources = getSources( sources );
  std::vector<std::shared_ptr<ISink>> pSinks = getSinks( sinks );

  // TODO: check the source, sink are already used in existing patch or not

  if( pSources.size() && pSinks.size() ){
    result = mPatchHandleCount++;
    std::shared_ptr<PatchPanel> pPatchPanel = PatchPanel::createPatch( pSources, pSinks );
    mPatchPanels.insert_or_assign( result, pPatchPanel );
    std::shared_ptr<MixerSplitter> pMixerSplitter = pPatchPanel->getMixerSplitter();
    pMixerSplitter->run();
  }

  return result;
}

AudioPatchHandle IDevice::updateAudioPatch(AudioPatchHandle previousPatch, std::vector<AudioPortConfig> sources, std::vector<AudioPortConfig> sinks)
{
  AudioPatchHandle result = 0;

  std::vector<std::shared_ptr<ISource>> pSources = getSources( sources );
  std::vector<std::shared_ptr<ISink>> pSinks = getSinks( sinks );

  // TODO: check the source, sink are already used in the other existing patch or not

  if( pSources.size() && pSinks.size() ){
    if( mPatchPanels.contains( previousPatch ) && mPatchPanels[previousPatch] ){
      std::shared_ptr<MixerSplitter> pMixerSplitter = mPatchPanels[previousPatch]->getMixerSplitter();
      pMixerSplitter->stop();
      pMixerSplitter.reset();
      mPatchPanels[previousPatch]->updatePatch( pSources, pSinks );
      pMixerSplitter = mPatchPanels[previousPatch]->getMixerSplitter();
      pMixerSplitter->run();
      result = previousPatch;
    }
  }

  return result;
}

HalResult IDevice::releaseAudioPatch(AudioPatchHandle patch)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;

  if( mPatchPanels.contains( patch ) ){
    std::shared_ptr<MixerSplitter> pMixerSplitter = mPatchPanels[patch]->getMixerSplitter();
    pMixerSplitter->stop();
    pMixerSplitter.reset();
    mPatchPanels.erase( patch );
    result = HalResult::OK;
  }
  return result;
}


AudioPort IDevice::getAudioPort(AudioPort port)
{
  AudioPort result = port;

  if( port.type == AUDIO_PORT_TYPE_DEVICE ){
    AndroidAudioPortHelper::getAndroidPortFromSourceSink(&result, SourceSinkManager::getSink( port ), port.ext.device.address, mHwModule, port.ext.device.type );
    SourceSinkManager::associateByAudioPort( result );
  }

  return result;
}

HalResult IDevice::setAudioPortConfig(AudioPortConfig config)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;

  SourceSinkManager::associateByAudioPortConfig( config );

  std::shared_ptr<ISourceSinkCommon> pSourceSink = SourceSinkManager::getSourceSink( config );
  if( pSourceSink ){
    bool bSuccess = pSourceSink->setAudioFormat( AndroidFormatHelper::getAudioFormatFromAndroidPortConfig( config ) );
    AudioGainConfig audioGainConfig = AndroidAudioPortHelper::getAudioGainConfigFromAudioPortConfig( config );
    std::shared_ptr<ISink> pSink = std::dynamic_pointer_cast<ISink>( pSourceSink );
    if( pSink ){
      pSink->setVolume( (float)audioGainConfig.index/100.0f );
    }

    result = bSuccess ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
  }

  return result;
}

std::shared_ptr<IStream> IDevice::getStream(AudioPortHandle device)
{
  std::shared_ptr<IStream> result;

  std::shared_ptr<ISourceSinkCommon> pSourceSink = SourceSinkManager::getSourceSink( device );
  if( pSourceSink ){
    for( auto& [audioIoHandle, pStream] : mStreams ){
      if( pStream ){
        std::shared_ptr<IPipe> pPipe = pStream->getPipe();
        if( pPipe ){
          if( pPipe->getSourceRef() == pSourceSink || pPipe->getSinkRef() == pSourceSink ){
            result = pStream;
            break;
          }
        }
      }
    }
  }

  return result;
}

std::shared_ptr<IPipe> IDevice::getPipe(AudioPortHandle device)
{
  std::shared_ptr<IPipe> result;

  std::shared_ptr<IStream> pStream = getStream( device );
  if( pStream ){
    result = pStream->getPipe();
  }

  return result;
}


HalResult IDevice::addDeviceEffect(AudioPortHandle device, uint64_t effectId)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;

  std::shared_ptr<IPipe> pPipe = getPipe( device );
  if( pPipe ){
    std::shared_ptr<IFilter> pFilter = AudioEffectHelper::getFilterFromEffectInstance( effectId );
    if( pFilter ){
      pPipe->addFilterToTail( pFilter );
      result = HalResult::OK;
    }
  }

  return result;
}

HalResult IDevice::removeDeviceEffect(AudioPortHandle device, uint64_t effectId)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;

  std::shared_ptr<IPipe> pPipe = getPipe( device );
  if( pPipe ){
    std::shared_ptr<IFilter> pFilter = AudioEffectHelper::getFilterFromEffectInstance( effectId );
    if( pFilter ){
      result = pPipe->removeFilter( pFilter ) ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
    }
  }

  return result;
}


HalResult IDevice::setMasterVolume(float volume)
{
  mMasterVolume = volume;
  for( auto& pSink : SourceSinkManager::getSinkDevices() ){
    pSink->setVolume( volume );
  }
  return HalResult::OK;
}

float IDevice::getMasterVolume(void)
{
  return mMasterVolume;
}

HalResult IDevice::setMasterMute(bool mute)
{
  setMasterVolume(0.0f);
  return HalResult::OK;
}

bool IDevice::getMasterMute(void)
{
  return ( mMasterVolume == 0.0f );
}

HalResult IDevice::setMicMute(bool mute)
{
  std::vector<std::shared_ptr<ISource>> pSources = SourceSinkManager::getSourceDevices();

  for( auto& pSource : pSources ){
    pSource->setMuteEnabled( mute );
  }

  return HalResult::NOT_SUPPORTED;
}

bool IDevice::getMicMute(void)
{
  bool result = true;

  std::vector<std::shared_ptr<ISource>> pSources = SourceSinkManager::getSourceDevices();

  for( auto& pSource : pSources ){
    result &= pSource->getMuteEnabled();
  }

  return pSources.empty() ? false : result;
}

std::vector<AudioMicrophoneCharacteristic> IDevice::getMicrophones(void)
{
  std::vector<AudioMicrophoneCharacteristic> result;

  std::vector<std::shared_ptr<ISource>> pSources = SourceSinkManager::getSourceDevices();

  int index = 0;
  for( auto& pSource : pSources ){
    AudioMicrophoneCharacteristic characteristic;
    memset(&characteristic, 0, sizeof(AudioMicrophoneCharacteristic) );

    strncpy(characteristic.device_id, pSource->toString().c_str(), sizeof(characteristic.device_id) );
    strncpy(characteristic.address, AndroidDeviceAddressHelper::getStringFromDeviceAddr( SourceSinkManager::getDeviceAddress( pSource ) ).c_str(), sizeof( characteristic.address ) );
    characteristic.id                 = SourceSinkManager::getAudioPortHandle( pSource );
    characteristic.device             = SourceSinkManager::getAudioDevice( pSource );
    characteristic.index_in_the_group = index++;

    result.push_back( characteristic );
  }

  return result;
}

uint64_t IDevice::getInputBufferSize(AudioConfig config)
{
  AudioFormat format = AndroidFormatHelper::getAudioFormatFromAndroidAudioConfig( config );

  return format.getChannelsSampleByte() * config.frame_count;
}

HalResult IDevice::setConnectedState(DeviceAddress address, bool connected)
{
  AndroidDeviceAddressHelper::setDeviceConnected( address, connected );
  return HalResult::OK;
}

AudioHwSync IDevice::getHwAvSync(void)
{
  return 0;
}

HalResult IDevice::setScreenState(bool turnedOn)
{
  return HalResult::NOT_SUPPORTED;
}


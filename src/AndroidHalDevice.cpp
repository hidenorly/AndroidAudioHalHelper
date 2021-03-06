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
#include "GainHelper.hpp"
#include <functional>
// TODO: the devices support

IDevice::IDevice(AudioModuleHandle hwModule, std::string filterPlugInPath):mMasterVolume(100.0f), mHwModule(hwModule), mPatchHandleCount(0), mScreenStateOn(false)
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
    } else {
      // TODO: fill in gap why the pSource is not found
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
    } else {
      // TODO: fill in gap why the pSink is not found
    }
  }

  return pSinks;
}

std::vector<std::shared_ptr<ISource>> IDevice::getUsedSourcesInPatchPanels(std::vector<std::shared_ptr<ISource>> pSources)
{
  std::vector<std::shared_ptr<ISource>> result;

  for( auto& pSource : pSources ){
    for( auto& [ aPatchPanel, sources ] : mPatchPanelSources ){
      for( auto& aSource : sources ){
        if( aSource == pSource ){
          result.push_back( pSource );
        }
      }
    }
  }

  return result;
}


AudioPatchHandle IDevice::createAudioPatch(std::vector<AudioPortConfig> sources, std::vector<AudioPortConfig> sinks)
{
  AudioPatchHandle result = 0;

  std::vector<std::shared_ptr<ISource>> pSources = getSources( sources );
  std::vector<std::shared_ptr<ISink>> pSinks = getSinks( sinks );

  for( auto& aSource : sources ){ setAudioPortConfig( aSource ); };
  for( auto& aSink : sinks ){ setAudioPortConfig( aSink ); };

  // TODO: check the source, sink are already used in existing patch or not
  std::vector<std::shared_ptr<ISource>> usedSources = getUsedSourcesInPatchPanels( pSources );
  assert( usedSources.size() == 0 );

  if( pSources.size() && pSinks.size() ){
    result = mPatchHandleCount++;
    std::shared_ptr<PatchPanel> pPatchPanel = PatchPanel::createPatch( pSources, pSinks );
    mPatchPanels.insert_or_assign( result, pPatchPanel );
    mPatchPanelSources.insert_or_assign( pPatchPanel, pSources );
    mPatchPanelSinks.insert_or_assign( pPatchPanel, pSinks );
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

  for( auto& aSource : sources ){ setAudioPortConfig( aSource ); };
  for( auto& aSink : sinks ){ setAudioPortConfig( aSink ); };

  // TODO: check the source, sink are already used in existing patch or not
  std::vector<std::shared_ptr<ISource>> usedSources = getUsedSourcesInPatchPanels( pSources );
  assert( usedSources.size() == 0 );

  if( pSources.size() && pSinks.size() ){
    if( mPatchPanels.contains( previousPatch ) && mPatchPanels[previousPatch] ){
      std::shared_ptr<PatchPanel> pPatchPanel = mPatchPanels[previousPatch];
      std::shared_ptr<MixerSplitter> pMixerSplitter = pPatchPanel->getMixerSplitter();
      pMixerSplitter->stop();
      pMixerSplitter.reset();
      pPatchPanel->updatePatch( pSources, pSinks );
      mPatchPanelSources.insert_or_assign( pPatchPanel, pSources );
      mPatchPanelSinks.insert_or_assign( pPatchPanel, pSinks );
      pMixerSplitter = pPatchPanel->getMixerSplitter();
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
    std::shared_ptr<PatchPanel> pPatchPanel = mPatchPanels[patch];
    std::shared_ptr<MixerSplitter> pMixerSplitter = pPatchPanel->getMixerSplitter();
    pMixerSplitter->stop();
    pMixerSplitter.reset();
    mPatchPanelSources.erase( pPatchPanel );
    mPatchPanelSinks.erase( pPatchPanel );
    pPatchPanel.reset();
    mPatchPanels.erase( patch );
    result = HalResult::OK;
  }
  return result;
}


AudioPort IDevice::getAudioPort(AudioPort port)
{
  AudioPort result = port;

  AndroidAudioPortHelper::getAndroidPortFromSourceSink( &result, SourceSinkManager::getSink( port ), port.id, port.ext.device.address, mHwModule, port.ext.device.type );
  SourceSinkManager::associateByAudioPort( result );

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
      std::vector<float> volumePercents;
      // TODO: Check the order is same as Android's expectation or not
      for(auto& aGainDb : audioGainConfig.values ){
        volumePercents.push_back( GainHelper::getVolumeRatioPercentageFromDb( aGainDb ) );
      }
      pSink->setVolume( volumePercents );
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
  for( auto& pSink : SourceSinkManager::getSinkDevices() ){
    pSink->setMuteEnabled( mute );
  }
  return HalResult::OK;
}

bool IDevice::getMasterMute(void)
{
  bool result = true;

  std::vector<std::shared_ptr<ISink>> pSinks = SourceSinkManager::getSinkDevices();

  for( auto& pSink : pSinks ){
    result &= pSink->getMuteEnabled();
  }

  return pSinks.empty() ? false : result;
}

HalResult IDevice::setMicMute(bool mute)
{
  std::vector<std::shared_ptr<ISource>> pSources = SourceSinkManager::getSourceDevices();

  for( auto& pSource : pSources ){
    pSource->setMuteEnabled( mute );
  }

  return HalResult::OK;
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
    memset( &characteristic, 0, sizeof(AudioMicrophoneCharacteristic) );

    strncpy( characteristic.device_id, pSource->toString().c_str(), sizeof(characteristic.device_id) );
    strncpy( characteristic.address, AndroidDeviceAddressHelper::getStringFromDeviceAddr( SourceSinkManager::getDeviceAddress( pSource ) ).c_str(), sizeof( characteristic.address ) );
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
  int64_t ptr = reinterpret_cast<int64_t>( shared_from_this().get() );
  return (AudioHwSync)std::hash<int64_t>()(ptr);
}

HalResult IDevice::setScreenState(bool turnedOn)
{
  mScreenStateOn = turnedOn;
  return HalResult::OK;
}


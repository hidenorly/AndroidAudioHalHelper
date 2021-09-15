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

#include "SourceSinkManager.hpp"
#include "PatchPanel.hpp"
#include "AudioEffectHelper.hpp"
#include "ParameterHelper.hpp"

// TODO: AudioEffect
// TODO: Input stream and the devices support

IDevice::IDevice(audio_module_handle_t hwModule, std::string filterPlugInPath):mMasterVolume(100.0f), mHwModule(hwModule), mPatchHandleCount(0)
{
  AudioEffectHelper::initialize( filterPlugInPath );
}

IDevice::~IDevice()
{
  AudioEffectHelper::terminate();
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
    audio_config config,
    audio_output_flags_t flags,
    SourceMetadata sourceMetadata,
  std::shared_ptr<IStreamOut>& pOutStream,
    audio_config& outSuggestedConfig)
{
  pOutStream = std::make_shared<IStreamOut>( ioHandle, deviceAddr, config, flags, sourceMetadata, shared_from_this(), SourceSinkManager::getSink(deviceAddr) );
  outSuggestedConfig = pOutStream->getSuggestedConfig();

  mStreams.insert_or_assign( ioHandle, pOutStream );

  return HalResult::OK;
}

HalResult IDevice::openInputStream(
  AudioIoHandle ioHandle,
    DeviceAddress deviceAddr,
    audio_config config,
    audio_input_flags_t flags,
    SinkMetadata sinkMetadata,
  std::shared_ptr<IStreamIn>& pOutInStream,
    audio_config& outSuggestedConfig)
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

std::vector<std::shared_ptr<ISource>> IDevice::getSources(std::vector<audio_port_config> sources)
{
  std::vector<std::shared_ptr<ISource>> pSources;

  for(auto& aSourceAudioPort : sources ){
    SourceSinkManager::associateByAudioPortConfig( aSourceAudioPort );
    std::shared_ptr<ISource> pSource = SourceSinkManager::getSource( aSourceAudioPort );
    if( pSource ){
      pSources.push_back( pSource );
    }
  }

  return pSources;
}

std::vector<std::shared_ptr<ISink>> IDevice::getSinks(std::vector<audio_port_config> sinks)
{
  std::vector<std::shared_ptr<ISink>> pSinks;

  for(auto& aSinkAudioPort : sinks ){
    SourceSinkManager::associateByAudioPortConfig( aSinkAudioPort );
    std::shared_ptr<ISink> pSink = SourceSinkManager::getSink( aSinkAudioPort );
    if( pSink ){
      pSinks.push_back( pSink );
    }
  }

  return pSinks;
}


audio_patch_handle_t IDevice::createAudioPatch(std::vector<audio_port_config> sources, std::vector<audio_port_config> sinks)
{
  audio_patch_handle_t result = 0;

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

audio_patch_handle_t IDevice::updateAudioPatch(audio_patch_handle_t previousPatch, std::vector<audio_port_config> sources, std::vector<audio_port_config> sinks)
{
  audio_patch_handle_t result = 0;

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

HalResult IDevice::releaseAudioPatch(audio_patch_handle_t patch)
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


audio_port IDevice::getAudioPort(audio_port port)
{
  audio_port result = port;

  if( port.type == AUDIO_PORT_TYPE_DEVICE ){
    // TODO: resolve port to ISourceSinkCommon
    AndroidAudioPortHelper::getAndroidPortFromSourceSink(&result, SourceSinkManager::getSink( AndroidDeviceAddressHelper::getDeviceAddrFromString( port.ext.device.address )), port.ext.device.address, mHwModule, port.ext.device.type );
    SourceSinkManager::associateByAudioPort( result );
  }

  return result;
}

HalResult IDevice::setAudioPortConfig(audio_port_config config)
{
  SourceSinkManager::associateByAudioPortConfig( config );

  // TODO: set config

  return HalResult::NOT_SUPPORTED;
}


HalResult IDevice::addDeviceEffect(audio_port_handle_t device, uint64_t effectId)
{
  // TODO : get the sink's pipe from patch's MixerSplitter
  // TODO : attach the filter to the pipe
  return HalResult::NOT_SUPPORTED;
}

HalResult IDevice::removeDeviceEffect(audio_port_handle_t device, uint64_t effectId)
{
  // TODO : get the sink's pipe from patch's MixerSplitter
  // TODO : deattach the filter from the pipe
  return HalResult::NOT_SUPPORTED;
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
  return HalResult::NOT_SUPPORTED;
}

bool IDevice::getMicMute(void)
{
  return false;
}

std::vector<audio_microphone_characteristic_t> IDevice::getMicrophones(void)
{
  return std::vector<audio_microphone_characteristic_t>();
}

uint64_t IDevice::getInputBufferSize(audio_config config)
{
  return 0;
}

HalResult IDevice::setConnectedState(DeviceAddress address, bool connected)
{
  return HalResult::NOT_SUPPORTED;
}

AudioHwSync IDevice::getHwAvSync(void)
{
  return 0;
}

HalResult IDevice::setScreenState(bool turnedOn)
{
  return HalResult::NOT_SUPPORTED;
}


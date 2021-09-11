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
#include "DeviceAddressHelper.hpp"

#include "PatchPanel.hpp"
#include "PipedSink.hpp"
#include "PipedSource.hpp"

void IDevice::attachSink(DeviceAddress deviceAddr, std::shared_ptr<ISink> pSink)
{
  std::shared_ptr<ISourceSinkCommon> pDevice = pSink;
  if( !std::dynamic_pointer_cast<PipedSink>(pSink) ){
    pDevice = std::make_shared<PipedSink>( pSink );
  }
  if( pDevice ){
    mSourceSinks.insert_or_assign( AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr), pDevice );
  }
}

void IDevice::attachSource(DeviceAddress deviceAddr, std::shared_ptr<ISource> pSource)
{
  std::shared_ptr<ISourceSinkCommon> pDevice = pSource;
  if( !std::dynamic_pointer_cast<PipedSource>(pSource) ){
    std::shared_ptr<PipedSource> pPipedSource = std::make_shared<PipedSource>();
    pPipedSource->attachSource( pSource );
    pDevice = pPipedSource;
  }
  if( pDevice ){
    mSourceSinks.insert_or_assign( AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr), pDevice );
  }
}

void IDevice::detachSourceSinkByDeviceAddr(DeviceAddress deviceAddr)
{
  std::string deviceAddrStr = AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr);
  if( mSourceSinks.contains( deviceAddrStr ) ){
    mSourceSinks.erase( deviceAddrStr );
  }
}

void IDevice::detachSourceSink(std::shared_ptr<ISourceSinkCommon> pSourceSink)
{
  std::string targetAddr;
  for( auto& [deviceAddr, pDevice] : mSourceSinks ){
    if( pDevice == pSourceSink ){
      targetAddr = deviceAddr;
      break;
    }
  }
  if( mSourceSinks.contains( targetAddr ) ){
    mSourceSinks.erase( targetAddr );
  }
}

HalResult IDevice::initCheck(void)
{
  return mSourceSinks.size() ? HalResult::OK : HalResult::NOT_INITIALIZED;
}

HalResult IDevice::close(void)
{
  mPatchPanels.clear();
  mSourceSinks.clear();
  return HalResult::OK;
}

std::shared_ptr<ISink> IDevice::getSinkFromDevice(DeviceAddress deviceAddr)
{
  std::shared_ptr<ISink> result;

  std::string deviceAddrStr = AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr);

  if( mSourceSinks.contains(deviceAddrStr) ){
    result = std::dynamic_pointer_cast<ISink>( mSourceSinks[deviceAddrStr] );
  }

  return result;
}


HalResult IDevice::openOutputStream(AudioIoHandle ioHandle, DeviceAddress deviceAddr, audio_config config, audio_output_flags_t flags, SourceMetadata sourceMetadata, std::shared_ptr<IStreamOut>& pOutStream, audio_config& outSuggestedConfig)
{
  pOutStream = std::make_shared<IStreamOut>( ioHandle, deviceAddr, config, flags, sourceMetadata, shared_from_this(), getSinkFromDevice(deviceAddr) );
  outSuggestedConfig = pOutStream->getSuggestedConfig();

  mStreams.insert_or_assign( ioHandle, pOutStream );

  return HalResult::OK;
}

HalResult IDevice::openInputStream(AudioIoHandle ioHandle, DeviceAddress deviceAddr, audio_config config, audio_input_flags_t flags, SinkMetadata sinkMetadata, std::shared_ptr<IStreamIn>& pOutInStream, audio_config& outSuggestedConfig)
{
  return HalResult::NOT_SUPPORTED;
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


std::vector<ParameterValue> IDevice::getParameters(std::vector<std::string> keys)
{
  return std::vector<ParameterValue>();
}

HalResult IDevice::setParameters(std::vector<ParameterValue> parameters)
{
  return HalResult::NOT_SUPPORTED;
}


bool IDevice::supportsAudioPatches(void)
{
  return false;
}

audio_patch_handle_t IDevice::createAudioPatch(std::vector<audio_port_config> sources, std::vector<audio_port_config> sinks)
{
  return 0;
}

audio_patch_handle_t IDevice::updateAudioPatch(audio_patch_handle_t previousPatch, std::vector<audio_port_config> sources, std::vector<audio_port_config> sinks)
{
  return 0;
}

HalResult IDevice::releaseAudioPatch(audio_patch_handle_t patch)
{
  return HalResult::NOT_SUPPORTED;
}


audio_port IDevice::getAudioPort(audio_port port)
{
  audio_port result = port;
  if( port.type == AUDIO_PORT_TYPE_DEVICE ){
    // TODO: resolve port to ISourceSinkCommon
    AndroidAudioPortHelper::getAndroidPortFromSourceSink(&result, getSinkFromDevice( AndroidDeviceAddressHelper::getDeviceAddrFromString( port.ext.device.address )), port.ext.device.address, mHwModule, port.ext.device.type );
  }

  return result;
}

HalResult IDevice::setAudioPortConfig(audio_port_config config)
{
  return HalResult::NOT_SUPPORTED;
}


HalResult IDevice::addDeviceEffect(audio_port_handle_t device, uint64_t effectId)
{

  return HalResult::NOT_SUPPORTED;
}

HalResult IDevice::removeDeviceEffect(audio_port_handle_t device, uint64_t effectId)
{
  return HalResult::NOT_SUPPORTED;
}


HalResult IDevice::setMasterVolume(float volume)
{
  mMasterVolume = volume;
  for(auto& [ deviceAddr, pDevice] : mSourceSinks ){
    std::shared_ptr<ISink> pSink = std::dynamic_pointer_cast<ISink>( pDevice );
    if( pSink ){
      pSink->setVolume( volume );
    }
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


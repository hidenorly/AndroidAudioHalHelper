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
#include "PatchPanel.hpp"
#include "AudioPortHelper.hpp"

IDevice::IDevice(std::shared_ptr<ISink> pSink)
{
  if( std::dynamic_pointer_cast<PipedSink>(pSink) ){
    mSink = std::dynamic_pointer_cast<PipedSink>(pSink);
  } else {
    mSink = std::make_shared<PipedSink>(pSink);
  }
}

IDevice::~IDevice()
{
}

HalResult IDevice::initCheck(void)
{
  return mSink ? HalResult::OK : HalResult::NOT_INITIALIZED;
}

HalResult IDevice::close(void)
{
  mPatchPanels.clear();
  mSink.reset();
  return HalResult::OK;
}


HalResult IDevice::openOutputStream(AudioIoHandle ioHandle, DeviceAddress device, audio_config config, audio_output_flags_t flags, SourceMetadata sourceMetadata, std::shared_ptr<IStreamOut>& pOutStream, audio_config& outSuggestedConfig)
{
  pOutStream = std::make_shared<IStreamOut>(mSink, ioHandle, device, config, flags, sourceMetadata);
  outSuggestedConfig = pOutStream->getSuggestedConfig();

  mStreams.push_back( pOutStream );

  return HalResult::OK;
}

HalResult IDevice::openInputStream(AudioIoHandle ioHandle, DeviceAddress device, audio_config config, audio_input_flags_t flags, SinkMetadata sinkMetadata, std::shared_ptr<IStreamIn>& pOutInStream, audio_config& outSuggestedConfig)
{
  return HalResult::NOT_SUPPORTED;
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
  audio_port result;
  uint32_t id = (uint32_t)((uint64_t)(this) & 0xFFFFFFFF);
  AndroidAudioPortHelper::getAndroidPortFromSourceSink(&result, mSink, "1", id );

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
  if( mSink ){
    mSink->setVolume(volume);
  }
  return HalResult::OK;
}

float IDevice::getMasterVolume(void)
{
  float result = 100.0f;
  if( mSink ){
    result = mSink->getVolume();
  }
  return result;
}

HalResult IDevice::setMasterMute(bool mute)
{
  if( mSink ){
    mSink->setVolume(0);
  }
  return HalResult::OK;
}

bool IDevice::getMasterMute(void)
{
  bool result = false;
  if( mSink ){
    result = ( mSink->getVolume() == 0.0f ) ? true : false;
  }
  return result;
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


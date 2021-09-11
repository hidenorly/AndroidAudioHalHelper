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

#ifndef __ANDROID_HAL_DEVICE_HPP__
#define __ANDROID_HAL_DEVICE_HPP__

#include "AudioFormat.hpp"
#include <system/audio.h>
#include <stdint.h>
#include "AndroidHalTypes.hpp"

#include "AndroidHalStreamOut.hpp"
#include "AndroidHalStreamIn.hpp"
#include "PatchPanel.hpp"
#include "PipedSink.hpp"
#include <vector>
#include <memory>
#include <map>

class IDevice : public IStream::StreamSessionHandler, public std::enable_shared_from_this<IDevice>
{
protected:
  std::map<AudioIoHandle, std::shared_ptr<IStream>> mStreams;
  std::shared_ptr<PipedSink> mSink;
  std::vector<std::shared_ptr<PatchPanel>> mPatchPanels;

public:
  IDevice(std::shared_ptr<ISink> pSink = nullptr);
  virtual ~IDevice();

  virtual HalResult initCheck(void);
  virtual HalResult close(void);

  virtual HalResult openOutputStream(AudioIoHandle ioHandle, DeviceAddress device, audio_config config, audio_output_flags_t flags, SourceMetadata sourceMetadata, std::shared_ptr<IStreamOut>& pOutStream, audio_config& outSuggestedConfig);
  virtual HalResult openInputStream(AudioIoHandle ioHandle, DeviceAddress device, audio_config config, audio_input_flags_t flags, SinkMetadata sinkMetadata, std::shared_ptr<IStreamIn>& pOutInStream, audio_config& outSuggestedConfig);
  virtual void onCloseStream(std::shared_ptr<IStream> pStream);

  virtual std::vector<ParameterValue> getParameters(std::vector<std::string> keys);
  virtual HalResult setParameters(std::vector<ParameterValue> parameters);

  virtual bool supportsAudioPatches(void);
  virtual audio_patch_handle_t createAudioPatch(std::vector<audio_port_config> sources, std::vector<audio_port_config> sinks);
  virtual audio_patch_handle_t updateAudioPatch(audio_patch_handle_t previousPatch, std::vector<audio_port_config> sources, std::vector<audio_port_config> sinks);
  virtual HalResult releaseAudioPatch(audio_patch_handle_t patch);

  virtual audio_port getAudioPort(audio_port port);
  virtual HalResult setAudioPortConfig(audio_port_config config);

  virtual HalResult addDeviceEffect(audio_port_handle_t device, uint64_t effectId);
  virtual HalResult removeDeviceEffect(audio_port_handle_t device, uint64_t effectId);

  virtual HalResult setMasterVolume(float volume);
  virtual float getMasterVolume(void);
  virtual HalResult setMasterMute(bool mute);
  virtual bool getMasterMute(void);

  virtual HalResult setMicMute(bool mute);
  virtual bool getMicMute(void);
  virtual std::vector<audio_microphone_characteristic_t> getMicrophones(void);

  virtual uint64_t getInputBufferSize(audio_config config);

  virtual HalResult setConnectedState(DeviceAddress address, bool connected);

  virtual AudioHwSync getHwAvSync(void);

  virtual HalResult setScreenState(bool turnedOn);
};

#endif /* __ANDROID_HAL_DEVICE_HPP__ */

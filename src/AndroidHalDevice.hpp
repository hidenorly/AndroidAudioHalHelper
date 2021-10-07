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
#include "Sink.hpp"
#include "Source.hpp"
#include <vector>
#include <memory>
#include <map>

class IDevice : public IStream::StreamSessionHandler, public std::enable_shared_from_this<IDevice>
{
protected:
  static inline std::map<AudioIoHandle, std::shared_ptr<IStream>> mStreams;
  std::map<AudioPatchHandle, std::shared_ptr<PatchPanel>> mPatchPanels;
  float mMasterVolume;
  AudioModuleHandle mHwModule;
  AudioPatchHandle mPatchHandleCount;

protected:
  std::vector<std::shared_ptr<ISource>> getSources(std::vector<AudioPortConfig> sources);
  std::vector<std::shared_ptr<ISink>> getSinks(std::vector<AudioPortConfig> sinks);
  std::shared_ptr<IStream> getStream(AudioPortHandle device);
  std::shared_ptr<IPipe> getPipe(AudioPortHandle device);

public:
  IDevice(AudioModuleHandle hwModule, std::string filterPlugInPath=".");
  virtual ~IDevice();

  virtual HalResult initCheck(void);
  virtual HalResult close(void);

  virtual HalResult openOutputStream(AudioIoHandle ioHandle, DeviceAddress deviceAddr, AudioConfig config, AudioOutputFlags flags, SourceMetadata sourceMetadata, std::shared_ptr<IStreamOut>& pOutStream, AudioConfig& outSuggestedConfig);
  virtual HalResult openInputStream(AudioIoHandle ioHandle, DeviceAddress deviceAddr, AudioConfig config, AudioInputFlags flags, SinkMetadata sinkMetadata, std::shared_ptr<IStreamIn>& pOutInStream, AudioConfig& outSuggestedConfig);
  virtual void onCloseStream(std::shared_ptr<IStream> pStream);

  virtual HalResult getParameters(std::vector<std::string> keys, std::vector<ParameterValue>& values);
  virtual HalResult setParameters(std::vector<ParameterValue> values);

  virtual bool supportsAudioPatches(void);
  virtual AudioPatchHandle createAudioPatch(std::vector<AudioPortConfig> sources, std::vector<AudioPortConfig> sinks);
  virtual AudioPatchHandle updateAudioPatch(AudioPatchHandle previousPatch, std::vector<AudioPortConfig> sources, std::vector<AudioPortConfig> sinks);
  virtual HalResult releaseAudioPatch(AudioPatchHandle patch);

  virtual AudioPort getAudioPort(AudioPort port);
  virtual HalResult setAudioPortConfig(AudioPortConfig config);

  virtual HalResult addDeviceEffect(AudioPortHandle device, uint64_t effectId);
  virtual HalResult removeDeviceEffect(AudioPortHandle device, uint64_t effectId);

  virtual HalResult setMasterVolume(float volume);
  virtual float getMasterVolume(void);
  virtual HalResult setMasterMute(bool mute);
  virtual bool getMasterMute(void);

  virtual HalResult setMicMute(bool mute);
  virtual bool getMicMute(void);
  virtual std::vector<AudioMicrophoneCharacteristic> getMicrophones(void);

  virtual uint64_t getInputBufferSize(AudioConfig config);

  virtual HalResult setConnectedState(DeviceAddress address, bool connected);

  virtual AudioHwSync getHwAvSync(void);

  virtual HalResult setScreenState(bool turnedOn);

  static std::shared_ptr<IStream> getStreamByIoHandle(AudioIoHandle ioHandle);
};

#endif /* __ANDROID_HAL_DEVICE_HPP__ */

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

#ifndef __SOURCE_SINK_MANAGER_HPP__
#define __SOURCE_SINK_MANAGER_HPP__

#include "AndroidHalTypes.hpp"
#include <map>
#include "Sink.hpp"
#include "Source.hpp"
#include "DeviceAddressHelper.hpp"

class SourceSinkManager
{
protected:
  static inline std::map<std::string, std::shared_ptr<ISourceSinkCommon>> mSourceSinksByDeviceAddr;
  static inline std::map<AudioDevice, std::shared_ptr<ISourceSinkCommon>> mSourceSinksByAudioDevice;
  static inline std::map<AudioPortHandle, std::shared_ptr<ISourceSinkCommon>> mSourceSinksByAudioPortHandle;

public:
  static void initialize(void);
  static void terminate(void);

  static int getSourceSinkCount();

  static void attachSink(DeviceAddress deviceAddr, std::shared_ptr<ISink> pSink);
  static void attachSource(DeviceAddress deviceAddr, std::shared_ptr<ISource> pSource);
  static void detachSourceSinkByDeviceAddr(DeviceAddress deviceAddr);
  static void detachSourceSink(std::shared_ptr<ISourceSinkCommon> pSourceSink);

  static std::shared_ptr<ISourceSinkCommon> getSourceSink(DeviceAddress deviceAddr);
  static std::shared_ptr<ISink> getSink(DeviceAddress deviceAddr);
  static std::shared_ptr<ISource> getSource(DeviceAddress deviceAddr);

  static std::shared_ptr<ISourceSinkCommon> getSourceSink(AudioDevice device);
  static std::shared_ptr<ISink> getSink(AudioDevice device);
  static std::shared_ptr<ISource> getSource(AudioDevice device);

  static AudioDevice getAudioDevice(std::shared_ptr<ISourceSinkCommon> pSourceSink);
  static DeviceAddress getDeviceAddress(std::shared_ptr<ISourceSinkCommon> pSourceSink);
  static AudioPortConfig getAudioPortConfig(std::shared_ptr<ISourceSinkCommon> pSourceSink);

  static std::vector<std::shared_ptr<ISink>> getSinkDevices(void);
  static std::vector<std::shared_ptr<ISource>> getSourceDevices(void);

  static AudioPortHandle getAudioPortHandle(const AudioPortConfig& audioPortConfig);
  static AudioPortHandle getAudioPortHandle(const AudioPort& audioPort);
  static AudioPortHandle getAudioPortHandle(std::shared_ptr<ISourceSinkCommon> pSourceSink);

  static void associateByAudioPortConfig(const AudioPortConfig& audioPortConfig);
  static void associateByAudioPort(const AudioPort& audioPort);

  static std::shared_ptr<ISourceSinkCommon> getSourceSink(AudioPortHandle audioPortHandle);
  static std::shared_ptr<ISink> getSink(AudioPortHandle audioPortHandle);
  static std::shared_ptr<ISource> getSource(AudioPortHandle audioPortHandle);

  static std::shared_ptr<ISourceSinkCommon> getSourceSink(const AudioPortConfig& audioPortConfig);
  static std::shared_ptr<ISink> getSink(const AudioPortConfig& audioPortConfig);
  static std::shared_ptr<ISource> getSource(const AudioPortConfig& audioPortConfig);

  static std::shared_ptr<ISourceSinkCommon> getSourceSink(const AudioPort& audioPort);
  static std::shared_ptr<ISink> getSink(const AudioPort& audioPort);
  static std::shared_ptr<ISource> getSource(const AudioPort& audioPort);
};

#endif /* __SOURCE_SINK_MANAGER_HPP__ */

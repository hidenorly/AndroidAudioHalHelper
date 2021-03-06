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

#ifndef __AUDIO_PORT_HELPER_HPP__
#define __AUDIO_PORT_HELPER_HPP__

#include <system/audio.h>
#include <stdint.h>

#include "PipeAndFilterCommon.hpp"
#include "AudioFormat.hpp"
#include "AndroidHalTypes.hpp"

class AndroidAudioPortHelper
{
public:
  static void getAndroidPortFromSourceSink(AudioPort* pOutAudioPort, std::shared_ptr<ISourceSinkCommon> pSourceSink, AudioPortHandle portHandle = -1, std::string address = "", AudioModuleHandle hwModule = 0, AudioDevice androidAudioDeviceType = AUDIO_DEVICE_OUT_SPEAKER);
  static void getAndroidPortConfigFromSourceSink(AudioPortConfig* pOutAudioPortConfig, std::shared_ptr<ISourceSinkCommon> pSourceSink, AudioPortHandle portHandle = -1, std::string address = "", AudioModuleHandle hwModule = 0, AudioDevice androidAudioDeviceType = AUDIO_DEVICE_OUT_SPEAKER);

  static AudioDevice getAudioDeviceFromAudioPortConfig(const AudioPortConfig& port);
  static AudioDevice getAudioDeviceFromAudioPort(const AudioPort& port);

  static AudioGainConfig getAudioGainConfigFromAudioPortConfig(const AudioPortConfig& config);

  static bool isDevicePort(const AudioPort& port);
  static bool isDevicePort(const AudioPortConfig& port);

  static AudioDevice getAudioDeviceFromAudioSource(const AudioSource& source);

public:
  struct table_source_device
  {
    AudioSource source;
    AudioDevice device;
  };

protected:
  static table_source_device* getSourceDeviceTable(void);
};

#endif /* __AUDIO_PORT_HELPER_HPP__ */

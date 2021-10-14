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

#ifndef __AUDIO_FORMAT_HELPER_HPP__
#define __AUDIO_FORMAT_HELPER_HPP__

#include "AudioFormat.hpp"
#include <system/audio.h>
#include <stdint.h>
#include "AndroidHalTypes.hpp"

#include "PipeAndFilterCommon.hpp"

class AndroidFormatHelper
{
public:
  static AudioFormat::ENCODING getEncodingFromAndroidEncoding(AndroidAudioFormat androidEncoding);
  static AndroidAudioFormat getAndroidEncodingFromEncoding(AudioFormat::ENCODING afwEncoding);

  static AudioFormat::CHANNEL getChannelFromAndroidChannel(AudioChannelMask androidChannel);
  static AudioChannelMask getAndroidChannelFromChannel(AudioFormat::CHANNEL afwChannel);

  static AudioConfig getAndroidAudioConfigFromAudioFormat(AudioFormat afwFormat);
  static AudioFormat getAudioFormatFromAndroidPortConfig(const AudioPortConfig& audioPortConfig);
  static AudioFormat getAudioFormatFromAndroidAudioConfig(const AudioConfig& audioConfig);
  static AudioFormat getAudioFormatFromAndroidEffectConfig(const EffectBufferConfig& effectConfig);
};

#endif /* __AUDIO_FORMAT_HELPER_HPP__ */

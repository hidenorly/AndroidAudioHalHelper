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

#include "PipeAndFilterCommon.hpp"

class AndroidFormatHelper
{
public:
  static AudioFormat::ENCODING getEncodingFromAndroidEncoding(audio_format_t androidEncoding);
  static audio_format_t getAndroidEncodingFromEncoding(AudioFormat::ENCODING afwEncoding);

  static AudioFormat::CHANNEL getChannelFromAndroidChannel(uint32_t androidChannel);
  static int getAndroidChannelFromChannel(AudioFormat::CHANNEL afwChannel);

  static audio_config getAndroidAudioConfigFromAudioFormat(AudioFormat afwFormat);
};

#endif /* __AUDIO_FORMAT_HELPER_HPP__ */

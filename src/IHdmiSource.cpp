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

#include "IHdmiSource.hpp"

IHdmiSource::IHdmiSource()
{
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_AAC,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_STEREO ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_AAC,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_5_1CH ) );

  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_AC3,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_5_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_E_AC3,        AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_5_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_E_AC3,        AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_7_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_AC4,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_5_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_MAT,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_5_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_MAT,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_7_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_DOLBY_TRUEHD, AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_5_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_DOLBY_TRUEHD, AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_7_1CH ) );

  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_DTS,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_5_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_DTS,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_7_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_DTS_HD,       AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_5_1CH ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_DTS_HD,       AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_7_1CH ) );

  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_WMA,          AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_STEREO ) );
  mAudioFormats.push_back( AudioFormat( AudioFormat::ENCODING::COMPRESSED_WMA_PRO,      AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT, AudioFormat::CHANNEL::CHANNEL_STEREO ) );
}

IHdmiSource::~IHdmiSource()
{

}

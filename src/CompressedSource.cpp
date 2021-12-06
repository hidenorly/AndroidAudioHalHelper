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

#include "CompressedSource.hpp"

CompressedSource::CompressedSource():Source(), mFormat(AudioFormat::ENCODING::COMPRESSED)
{
  for(int anEncoding = AudioFormat::ENCODING::PCM_8BIT; anEncoding < AudioFormat::ENCODING::COMPRESSED_UNKNOWN; anEncoding++){
    for( int aChannel = AudioFormat::CHANNEL::CHANNEL_MONO; aChannel < AudioFormat::CHANNEL::CHANNEL_UNKNOWN; aChannel++){
      mAudioFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding,  48000, (AudioFormat::CHANNEL)aChannel) );
      mAudioFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding,  96000, (AudioFormat::CHANNEL)aChannel) );
      mAudioFormats.push_back( AudioFormat((AudioFormat::ENCODING)anEncoding, 192000, (AudioFormat::CHANNEL)aChannel) );
    }
  }
}

CompressedSource::~CompressedSource()
{

}

void CompressedSource::setAudioFormatPrimitive(AudioFormat format)
{
  mFormat = format;
}

void CompressedSource::readPrimitive(IAudioBuffer& buf)
{
  ByteBuffer esRawBuf( 256, 0 );
  buf.setRawBuffer( esRawBuf );
  buf.setAudioFormat( mFormat );
}

AudioFormat CompressedSource::getAudioFormat(void)
{
  return mFormat;
}

std::vector<AudioFormat> CompressedSource::getSupportedAudioFormats(void)
{
  return mAudioFormats;
}
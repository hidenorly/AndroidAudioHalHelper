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

#include "PlaybackRateFilter.hpp"
#include "AudioFormatAdaptor.hpp"
#include "FifoBuffer.hpp"

PlaybackRateFilter::PlaybackRateFilter():PassThroughFilter()
{

}

PlaybackRateFilter::~PlaybackRateFilter()
{

}

bool PlaybackRateFilter::setPlaybackRate(PlaybackRate rate)
{
  mPlaybackRate = rate;
  return true;
}


void PlaybackRateFilter::process(AudioBuffer& inBuf, AudioBuffer& outBuf)
{
  if( mPlaybackRate.speed != 1.0f ){
    AudioFormat srcFormat = inBuf.getAudioFormat();
    AudioFormat dstFormat = AudioFormat( srcFormat.getEncoding(), srcFormat.getSamplingRate()/mPlaybackRate.speed, srcFormat.getChannels() );
    mFifoBuffer.setAudioFormat( srcFormat );
    outBuf.setAudioFormat( dstFormat, true );
    AudioFormatAdaptor::convert(inBuf, outBuf);
    outBuf.setAudioFormat( srcFormat, true );
    mFifoBuffer.write( outBuf );
    int srcSize = inBuf.getRawBufferSize();
    int dstSize = outBuf.getRawBufferSize();
    if( dstSize > srcSize ){
      ByteBuffer rawDstBuffer = outBuf.getRawBuffer();
      rawDstBuffer.resize( srcSize );
      outBuf.setRawBuffer( rawDstBuffer );
      // outBuf.resize( inBuf.getNumberOfSamples() ); might be working?
    }
    mFifoBuffer.read( outBuf );
  } else {
    // speed = 1.0f;
    mFifoBuffer.write( inBuf );
    mFifoBuffer.read( outBuf );
  }
}

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

#include "DualMonoFilter.hpp"

DualMonoFilter::DualMonoFilter():PassThroughFilter(),mDualMonoMode(AUDIO_DUAL_MONO_MODE_OFF)
{

}

DualMonoFilter::~DualMonoFilter()
{

}


void DualMonoFilter::setDualMonoMode(DualMonoMode dualMonoMode)
{
  mDualMonoMode = dualMonoMode;
}

void DualMonoFilter::process(AudioBuffer& inBuf, AudioBuffer& outBuf)
{
  if( mDualMonoMode == AUDIO_DUAL_MONO_MODE_OFF || inBuf.getAudioFormat().getChannels() != AudioFormat::CHANNEL::CHANNEL_STEREO ){
    // OFF or not stereo channel
    outBuf = inBuf;
  } else {
    AudioFormat::ChannelMapper mapper;
    switch( mDualMonoMode ){
      case AUDIO_DUAL_MONO_MODE_LR:
        // blend L+R
        // dst, src
        mapper.insert_or_assign( AudioFormat::CH::L, AudioFormat::CH::L );
        mapper.insert_or_assign( AudioFormat::CH::L, AudioFormat::CH::R );
        mapper.insert_or_assign( AudioFormat::CH::R, AudioFormat::CH::L );
        mapper.insert_or_assign( AudioFormat::CH::R, AudioFormat::CH::R );
        break;
      case AUDIO_DUAL_MONO_MODE_LL:
        // use L
        // dst, src
        mapper.insert_or_assign( AudioFormat::CH::L, AudioFormat::CH::L );
        mapper.insert_or_assign( AudioFormat::CH::R, AudioFormat::CH::L );
        break;
      case AUDIO_DUAL_MONO_MODE_RR:
        // use R
        // dst, src
        mapper.insert_or_assign( AudioFormat::CH::L, AudioFormat::CH::R );
        mapper.insert_or_assign( AudioFormat::CH::R, AudioFormat::CH::R );
        break;
      default:;
        mapper.insert_or_assign( AudioFormat::CH::L, AudioFormat::CH::L );
        mapper.insert_or_assign( AudioFormat::CH::R, AudioFormat::CH::R );
        break;
    }
    AudioBuffer tmpBuf = inBuf.getSelectedChannelData(outBuf.getAudioFormat(), mapper);
    outBuf = tmpBuf;
  }
}
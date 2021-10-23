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

#include "AndroidHalEffectAcousticEchoCanceler.hpp"
#include "SourceSinkManager.hpp"
#include "AccousticEchoCancelledSource.hpp"

IAcousticEchoCancelerEffect::IAcousticEchoCancelerEffect():IEffect( IAcousticEchoCancelerEffect::UUID ), mEchoDelayMs(0)
{

}

IAcousticEchoCancelerEffect::~IAcousticEchoCancelerEffect()
{

}

HalResult IAcousticEchoCancelerEffect::setInputDevice(AudioDevice device)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;

  mInputDevice = device;

  std::shared_ptr<ISource> pSource = SourceSinkManager::getSource( device );
  if( pSource ){
    mInputSource = pSource;
    ensureFilter();
  }

  return result;
}

HalResult IAcousticEchoCancelerEffect::setAudioSource(AudioSource source)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;

  mAudioSource = source;

  std::shared_ptr<ISource> pSource; // = SourceSinkManager::getSource( source );
  if( pSource ){
    mReferenceSource = pSource;
    ensureFilter();
  }

  return result;
}

void IAcousticEchoCancelerEffect::ensureFilter(void)
{
  if( mInputSource && mAecSource ){
    mAecSource = std::make_shared<AccousticEchoCancelledSource>( mInputSource, mAecSource );
  }
}


HalResult IAcousticEchoCancelerEffect::setEchoDelay(uint32_t echoDelayMs)
{
  mEchoDelayMs = echoDelayMs;

  std::shared_ptr<AccousticEchoCancelledSource> pAec = std::dynamic_pointer_cast<AccousticEchoCancelledSource>( mAecSource );
  if( pAec ){
    // TODO : apply the echoDelayMs to the pAEC (TODO: need to add the additional delay)
    pAec->adjustDelay();
  }
  return HalResult::OK;
}

uint32_t IAcousticEchoCancelerEffect::getEchoDelay(void)
{
  return mEchoDelayMs;
}

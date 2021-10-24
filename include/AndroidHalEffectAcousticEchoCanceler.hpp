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

#ifndef __ANDROID_HAL_ACOUSTIC_ECHO_CANCELLER_EFFECT_HPP__
#define __ANDROID_HAL_ACOUSTIC_ECHO_CANCELLER_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IAcousticEchoCancelerEffect : public IEffect
{
public:
  IAcousticEchoCancelerEffect();
  virtual ~IAcousticEchoCancelerEffect();

  static inline const char* UUID = "7b491460-8d4d-11e0-bd61-0002a5d5c51b";
  _EffectGetInstanceIfUuidMatch(IAcousticEchoCancelerEffect);

protected:
  uint32_t mEchoDelayMs;
  AudioDevice mInputDevice;
  AudioSource mAudioSource;

  std::shared_ptr<ISource> mInputSource;
  std::shared_ptr<ISource> mReferenceSource;
  std::shared_ptr<ISource> mAecSource;
  virtual void ensureFilter(void);

public:
  virtual HalResult setInputDevice(AudioDevice device);
  virtual HalResult setAudioSource(AudioSource source);

  virtual HalResult setEchoDelay(uint32_t echoDelayMs);
  virtual uint32_t getEchoDelay(void);
};

#endif /* __ANDROID_HAL_ACOUSTIC_ECHO_CANCELLER_EFFECT_HPP__ */

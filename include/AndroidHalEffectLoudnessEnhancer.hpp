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

#ifndef __ANDROID_HAL_LOUDNESS_ENHANCER_EFFECT_HPP__
#define __ANDROID_HAL_LOUDNESS_ENHANCER_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class ILoudnessEnhancerEffect : public IEffect
{
public:
  ILoudnessEnhancerEffect();
  virtual ~ILoudnessEnhancerEffect();

  static inline const char* UUID = "fe3199be-aed0-413f-87bb-11260eb63cf1";
  _EffectGetInstanceIfUuidMatch(ILoudnessEnhancerEffect);

protected:
  int32_t mTargetGainMb;

public:
  virtual HalResult setTargetGain(int32_t targetGainMb);
  virtual int32_t getTargetGain(void);
};

#endif /* __ANDROID_HAL_LOUDNESS_ENHANCER_EFFECT_HPP__ */

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

#ifndef __ANDROID_HAL_AUTOMATIC_GAIN_CONTROL_EFFECT_HPP__
#define __ANDROID_HAL_AUTOMATIC_GAIN_CONTROL_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IAutomaticGainControlEffect : public IEffect
{
public:
  IAutomaticGainControlEffect();
  virtual ~IAutomaticGainControlEffect();

  static inline const char* UUID = "0a8abfe0-654c-11e0-ba26-0002a5d5c51b";
  _EffectGetInstanceIfUuidMatch(IAutomaticGainControlEffect);

public:
  struct AllProperties
  {
    int16_t targetLevelMb;
    int16_t compGainMb;
    bool limiterEnabled;
    AllProperties():targetLevelMb(0), compGainMb(0), limiterEnabled(false){};
  };

protected:
  AllProperties mProperties;

public:
  virtual HalResult setTargetLevel(int16_t targetLevelMb);
  virtual int16_t getTargetLevel(void);
  virtual HalResult setCompGain(int16_t compGainMb);
  virtual int16_t getCompGain(void);
  virtual HalResult setLimiterEnabled(bool enabled);
  virtual bool isLimiterEnabled(void);
  virtual HalResult setAllProperties(AllProperties properties);
  virtual AllProperties getAllProperties(void);
};

#endif /* __ANDROID_HAL_AUTOMATIC_GAIN_CONTROL_EFFECT_HPP__ */

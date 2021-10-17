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

#ifndef __ANDROID_HAL_BASS_BOOST_EFFECT_HPP__
#define __ANDROID_HAL_BASS_BOOST_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IBassBoostEffect : public IEffect
{
public:
  IBassBoostEffect();
  virtual ~IBassBoostEffect();

  static inline const char* UUID = "0634f220-ddd4-11db-a0fc-0002a5d5c51b";

protected:
  uint16_t mStrength;

public:
  virtual bool isStrengthSupported(void);

  enum StrengthRange
  {
    MIN = 0,
    MAX = 1000
  };

  virtual HalResult setStrength(uint16_t strength);
  virtual uint16_t getStrength(void);
};

#endif /* __ANDROID_HAL_BASS_BOOST_EFFECT_HPP__ */

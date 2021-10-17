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

#ifndef __ANDROID_HAL_DOWN_MIX_EFFECT_HPP__
#define __ANDROID_HAL_DOWN_MIX_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IDownmixEffect : public IEffect
{
public:
  IDownmixEffect();
  virtual ~IDownmixEffect();

  static inline const char* UUID = "93f04452-e4fe-41cc-91f9-e475b6d1d69f";

  enum Type
  {
    STRIP, // throw away the extra channels
    FOLD   // mix the extra channels with FL/FR
  };

protected:
  Type mType;

public:
  virtual HalResult setType(Type preset);
  virtual Type getType(void);
};

#endif /* __ANDROID_HAL_DOWN_MIX_EFFECT_HPP__ */

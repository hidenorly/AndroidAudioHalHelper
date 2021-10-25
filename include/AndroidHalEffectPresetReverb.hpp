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

#ifndef __ANDROID_HAL_PRESET_REVERB_EFFECT_HPP__
#define __ANDROID_HAL_PRESET_REVERB_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IPresetReverbEffect : public IEffect
{
public:
  IPresetReverbEffect();
  virtual ~IPresetReverbEffect();

  static inline const char* UUID = "47382d60-ddd8-11db-bf3a-0002a5d5c51b";

public:
  enum Preset
  {
    NONE,
    SMALLROOM,
    MEDIUMROOM,
    LARGEROOM,
    MEDIUMHALL,
    LARGEHALL,
    PLATE,
    LAST = PLATE
  };

protected:
  Preset mPreset;

public:
  virtual HalResult setPreset(Preset preset);
  virtual Preset getPreset(void);
};

#endif /* __ANDROID_HAL_PRESET_REVERB_EFFECT_HPP__ */

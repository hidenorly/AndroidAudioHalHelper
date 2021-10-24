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

#ifndef __ANDROID_HAL_ENVIRONMENTAL_REVERB_EFFECT_HPP__
#define __ANDROID_HAL_ENVIRONMENTAL_REVERB_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IEnvironmentalReverbEffect : public IEffect
{
public:
  IEnvironmentalReverbEffect();
  virtual ~IEnvironmentalReverbEffect();

  static inline const char* UUID = "c2e5d5f0-94bd-4763-9cac-4e234d06839e";
  _EffectGetInstanceIfUuidMatch(IEnvironmentalReverbEffect);

public:
  enum ParamRange
  {
    ROOM_LEVEL_MIN = -6000,
    ROOM_LEVEL_MAX = 0,
    ROOM_HF_LEVEL_MIN = -4000,
    ROOM_HF_LEVEL_MAX = 0,
    DECAY_TIME_MIN = 100,
    DECAY_TIME_MAX = 20000,
    DECAY_HF_RATIO_MIN = 100,
    DECAY_HF_RATIO_MAX = 1000,
    REFLECTIONS_LEVEL_MIN = -6000,
    REFLECTIONS_LEVEL_MAX = 0,
    REFLECTIONS_DELAY_MIN = 0,
    REFLECTIONS_DELAY_MAX = 65,
    REVERB_LEVEL_MIN = -6000,
    REVERB_LEVEL_MAX = 0,
    REVERB_DELAY_MIN = 0,
    REVERB_DELAY_MAX = 65,
    DIFFUSION_MIN = 0,
    DIFFUSION_MAX = 1000,
    DENSITY_MIN = 0,
    DENSITY_MAX = 1000
  };

  struct AllProperties
  {
    int16_t  roomLevel;
    int16_t  roomHfLevel;
    uint32_t decayTime;
    int16_t  decayHfRatio;
    int16_t  reflectionsLevel;
    uint32_t reflectionsDelay;
    int16_t  reverbLevel;
    uint32_t reverbDelay;
    int16_t  diffusion;
    int16_t  density;
    AllProperties():
      roomLevel(ROOM_LEVEL_MIN),
      roomHfLevel(ROOM_HF_LEVEL_MIN),
      decayTime(DECAY_TIME_MIN),
      decayHfRatio(DECAY_HF_RATIO_MIN),
      reflectionsLevel(REFLECTIONS_LEVEL_MIN),
      reflectionsDelay(REFLECTIONS_DELAY_MIN),
      reverbLevel(REVERB_LEVEL_MIN),
      reverbDelay(REVERB_DELAY_MIN),
      diffusion(DIFFUSION_MIN),
      density(DENSITY_MIN){};
  };

protected:
  AllProperties mProperties;
  bool mBypass;

public:
  virtual HalResult setBypass(bool bypass);
  virtual bool getBypass(void);

  // in millibels, range -6000 to 0
  virtual HalResult setRoomLevel(int16_t roomLevel);
  virtual int16_t getRoomLevel(void);

  // in millibels, range -4000 to 0
  virtual HalResult setRoomHfLevel(int16_t roomHfLevel);
  virtual int16_t getRoomHfLevel(void);

  // in milliseconds, range 100 to 20000
  virtual HalResult setDecayTime(uint32_t decayTime);
  virtual uint32_t getDecayTime(void);

  // in permilles, range 100 to 1000
  virtual HalResult setDecayHfRatio(int16_t decayHfRatio);
  virtual int16_t getDecayHfRatio(void);

  // in millibels, range -6000 to 0
  virtual HalResult setReflectionsLevel(int16_t reflectionsLevel);
  virtual int16_t getReflectionsLevel(void);

  // in milliseconds, range 0 to 65
  virtual HalResult setReflectionsDelay(uint32_t reflectionsDelay);
  virtual uint32_t getReflectionsDelay(void);

  // in millibels, range -6000 to 0
  virtual HalResult setReverbLevel(int16_t reverbLevel);
  virtual int16_t getReverbLevel(void);

  // in milliseconds, range 0 to 65
  virtual HalResult setReverbDelay(uint32_t reverbDelay);
  virtual uint32_t getReverbDelay(void);

  // in permilles, range 0 to 1000
  virtual HalResult setDiffusion(int16_t diffusion);
  virtual int16_t getDiffusion(void);

  // in permilles, range 0 to 1000
  virtual HalResult setDensity(int16_t density);
  virtual int16_t getDensity(void);

  virtual HalResult setAllProperties(const AllProperties properties);
  virtual AllProperties getAllProperties(void);
};

#endif /* __ANDROID_HAL_ENVIRONMENTAL_REVERB_EFFECT_HPP__ */

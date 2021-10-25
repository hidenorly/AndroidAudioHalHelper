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

#ifndef __ANDROID_HAL_BASS_EQUALIZER_EFFECT_HPP__
#define __ANDROID_HAL_BASS_EQUALIZER_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IEqualizerEffect : public IEffect
{
public:
  IEqualizerEffect();
  virtual ~IEqualizerEffect();

  static inline const char* UUID = "0bed4300-ddd6-11db-8f34-0002a5d5c51b";

  virtual uint16_t getNumBands(void);
  struct LevelRange
  {
    int16_t minLevel;
    int16_t maxLevel;
    LevelRange():minLevel(0),maxLevel(0){};
  };
  virtual LevelRange getLevelRange(void);
  virtual HalResult setBandLevel(uint16_t band, int16_t level);
  virtual uint16_t getBandLevel(uint16_t band);

  virtual uint16_t getBandForFrequency(uint32_t freqmHz);
  struct FreqMhzRange
  {
    uint32_t minFreqmHz;
    uint32_t maxFreqmHz;
    FreqMhzRange():minFreqmHz(0), maxFreqmHz(0){};
  };
  virtual FreqMhzRange getBandFrequencyRange(uint16_t band);
  virtual uint32_t getBandCenterFrequency(uint16_t band);

  virtual std::vector<std::string> getPresetNames(void);
  virtual HalResult setCurrentPreset(uint16_t preset);
  virtual uint16_t getCurrentPreset(void);

  struct AllProperties {
    uint16_t curPreset;
    std::vector<int16_t> bandLevels;
    AllProperties():curPreset(0){};
  };

  virtual HalResult setAllProperties(AllProperties properties);
  virtual AllProperties getAllProperties(void);
};

#endif /* __ANDROID_HAL_BASS_EQUALIZER_EFFECT_HPP__ */

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

#ifndef __ANDROID_HAL_VIRTUALIZER_EFFECT_HPP__
#define __ANDROID_HAL_VIRTUALIZER_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IVirtualizerEffect : public IEffect
{
public:
  IVirtualizerEffect();
  virtual ~IVirtualizerEffect();

  static inline const char* UUID = "37cc2c00-dddd-11db-8577-0002a5d5c51b";

public:
  enum StrengthRange
  {
    MIN = 0,
    MAX = 1000
  };
  struct SpeakerAngles
  {
    AudioChannelMask mask;
    std::vector<int16_t> azimuth;
    std::vector<int16_t> elevation;
    SpeakerAngles():mask(AUDIO_CHANNEL_OUT_STEREO), azimuth({0}), elevation({0}){};
  };

protected:
  uint16_t mStrength;
  DeviceAddress mDeviceAddress;

public:
  virtual bool isStrengthSupported(void);
  virtual HalResult setStrength(uint16_t strength);
  virtual uint16_t getStrength(void);
  virtual SpeakerAngles getVirtualSpeakerAngles(const AudioChannelMask& mask, const DeviceAddress& device);
  virtual HalResult forceVirtualizationMode(const DeviceAddress& device);
  virtual DeviceAddress getVirtualizationMode(void);
};

#endif /* __ANDROID_HAL_VIRTUALIZER_EFFECT_HPP__ */

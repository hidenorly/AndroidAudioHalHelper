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

#ifndef __ANDROID_HAL_NOISE_SUPRESSION_EFFECT_HPP__
#define __ANDROID_HAL_NOISE_SUPRESSION_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class INoiseSuppressionEffect : public IEffect
{
public:
  INoiseSuppressionEffect();
  virtual ~INoiseSuppressionEffect();

  static inline const char* UUID = "58b4b260-8e06-11e0-aa8e-0002a5d5c51b";

public:
  enum Level
  {
    LOW,
    MEDIUM,
    HIGH
  };

  enum Type
  {
    SINGLE_CHANNEL,
    MULTI_CHANNEL
  };

  struct AllProperties
  {
    Level level;
    Type type;
    AllProperties():level(Level::LOW), type(Type::MULTI_CHANNEL){};
  };

protected:
  AllProperties mProperties;

public:
  virtual HalResult setSuppressionLevel(Level level);
  virtual Level getSuppressionLevel(void);
  virtual HalResult setSuppressionType(Type type);
  virtual Type getSuppressionType(void);
  virtual HalResult setAllProperties(AllProperties properties);
  virtual AllProperties getAllProperties(void);
};

#endif /* __ANDROID_HAL_NOISE_SUPRESSION_EFFECT_HPP__ */

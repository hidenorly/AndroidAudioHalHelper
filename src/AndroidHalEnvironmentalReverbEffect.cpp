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

#include "AndroidHalTypes.hpp"
#include "AndroidHalEnvironmentalReverbEffect.hpp"

IEnvironmentalReverbEffect::IEnvironmentalReverbEffect():IEffect( IEnvironmentalReverbEffect::UUID ), mBypass(false)
{

}

IEnvironmentalReverbEffect::~IEnvironmentalReverbEffect()
{

}

HalResult IEnvironmentalReverbEffect::setBypass(bool bypass)
{
  HalResult result = HalResult::OK;
  mBypass = bypass;
  return result;
}

bool IEnvironmentalReverbEffect::getBypass(void)
{
  return mBypass;
}

HalResult IEnvironmentalReverbEffect::setRoomLevel(int16_t roomLevel)
{
  HalResult result = HalResult::OK;
  mProperties.roomLevel = roomLevel;
  return result;
}

int16_t IEnvironmentalReverbEffect::getRoomLevel(void)
{
  return mProperties.roomLevel;
}

HalResult IEnvironmentalReverbEffect::setRoomHfLevel(int16_t roomHfLevel)
{
  HalResult result = HalResult::OK;
  mProperties.roomHfLevel = roomHfLevel;
  return result;
}

int16_t IEnvironmentalReverbEffect::getRoomHfLevel(void)
{
  return mProperties.roomHfLevel;
}

HalResult IEnvironmentalReverbEffect::setDecayTime(uint32_t decayTime)
{
  HalResult result = HalResult::OK;
  mProperties.decayTime = decayTime;
  return result;
}

uint32_t IEnvironmentalReverbEffect::getDecayTime(void)
{
  return mProperties.decayTime;
}

HalResult IEnvironmentalReverbEffect::setDecayHfRatio(int16_t decayHfRatio)
{
  HalResult result = HalResult::OK;
  mProperties.decayHfRatio = decayHfRatio;
  return result;
}

int16_t IEnvironmentalReverbEffect::getDecayHfRatio(void)
{
  return mProperties.decayHfRatio;
}

HalResult IEnvironmentalReverbEffect::setReflectionsLevel(int16_t reflectionsLevel)
{
  HalResult result = HalResult::OK;
  mProperties.reflectionsLevel = reflectionsLevel;
  return result;
}

int16_t IEnvironmentalReverbEffect::getReflectionsLevel(void)
{
  return mProperties.reflectionsLevel;
}

HalResult IEnvironmentalReverbEffect::setReflectionsDelay(uint32_t reflectionsDelay)
{
  HalResult result = HalResult::OK;
  mProperties.reflectionsDelay = reflectionsDelay;
  return result;
}

uint32_t IEnvironmentalReverbEffect::getReflectionsDelay(void)
{
  return mProperties.reflectionsDelay;
}

HalResult IEnvironmentalReverbEffect::setReverbLevel(int16_t reverbLevel)
{
  HalResult result = HalResult::OK;
  mProperties.reverbLevel = reverbLevel;
  return result;
}

int16_t IEnvironmentalReverbEffect::getReverbLevel(void)
{
  return mProperties.reverbLevel;
}

HalResult IEnvironmentalReverbEffect::setReverbDelay(uint32_t reverbDelay)
{
  HalResult result = HalResult::OK;
  mProperties.reverbDelay = reverbDelay;
  return result;
}

uint32_t IEnvironmentalReverbEffect::getReverbDelay(void)
{
  return mProperties.reverbDelay;
}

HalResult IEnvironmentalReverbEffect::setDiffusion(int16_t diffusion)
{
  HalResult result = HalResult::OK;
  mProperties.diffusion = diffusion;
  return result;
}

int16_t IEnvironmentalReverbEffect::getDiffusion(void)
{
  return mProperties.diffusion;
}

HalResult IEnvironmentalReverbEffect::setDensity(int16_t density)
{
  HalResult result = HalResult::OK;
  mProperties.density = density;
  return result;
}

int16_t IEnvironmentalReverbEffect::getDensity(void)
{
  return mProperties.density;
}

HalResult IEnvironmentalReverbEffect::setAllProperties(const IEnvironmentalReverbEffect::AllProperties properties)
{
  HalResult result = HalResult::OK;
  mProperties = properties;
  return result;
}

IEnvironmentalReverbEffect::AllProperties IEnvironmentalReverbEffect::getAllProperties(void)
{
  return mProperties;
}

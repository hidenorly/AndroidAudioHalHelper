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
#include <algorithm>

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
  roomLevel = std::min<int16_t>( std::max<int16_t>(roomLevel, ParamRange::ROOM_LEVEL_MIN), ParamRange::ROOM_LEVEL_MAX );
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
  roomHfLevel = std::min<int16_t>( std::max<int16_t>(roomHfLevel, ParamRange::ROOM_HF_LEVEL_MIN), ParamRange::ROOM_HF_LEVEL_MAX );
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
  decayTime = std::min<uint32_t>( std::max<uint32_t>(decayTime, ParamRange::DECAY_TIME_MIN), ParamRange::DECAY_TIME_MIN );
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
  decayHfRatio = std::min<int16_t>( std::max<int16_t>(decayHfRatio, ParamRange::DECAY_HF_RATIO_MIN), ParamRange::DECAY_HF_RATIO_MAX );
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
  reflectionsLevel = std::min<int16_t>( std::max<int16_t>(reflectionsLevel, ParamRange::REFLECTIONS_LEVEL_MIN), ParamRange::REFLECTIONS_LEVEL_MAX );
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
  reflectionsDelay = std::min<uint32_t>( std::max<uint32_t>(reflectionsDelay, ParamRange::REFLECTIONS_DELAY_MIN), ParamRange::REFLECTIONS_DELAY_MAX );
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
  reverbLevel = std::min<int16_t>( std::max<int16_t>(reverbLevel, ParamRange::REVERB_LEVEL_MIN), ParamRange::REVERB_LEVEL_MAX );
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
  reverbDelay = std::min<uint32_t>( std::max<uint32_t>(reverbDelay, ParamRange::REVERB_DELAY_MIN), ParamRange::REVERB_DELAY_MAX );
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
  diffusion = std::min<int16_t>( std::max<int16_t>(diffusion, ParamRange::DIFFUSION_MIN), ParamRange::DIFFUSION_MAX );
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
  density = std::min<int16_t>( std::max<int16_t>(density, ParamRange::DENSITY_MIN), ParamRange::DENSITY_MIN );
  mProperties.density = density;
  return result;
}

int16_t IEnvironmentalReverbEffect::getDensity(void)
{
  return mProperties.density;
}

HalResult IEnvironmentalReverbEffect::setAllProperties(const IEnvironmentalReverbEffect::AllProperties properties)
{
  bool result = true;

  IEnvironmentalReverbEffect::AllProperties bkupProperties = mProperties;

  result &= HalResult::OK == setRoomLevel( mProperties.roomLevel );
  result &= HalResult::OK == setRoomHfLevel( mProperties.roomHfLevel );
  result &= HalResult::OK == setDecayTime( mProperties.decayTime );
  result &= HalResult::OK == setDecayHfRatio( mProperties.decayHfRatio );
  result &= HalResult::OK == setReflectionsLevel( mProperties.reflectionsLevel );
  result &= HalResult::OK == setReflectionsDelay( mProperties.reflectionsDelay );
  result &= HalResult::OK == setReverbLevel( mProperties.reverbLevel );
  result &= HalResult::OK == setReverbDelay( mProperties.reverbDelay );
  result &= HalResult::OK == setDiffusion( mProperties.diffusion );
  result &= HalResult::OK == setDensity( mProperties.density );

  if( !result ){
    setAllProperties( bkupProperties );
  }

  return result ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
}

IEnvironmentalReverbEffect::AllProperties IEnvironmentalReverbEffect::getAllProperties(void)
{
  return mProperties;
}

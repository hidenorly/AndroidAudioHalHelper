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
#include "AndroidHalEffectAutomaticGainControl.hpp"

IAutomaticGainControlEffect::IAutomaticGainControlEffect():IEffect( IAutomaticGainControlEffect::UUID ), mProperties()
{

}

IAutomaticGainControlEffect::~IAutomaticGainControlEffect()
{

}

HalResult IAutomaticGainControlEffect::setTargetLevel(int16_t targetLevelMb)
{
  HalResult result = HalResult::OK;

  mProperties.targetLevelMb = targetLevelMb;

  return result;
}

int16_t IAutomaticGainControlEffect::getTargetLevel(void)
{
  return mProperties.targetLevelMb;
}

HalResult IAutomaticGainControlEffect::setCompGain(int16_t compGainMb)
{
  HalResult result = HalResult::OK;

  mProperties.compGainMb = compGainMb;

  return result;
}

int16_t IAutomaticGainControlEffect::getCompGain(void)
{
  return mProperties.compGainMb;
}

HalResult IAutomaticGainControlEffect::setLimiterEnabled(bool enabled)
{
  HalResult result = HalResult::OK;

  mProperties.limiterEnabled = enabled;

  return result;
}

bool IAutomaticGainControlEffect::isLimiterEnabled(void)
{
  return mProperties.limiterEnabled;
}

HalResult IAutomaticGainControlEffect::setAllProperties(IAutomaticGainControlEffect::AllProperties properties)
{
  bool result = true;

  IAutomaticGainControlEffect::AllProperties bkupProperties = mProperties;

  result &= HalResult::OK == setTargetLevel( properties.targetLevelMb );
  result &= HalResult::OK == setCompGain( properties.compGainMb );
  result &= HalResult::OK == setLimiterEnabled( properties.limiterEnabled );

  if( !result ){
    mProperties = bkupProperties;
    setAllProperties( bkupProperties );
  }

  return result ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
}

IAutomaticGainControlEffect::AllProperties IAutomaticGainControlEffect::getAllProperties(void)
{
  return mProperties;
}

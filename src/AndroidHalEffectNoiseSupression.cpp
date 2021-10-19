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

#include "AndroidHalEffectNoiseSupression.hpp"

INoiseSuppressionEffect::INoiseSuppressionEffect():IEffect( INoiseSuppressionEffect::UUID )
{

}

INoiseSuppressionEffect::~INoiseSuppressionEffect()
{

}

HalResult INoiseSuppressionEffect::setSuppressionLevel(INoiseSuppressionEffect::Level level)
{
  HalResult result = HalResult::OK;

  mProperties.level = level;

  return result;
}

INoiseSuppressionEffect::Level INoiseSuppressionEffect::getSuppressionLevel(void)
{
  return mProperties.level;
}

HalResult INoiseSuppressionEffect::setSuppressionType(INoiseSuppressionEffect::Type type)
{
  HalResult result = HalResult::OK;

  mProperties.type = type;

  return result;
}

INoiseSuppressionEffect::Type INoiseSuppressionEffect::getSuppressionType(void)
{
  return mProperties.type;
}

HalResult INoiseSuppressionEffect::setAllProperties(INoiseSuppressionEffect::AllProperties properties)
{
  bool result = true;

  INoiseSuppressionEffect::AllProperties bkup = mProperties;

  result &= HalResult::OK == setSuppressionLevel( properties.level );
  result &= HalResult::OK == setSuppressionType( properties.type );

  if( !result ){
    setAllProperties( bkup );
  }

  return result ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
}

INoiseSuppressionEffect::AllProperties INoiseSuppressionEffect::getAllProperties(void)
{
  return mProperties;
}

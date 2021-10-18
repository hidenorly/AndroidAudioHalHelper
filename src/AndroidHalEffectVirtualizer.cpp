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
#include "AndroidHalEffectVirtualizer.hpp"

IVirtualizerEffect::IVirtualizerEffect():IEffect( IVirtualizerEffect::UUID ), mStrength( IVirtualizerEffect::StrengthRange::MIN )
{

}

IVirtualizerEffect::~IVirtualizerEffect()
{

}

bool IVirtualizerEffect::isStrengthSupported(void)
{
  return false;
}

HalResult IVirtualizerEffect::setStrength(uint16_t strength)
{
  HalResult result = HalResult::OK;

  strength = std::max<uint16_t>( std::min<uint16_t>( strength, IVirtualizerEffect::StrengthRange::MAX ), IVirtualizerEffect::StrengthRange::MIN );
  mStrength = strength;

  return result;
}

uint16_t IVirtualizerEffect::getStrength(void)
{
  return mStrength;
}

IVirtualizerEffect::SpeakerAngles IVirtualizerEffect::getVirtualSpeakerAngles(const AudioChannelMask& mask, const DeviceAddress& device)
{
  IVirtualizerEffect::SpeakerAngles result;

  result.mask = mask;
  mDeviceAddress = device;

  return result;
}

HalResult IVirtualizerEffect::forceVirtualizationMode(const DeviceAddress& device)
{
  HalResult result = HalResult::OK;

  mDeviceAddress = device;

  return result;
}

DeviceAddress IVirtualizerEffect::getVirtualizationMode(void)
{
  return mDeviceAddress;
}
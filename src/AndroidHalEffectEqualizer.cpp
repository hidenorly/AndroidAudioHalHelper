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

#include "AndroidHalEffectEqualizer.hpp"

IEqualizerEffect::IEqualizerEffect():IEffect( IEqualizerEffect::UUID )
{

}

IEqualizerEffect::~IEqualizerEffect()
{

}

uint16_t IEqualizerEffect::getNumBands(void)
{
  return 0;
}

IEqualizerEffect::LevelRange IEqualizerEffect::getLevelRange(void)
{
  IEqualizerEffect::LevelRange result;

  return result;
}

HalResult IEqualizerEffect::setBandLevel(uint16_t band, int16_t level)
{
  HalResult result = HalResult::OK;

  return result;
}

uint16_t IEqualizerEffect::getBandLevel(uint16_t band)
{
  return 0;
}

uint16_t IEqualizerEffect::getBandForFrequency(uint32_t freqmHz)
{
  return 0;
}

IEqualizerEffect::FreqMhzRange IEqualizerEffect::getBandFrequencyRange(uint16_t band)
{
  IEqualizerEffect::FreqMhzRange result;

  return result;
}

uint32_t IEqualizerEffect::getBandCenterFrequency(uint16_t band)
{
  return 0;
}

std::vector<std::string> IEqualizerEffect::getPresetNames(void)
{
  std::vector<std::string> result;

  return result;
}

HalResult IEqualizerEffect::setCurrentPreset(uint16_t preset)
{
  HalResult result = HalResult::OK;

  return result;
}

uint16_t IEqualizerEffect::getCurrentPreset(void)
{
  return 0;
}

HalResult IEqualizerEffect::setAllProperties(IEqualizerEffect::AllProperties properties)
{
  HalResult result = HalResult::OK;

  return result;
}

IEqualizerEffect::AllProperties IEqualizerEffect::getAllProperties(void)
{
  IEqualizerEffect::AllProperties result;

  return result;
}

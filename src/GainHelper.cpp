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

#include "GainHelper.hpp"
#include <cmath>
#include <algorithm>

float GainHelper::getAmplifierRatioFromDb(float gainDb)
{
  float result = 1.0f;
  if( gainDb ){
    result = std::pow(10, gainDb/20.0f);
  }
  return result;
}

float GainHelper::getVolumeRatioPercentageFromDb(float gainDb, float minVolumePercent, float maxVolumePercent)
{
  float result = getAmplifierRatioFromDb( gainDb ) * 100.0f;
  if( minVolumePercent != FLT_MIN && maxVolumePercent != FLT_MAX ){
    result = std::max( std::min( result, minVolumePercent) , maxVolumePercent );
  }
  return result;
}

float GainHelper::getDbFromAmplifierRatio(float amplifierRatio)
{
  return 20.0f * std::log10( amplifierRatio );
}

float GainHelper::getDbFromVolumeRatioPercentage(float volumeRatioPercentage)
{
  return getDbFromAmplifierRatio( volumeRatioPercentage / 100.0f );
}

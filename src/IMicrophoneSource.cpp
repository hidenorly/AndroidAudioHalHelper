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

#include "IMicrophoneSource.hpp"
#include <algorithm>
#include "MicrophoneInfoHelper.hpp"


IMicrophoneSource::IMicrophoneSource():mDirection( MIC_DIRECTION_UNSPECIFIED ), mZoom( 0.0f )
{

}

IMicrophoneSource::~IMicrophoneSource()
{

}

AudioMicrophoneCharacteristic IMicrophoneSource::getActiveMicrophones(void)
{
  return MicrophoneInfoHelper::getMicrophoneInfo( shared_from_this() );
}

bool IMicrophoneSource::setMicrophoneDirection(AudioMicrophoneDirection direction)
{
  bool result = true;

  mDirection = direction;

  return result;
}

bool IMicrophoneSource::setMicrophoneFieldDimension(float zoom)
{
  bool result = true;

  mZoom = std::max<float>(-1.0f, std::min<float>( 1.0f, zoom ) );

  return result;
}
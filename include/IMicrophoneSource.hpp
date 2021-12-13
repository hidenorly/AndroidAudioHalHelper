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

#ifndef __IMICROPHONE_SOURCE_HPP__
#define __IMICROPHONE_SOURCE_HPP__

#include "LPcmSource.hpp"
#include "AndroidHalTypes.hpp"

class IMicrophoneSource : public LPcmSource, public std::enable_shared_from_this<IMicrophoneSource>
{
protected:
  AudioMicrophoneDirection mDirection;
  float mZoom;

public:
  IMicrophoneSource();
  virtual ~IMicrophoneSource();
  virtual AudioMicrophoneCharacteristic getMicrophoneInfo(void);
  virtual bool setMicrophoneDirection(AudioMicrophoneDirection direction);
  virtual bool setMicrophoneFieldDimension(float zoom);

  virtual std::string toString(void){return "IMicrophoneSource";};
};

#endif /* __IMICROPHONE_SOURCE_HPP__ */
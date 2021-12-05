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

#ifndef __LPCM_SOURCE_HPP__
#define __LPCM_SOURCE_HPP__

#include "Source.hpp"

class LPcmSource : public Source
{
protected:
  std::vector<AudioFormat> mSupportedFormats;

public:
  LPcmSource();
  virtual ~LPcmSource();
  virtual std::string toString(void){ return "LPcmSource";};
  virtual std::vector<AudioFormat> getSupportedAudioFormats(void);
  virtual bool isAvailableFormat(AudioFormat format);
};

#endif /* __LPCM_SOURCE_HPP__ */
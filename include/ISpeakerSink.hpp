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

#ifndef __ISPEAKER_SINK_HPP__
#define __ISPEAKER_SINK_HPP__

#include "Sink.hpp"

class LPcmSink : public Sink
{
protected:
  std::vector<AudioFormat> mSupportedFormats;

public:
  LPcmSink();
  virtual ~LPcmSink();
  virtual std::string toString(void){ return "LPcmSink";};
  virtual std::vector<AudioFormat> getSupportedAudioFormats(void);
  virtual bool isAvailableFormat(AudioFormat format);
};

class ISpeakerSink : public LPcmSink
{
public:
  ISpeakerSink();
  virtual ~ISpeakerSink();
  virtual std::string toString(void){ return "ISpeakerSink";};
};

#endif /* __ISPEAKER_SINK_HPP__ */
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

#ifndef __IHDMI_ARC_SINK_HPP__
#define __IHDMI_ARC_SINK_HPP__

#include "Sink.hpp"
#include "EncodedSink.hpp"

class CompressedSink : public Sink
{
protected:
  std::vector<AudioFormat> mAudioFormats;
  AudioFormat mFormat;

protected:
  virtual void setAudioFormatPrimitive(AudioFormat format){mFormat=format;};

public:
  CompressedSink():Sink(),mFormat(AudioFormat::ENCODING::ENCODING_DEFAULT){};
  virtual ~CompressedSink(){};
  virtual AudioFormat getAudioFormat(void){ return mFormat; };
  std::vector<AudioFormat> getSupportedAudioFormats(void){ return mAudioFormats; };
  virtual std::string toString(void){return "CompressedSink";};
};

class IHdmiArcSink : public CompressedSink
{
public:
  enum ArcType
  {
    ARC,
    EARC,
  };
  const static inline char* PARAM_KEY_SAD = "hdmi.edid.sad";

protected:
  int mParameterHandler;
  bool mIsAtmosCapable;
  ArcType mArcType;

public:
  IHdmiArcSink(ArcType arcType = ArcType::ARC);
  virtual ~IHdmiArcSink();

  ArcType getArcType(void);

  virtual std::string toString(void){ return "HdmiArcSink";};
};


#endif /* __IHDMI_ARC_SINK_HPP__ */
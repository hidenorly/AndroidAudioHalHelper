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

#ifndef __IBLUETOOTH_AUDIO_SINK_HPP__
#define __IBLUETOOTH_AUDIO_SINK_HPP__

#include "LPcmSink.hpp"

class IBluetoothAudioSink : public LPcmSink
{
protected:
  virtual void writePrimitive(IAudioBuffer& buf);

public:
  IBluetoothAudioSink();
  virtual ~IBluetoothAudioSink();
  virtual std::string toString(void){ return "IBluetoothAudioSink"; };
};

#endif /* __IBLUETOOTH_AUDIO_SINK_HPP__ */
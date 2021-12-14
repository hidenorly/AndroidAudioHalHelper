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

#include "IBluetoothAudioSink.hpp"

IBluetoothAudioSink::IBluetoothAudioSink():LPcmSink()
{
  mSupportedFormats.clear();
  // Bluedroid only accepts 16bit/48KHz/2ch (for SBC)
  mSupportedFormats.push_back( AudioFormat( AudioFormat::ENCODING::PCM_16BIT, AudioFormat::SAMPLING_RATE::SAMPLING_RATE_48_KHZ, AudioFormat::CHANNEL::CHANNEL_STEREO ) );
}

IBluetoothAudioSink::~IBluetoothAudioSink()
{

}

void IBluetoothAudioSink::writePrimitive(IAudioBuffer& buf)
{
  // communicate with bluetooth audio hal which expects to communicate to bluedroid for SW encoding
  // send the LPCM data to HW BT module through I2S
}

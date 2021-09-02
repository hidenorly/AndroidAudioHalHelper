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

#include "AndroidHalStreamIn.hpp"

IStreamIn::ReadPipeInfo IStreamIn::prepareForReading(uint32_t frameSize, uint32_t framesCount)
{
  return ReadPipeInfo();
}

uint32_t IStreamIn::getInputFramesLost(void)
{
  return 0;
}

PresentationPosition IStreamIn::getCapturePosition(void)
{
  return PresentationPosition();
}

HalResult IStreamIn::setGain(float gain)
{
  return HalResult::NOT_SUPPORTED;
}

AudioSource IStreamIn::getAudioSource(void)
{
  return 0;
}

std::vector<audio_microphone_characteristic_t> IStreamIn::getActiveMicrophones(void)
{
  return std::vector<audio_microphone_characteristic_t>();
}

HalResult IStreamIn::setMicrophoneDirection(audio_microphone_direction_t direction)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStreamIn::setMicrophoneFieldDimension(float zoom)
{
  return HalResult::NOT_SUPPORTED;
}

void IStreamIn::updateSinkMetadata(SinkMetadata sinkMetadata)
{

}

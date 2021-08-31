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

#include "AndroidHalStreamOut.hpp"

IStreamOut::WritePipeInfo IStreamOut::prepareForWriting(uint32_t frameSize, uint32_t framesCount)
{
  return WritePipeInfo();
}

// capability check
bool IStreamOut::supportsPause(void)
{
  return false;
}

bool IStreamOut::supportsResume(void)
{
  return false;
}

bool IStreamOut::supportsDrain(void)
{
  return false;
}

// operation
HalResult IStreamOut::pause(void)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStreamOut::resume(void)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStreamOut::drain(AudioDrain type)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStreamOut::flush(void)
{
  return HalResult::NOT_SUPPORTED;
}


HalResult IStreamOut::setCallback(IStreamOut::IStreamOutCallback callback)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStreamOut::clearCallback(void)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStreamOut::setEventCallback(IStreamOut::IStreamOutEventCallback callback)
{
  return HalResult::NOT_SUPPORTED;
}

// get status
int64_t IStreamOut::getNextWriteTimestampUsec()
{
  return 0;
}

uint32_t IStreamOut::getLatencyMsec(void)
{
  return 0;
}

uint32_t IStreamOut::getRenderPositionDspFrames(void)
{
  return 0;
}

PresentationPosition IStreamOut::getPresentationPosition(void)
{
  return PresentationPosition();
}

HalResult IStreamOut::setVolume(float left, float right)
{
  return HalResult::NOT_SUPPORTED;
}

PlaybackRate IStreamOut::getPlaybackRateParameters(void)
{
  return PlaybackRate();
}

HalResult IStreamOut::setPlaybackRateParameters(PlaybackRate playbackRate)
{
  return HalResult::NOT_SUPPORTED;
}

DualMonoMode IStreamOut::getDualMonoMode(void)
{
  return DualMonoMode();
}

HalResult IStreamOut::setDualMonoMode(DualMonoMode mode)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStreamOut::selectPresentation(int32_t presentationId, int32_t programId)
{
  return HalResult::NOT_SUPPORTED;
}


float IStreamOut::getAudioDescriptionMixLevelDb(void)
{
  return 0.0f;
}

HalResult IStreamOut::setAudioDescriptionMixLevel(float leveldB)
{
  return HalResult::NOT_SUPPORTED;
}

void IStreamOut::updateSourceMetadata(SourceMetadata sourceMetadata)
{

}

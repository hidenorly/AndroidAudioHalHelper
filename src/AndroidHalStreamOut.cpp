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
#include <algorithm>
#include "Buffer.hpp"
#include "PipeMultiThread.hpp"

void IStreamOut::AndroidAudioSource::readPrimitive(IAudioBuffer& buf)
{
  if( mDataMQ ){
    int availToRead = mDataMQ->availableToRead();
    int nBufSize = buf.getRawBufferSize();
    nBufSize = nBufSize ? nBufSize : availToRead;
    int nReadSize = std::min(nBufSize, availToRead);

    ByteBuffer rawBuf = buf.getRawBuffer();
    rawBuf.resize( nReadSize );
    mDataMQ->read( rawBuf.data(), nReadSize );
    buf.setRawBuffer( rawBuf ); // just in case. basically not be required.
  }
}

std::shared_ptr<IStreamOut::WritePipeInfo> IStreamOut::prepareForWriting(uint32_t frameSize, uint32_t framesCount)
{
  mWritePipeInfo.reset();
  mSource.reset();
  mPipe.reset();

  mWritePipeInfo = std::make_shared<WritePipeInfo>();
  mSource = std::make_shared<IStreamOut::AndroidAudioSource>( mWritePipeInfo->dataMQ );
  mPipe = std::make_shared<PipeMultiThread>();
  mPipe->attachSource( mSource );
  mPipe->attachSink( mSink );

  return mWritePipeInfo;
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
  return true;
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
  HalResult result = HalResult::INVALID_STATE;
  if( mPipe ){
    mPipe->stopAndFlush();
    result = HalResult::OK;
  }
  return result;
}

HalResult IStreamOut::flush(void)
{
  HalResult result = HalResult::INVALID_STATE;
  if( mPipe ){
    mPipe->stopAndFlush();
    result = HalResult::OK;
  }
  return result;
}


HalResult IStreamOut::setCallback(std::weak_ptr<IStreamOut::IStreamOutCallback> callback)
{
  mCallback = callback;
  return HalResult::OK;
}

HalResult IStreamOut::clearCallback(void)
{
  mCallback.reset();
  return HalResult::OK;
}

HalResult IStreamOut::setEventCallback(std::weak_ptr<IStreamOut::IStreamOutEventCallback> callback)
{
  mEventCallback = callback;
  return HalResult::OK;
}

// get status
int64_t IStreamOut::getNextWriteTimestampUsec()
{
  return 0;
}

uint32_t IStreamOut::getLatencyMsec(void)
{
  uint32_t result = 0;

  if( mPipe ){
    result = mPipe->getLatencyUSec() / 1000;
  }
  return result;
}

uint32_t IStreamOut::getRenderPositionDspFrames(void)
{
  uint32_t result = 0;

  if( mPipe ){
    std::shared_ptr<ISink> pSink = mPipe->getSinkRef();
    if( pSink ){
      long time = pSink->getPresentationTime(); // usec
      int windowSize = mPipe->getWindowSizeUsec();
      result = time ? (uint32_t)( time / (long)windowSize ) : 0;
    }
  }

  return result;
}

PresentationPosition IStreamOut::getPresentationPosition(void)
{
  PresentationPosition result;

  if( mPipe ){
    std::shared_ptr<ISink> pSink = mPipe->getSinkRef();
    if( pSink ){
      long time = pSink->getPresentationTime(); // usec
      result.timeStamp.tvSec = time / 1000000; // usec to sec
      result.timeStamp.tvNSec = (time - result.timeStamp.tvSec * 1000000) * 1000; // usec to nsec
      result.frames = getRenderPositionDspFrames();
    }
  }

  return result;
}

HalResult IStreamOut::setVolume(float left, float right)
{
  if( mPipe ){
    std::shared_ptr<ISink> pSink = mPipe->getSinkRef();
    if( pSink ){
      if( pSink->getAudioFormat().getChannels() == AudioFormat::CHANNEL::CHANNEL_STEREO ){
        Volume::CHANNEL_VOLUME chVolume;
        chVolume[AudioFormat::CH::L] = left;
        chVolume[AudioFormat::CH::R] = left;
        pSink->setVolume( chVolume );
      } else {
        pSink->setVolume( (left + right) / 2 );
      }
    }
  }

  return HalResult::OK;
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

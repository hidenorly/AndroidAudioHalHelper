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
#include "SourceSinkManager.hpp"
#include <chrono>

void IStreamOut::AndroidAudioSource::readPrimitive(IAudioBuffer& buf)
{
  if( mDataMQ ){
    int availToRead = mDataMQ->availableToRead();

    while( !availToRead ){ // TODO: check this stream is actually running or not (only wait during running)
      uint32_t efState = 0;
      mEfGroup->wait( MessageQueueFlagBits::NOT_EMPTY, &efState);
      if( efState & MessageQueueFlagBits::NOT_EMPTY ){
        availToRead = mDataMQ->availableToRead();
      }
    }

    int nBufSize = buf.getRawBufferSize();
    nBufSize = nBufSize ? nBufSize : availToRead;
    int nReadSize = std::min(nBufSize, availToRead);

    ByteBuffer rawBuf = buf.getRawBuffer();
    rawBuf.resize( nReadSize );
    mDataMQ->read( rawBuf.data(), nReadSize );
    buf.setRawBuffer( rawBuf ); // just in case. basically not be required.

    IStreamOut::WriteStatus status;
    status.replyTo = IStreamOut::WriteCommand::WRITE;
    status.retval = HalResult::OK;
    status.reply.written = nReadSize;
    if( mStatusMQ ){
      mStatusMQ->write( &status );
    }
  }
}

void IStreamOut::AndroidAudioSource::notifyDoRead(void)
{
  mEfGroup->wake( MessageQueueFlagBits::NOT_EMPTY );
}

void IStreamOut::AndroidAudioSource::notifyNextCommand(void)
{
  mEfGroup->wake( MessageQueueFlagBits::NOT_FULL );
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

HalResult IStreamOut::getDevices(std::vector<DeviceAddress>& devices)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;

  if( mPipe ){
    DeviceAddress deviceAddr = SourceSinkManager::getDeviceAddress( mPipe->getSinkRef() );
    if( !AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr).empty() ){
      devices.push_back( deviceAddr );
      result = HalResult::OK;
    }
  }

  return result;
}

HalResult IStreamOut::setDevices(std::vector<DeviceAddress> devices)
{
  HalResult result = HalResult::INVALID_ARGUMENTS;
  if( mPipe && !devices.empty() ){
    std::shared_ptr<ISink> pSink = SourceSinkManager::getSink( devices[0] );
    // TODO: if devices.size()>1 then use MultiSink and support it
    if( pSink ){
      mPipe->attachSink( pSink );
      result = HalResult::OK;
    }
  }
  return result;
}

void IStreamOut::process(void)
{
  while( mbIsRunning ){
    IStreamOut::WriteCommand wCommand;

    if( mWritePipeInfo && mWritePipeInfo->commandMQ && mWritePipeInfo->commandMQ->read( &wCommand ) ){
      IStreamOut::WriteStatus wStatus;
        wStatus.retval = HalResult::INVALID_ARGUMENTS;
        wStatus.replyTo = wCommand;
        wStatus.reply.written = 0;

      switch (wCommand) {
        case IStreamOut::WriteCommand::WRITE:
          if( mSource ){
            mSource->notifyDoRead(); // wake up the blocking
          }
          break;

        case IStreamOut::WriteCommand::GET_PRESENTATION_POSITION:
          wStatus.reply.presentationPosition = getPresentationPosition();
          break;

        case IStreamOut::WriteCommand::GET_LATENCY:
          wStatus.reply.latencyMs = getLatencyMsec();
          break;

        default:
          wStatus.retval = HalResult::NOT_SUPPORTED;
          break;
      }

      if( mWritePipeInfo->statusMQ ){
        mWritePipeInfo->statusMQ->write( &wStatus );
      }
      if( mSource ){
        mSource->notifyNextCommand();
      }
    } else {
      std::this_thread::sleep_for(std::chrono::microseconds(10000));
    }
  }
}

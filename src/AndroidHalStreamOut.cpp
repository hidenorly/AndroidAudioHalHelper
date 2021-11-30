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
#include "MultipleSink.hpp"
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

    mLastWritePts += getDeltaPtsUsecByBytes( nReadSize );

    IStreamOut::WriteStatus status;
    status.replyTo = IStreamOut::WriteCommand::WRITE;
    status.retval = HalResult::OK;
    status.reply.written = nReadSize;
    if( mStatusMQ ){
      mStatusMQ->write( &status );
    }
  }
}

long IStreamOut::AndroidAudioSource::getDeltaPtsUsecByBytes(int64_t bytes)
{
  AudioFormat format = getAudioFormat();
  int64_t nSampleByte = format.getChannelsSampleByte();
  return 1000000 * bytes / nSampleByte / format.getSamplingRate();
}


void IStreamOut::AndroidAudioSource::notifyDoRead(void)
{
  mEfGroup->wake( MessageQueueFlagBits::NOT_EMPTY );
}

void IStreamOut::AndroidAudioSource::notifyNextCommand(void)
{
  mEfGroup->wake( MessageQueueFlagBits::NOT_FULL );
}


IStreamOut::IStreamOut(AudioIoHandle ioHandle, DeviceAddress device, AudioConfig config, AudioOutputFlags flags, SourceMetadata sourceMetadata, std::shared_ptr<IStream::StreamSessionHandler> pSessionHandler, std::shared_ptr<ISink> pSink) :
  IStream(ioHandle, device, config, pSessionHandler),
  mSink(pSink),
  mOutputFlags(flags),
  mSourceMetadata(sourceMetadata),
  mAudioDescMixLevlDb(0.0f),
  mDualMonoMode(AUDIO_DUAL_MONO_MODE_OFF),
  mPresentationId(0),
  mProgramId(0),
  mLastPts(0),
  mLastWritePtsBase(0)
{

}


std::shared_ptr<IStreamOut::WritePipeInfo> IStreamOut::prepareForWriting(uint32_t frameSize, uint32_t framesCount)
{
  mWritePipeInfo.reset();
  mSource.reset();
  mPipe.reset();

  mWritePipeInfo = std::make_shared<WritePipeInfo>();
  mSource = std::make_shared<IStreamOut::AndroidAudioSource>( mWritePipeInfo->dataMQ );
  mPipe = std::make_shared<PipeMultiThread>();
  mPipe->attachSource( mSource ); // TODO: should register this as session or mixPort into SourceSinkManager, etc.
  mPipe->attachSink( mSink );

  mSource->resetWritePts();
  mLastWritePtsBase = mSource->getPresentationTime();

  return mWritePipeInfo;
}

// capability check
bool IStreamOut::supportsPause(void)
{
  return true;
}

bool IStreamOut::supportsResume(void)
{
  return true;
}

bool IStreamOut::supportsDrain(void)
{
  return true;
}

// operation
HalResult IStreamOut::pause(void)
{
  mLastPts = 0;
  if( mPipe && mPipe->isRunning() ){
    std::shared_ptr<ISink> pSink = mPipe->getSinkRef();
    mLastPts = pSink ? pSink->getPresentationTime() : 0;
    mPipe->stopAndFlush();
  }
  return HalResult::OK;
}

HalResult IStreamOut::resume(void)
{
  if( mPipe && mLastPts ){
    // TODO: May require to do as mPipe->seek(mLastPts);
    mPipe->run();
  }
  mLastPts = 0;
  return HalResult::OK;
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
  int64_t result = mLastWritePtsBase;
  if( mSource ){
    result += mSource->getWritePresentationTimeUsec();
  }
  return result;
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
      result = (time && windowSize) ? (uint32_t)( time / (long)windowSize ) : 0;
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
        chVolume[AudioFormat::CH::R] = right;
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
  return mPlaybackRate;
}

HalResult IStreamOut::setPlaybackRateParameters(PlaybackRate playbackRate)
{
  if( mPlaybackRate.speed != playbackRate.speed ){
    if( !mPlaybackRateFilter ){
      mPlaybackRateFilter = std::make_shared<PlaybackRateFilter>();
    }
    // just in case but this has side effect
    bool bRunning = false;
    if( mPipe ){
      bRunning = mPipe->isRunning();
      mPipe->stopAndFlush();
      mPipe->removeFilter( mPlaybackRateFilter );
      mPlaybackRateFilter->setPlaybackRate( playbackRate );
      mPipe->addFilterToTail( mPlaybackRateFilter );
      if( bRunning ){
        mPipe->run();
      }
    }
  }
  mPlaybackRate = playbackRate;
  // TODO
  return HalResult::OK;
}

DualMonoMode IStreamOut::getDualMonoMode(void)
{
  return mDualMonoMode;
}

HalResult IStreamOut::setDualMonoMode(DualMonoMode mode)
{
  // TODO
  if( mDualMonoMode != mode ){
    if( !mDualMonoFilter ){
      mDualMonoFilter = std::make_shared<DualMonoFilter>();
    }
    // just in case but this has side effect
    bool bRunning = false;
    if( mPipe ){
      bRunning = mPipe->isRunning();
      mPipe->stopAndFlush();
      mPipe->removeFilter( mDualMonoFilter );
      mDualMonoFilter->setDualMonoMode( mode );
      mPipe->addFilterToTail( mDualMonoFilter );
      if( bRunning ){
        mPipe->run();
      }
    }
  }
  mDualMonoMode = mode;
  // TODO
  return HalResult::OK;
}

HalResult IStreamOut::selectPresentation(int32_t presentationId, int32_t programId)
{
  mPresentationId = presentationId;
  mProgramId = programId;
  // TODO
  return HalResult::OK;
}


float IStreamOut::getAudioDescriptionMixLevelDb(void)
{
  return mAudioDescMixLevlDb;
}

HalResult IStreamOut::setAudioDescriptionMixLevel(float leveldB)
{
  mAudioDescMixLevlDb = leveldB;
  // TODO
  return HalResult::OK;
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
    std::shared_ptr<ISink> pSink;

    if( devices.size() > 1 ){
      std::shared_ptr<MultipleSink> pMultipleSink = std::make_shared<MultipleSink>();
      for( auto aDevice : devices ){
        std::shared_ptr<ISink> aSink = SourceSinkManager::getSink( aDevice );
        if( aSink ){
          pMultipleSink->attachSink( aSink, aSink->getAudioFormat().getSameChannelMapper() );
        }
      }
      pSink = pMultipleSink;
    } else {
      pSink = SourceSinkManager::getSink( devices[0] );
    }

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

HalResult IStreamOut::streamClose(void)
{
  mCallback.reset();
  mEventCallback.reset();
  mWritePipeInfo.reset();
  if( mSource ){
    mSource->resetWritePts();
  }
  mSource.reset();
  mSink.reset();

  mOutputFlags = AudioOutputFlags();
  mSourceMetadata = SourceMetadata();

  mAudioDescMixLevlDb = 0.0f;
  mPlaybackRate = PlaybackRate();
  mPlaybackRateFilter.reset();
  mDualMonoMode = DualMonoMode();
  mDualMonoFilter.reset();
  mPresentationId = 0;
  mProgramId = 0;

  mLastPts = 0;
  mLastWritePtsBase = 0;

  return IStream::streamClose();
}


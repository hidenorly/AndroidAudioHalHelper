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

void IStreamIn::AndroidAudioSink::writePrimitive(IAudioBuffer& buf)
{
  if( mDataMQ ){
    int nBufSize = buf.getRawBufferSize();
    int nCount = 0;
    while( nCount < nBufSize ){
      int availToWrite = mDataMQ->availableToWrite();
      if( nBufSize && availToWrite ){
        int nWriteSize = std::min(nBufSize-nCount, availToWrite);
        nWriteSize = ( nWriteSize >= 0 ) ? nWriteSize : 0;
        ByteBuffer rawBuf = buf.getRawBuffer();
        mDataMQ->write( rawBuf.data()+nCount, nWriteSize );
        nCount += nWriteSize;
      }
      // TODO: need to wait if nCount isn't read enough to full with buf
    }
  }
}


std::shared_ptr<IStreamIn::ReadPipeInfo> IStreamIn::prepareForReading(uint32_t frameSize, uint32_t framesCount)
{
  mReadPipeInfo.reset();
  mSource.reset();
  mPipe.reset();

  mReadPipeInfo = std::make_shared<ReadPipeInfo>();
  mSink = std::make_shared<IStreamIn::AndroidAudioSink>( mReadPipeInfo->dataMQ );
  mPipe = std::make_shared<PipeMultiThread>();
  mPipe->attachSource( mSource );
  mPipe->attachSink( mSink );  // TODO: should register this as session or mixPort into SourceSinkManager, etc.

  return mReadPipeInfo;
}

uint32_t IStreamIn::getInputFramesLost(void)
{
  return 0;
}

PresentationPosition IStreamIn::getCapturePosition(void)
{
  PresentationPosition result;

  if( mPipe ){
    std::shared_ptr<ISource> pSource = mPipe->getSourceRef();
    if( pSource ){
      long time = pSource->getPresentationTime(); // usec
      result.timeStamp.tvSec = time / 1000000; // usec to sec
      result.timeStamp.tvNSec = (time - result.timeStamp.tvSec * 1000000) * 1000; // usec to nsec


      int windowSize = mPipe->getWindowSizeUsec();
      result.frames = (time && windowSize) ? (uint32_t)( time / (long)windowSize ) : 0;
    }
  }

  return result;
}

HalResult IStreamIn::setGain(float gain)
{
  return HalResult::NOT_SUPPORTED;
}

AudioSource IStreamIn::getAudioSource(void)
{
  return AUDIO_SOURCE_DEFAULT;
}

std::vector<AudioMicrophoneCharacteristic> IStreamIn::getActiveMicrophones(void)
{
  return std::vector<AudioMicrophoneCharacteristic>();
}

HalResult IStreamIn::setMicrophoneDirection(AudioMicrophoneDirection direction)
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

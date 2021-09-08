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

#ifndef __ANDROID_HAL_STREAM_OUT_HPP__
#define __ANDROID_HAL_STREAM_OUT_HPP__

#include "AndroidHalStream.hpp"
#include <fmq/MessageQueue.h>
#include <vector>

#include <system/audio.h>
#include <stdint.h>
#include "Strategy.hpp"
#include "Pipe.hpp"

class IStreamOut : public IStream
{
public:
  class IStreamOutCallback
  {
  public:
    virtual void onWriteReady(void);
    virtual void onDrainReady(void);
    virtual void onError(void);
  };

  class IStreamOutEventCallback
  {
  public:
    virtual void onCodecFormatChanged(std::vector<uint8_t> audioMetadata);
  };

  enum WriteCommand
  {
    WRITE,
    GET_PRESENTATION_POSITION,
    GET_LATENCY
  };

  struct WriteStatus
  {
    HalResult retval;
    WriteCommand replyTo;
    union Reply {
        uint64_t written;
        PresentationPosition presentationPosition;
        uint32_t latencyMs;
    } reply;
  };

  typedef android::hardware::MessageQueue<IStreamOut::WriteCommand, android::hardware::kSynchronizedReadWrite> CommandMQ;
  typedef android::hardware::MessageQueue<IStreamOut::WriteStatus, android::hardware::kSynchronizedReadWrite> StatusMQ;
  typedef android::hardware::MessageQueue<uint8_t, android::hardware::kSynchronizedReadWrite> DataMQ;

  class WritePipeInfo
  {
  public:
    //TODO: will be update
    CommandMQ commandMQ;
    StatusMQ dataMQ;
    DataMQ statusMQ;
    //ThreadInfo threadInfo;

    WritePipeInfo(int bufferSize = 4096):commandMQ(1), dataMQ(bufferSize, true), statusMQ(1){};
    virtual ~WritePipeInfo(){};
  };

protected:
  std::weak_ptr<IStreamOutCallback> mCallback;
  std::weak_ptr<IStreamOutEventCallback> mEventCallback;
  std::shared_ptr<IPipe> mPipe;

public:
  virtual WritePipeInfo prepareForWriting(uint32_t frameSize, uint32_t framesCount);

  // capability check
  virtual bool supportsPause(void);
  virtual bool supportsResume(void);
  virtual bool supportsDrain(void);

  // operation
  virtual HalResult pause(void);
  virtual HalResult resume(void);
  virtual HalResult drain(AudioDrain type);
  virtual HalResult flush(void);

  // callback
  virtual HalResult setCallback(std::weak_ptr<IStreamOutCallback> callback);
  virtual HalResult clearCallback(void);
  virtual HalResult setEventCallback(std::weak_ptr<IStreamOutEventCallback> callback);

  // get status
  virtual int64_t getNextWriteTimestampUsec();
  virtual uint32_t getLatencyMsec(void);
  virtual uint32_t getRenderPositionDspFrames(void);
  virtual PresentationPosition getPresentationPosition(void);

  virtual HalResult setVolume(float left, float right);

  virtual PlaybackRate getPlaybackRateParameters(void);
  virtual HalResult setPlaybackRateParameters(PlaybackRate playbackRate);

  virtual DualMonoMode getDualMonoMode(void);
  virtual HalResult setDualMonoMode(DualMonoMode mode);

  virtual HalResult selectPresentation(int32_t presentationId, int32_t programId);

  virtual float getAudioDescriptionMixLevelDb(void);
  virtual HalResult setAudioDescriptionMixLevel(float leveldB);

  virtual void updateSourceMetadata(SourceMetadata sourceMetadata);
};


class StreamOutContext : public StrategyContext
{
public:
  AudioIoHandle ioHandle;
  DeviceAddress device;
  audio_config config;
  audio_output_flags_t flags;
  SourceMetadata sourceMetadata;

public:
  StreamOutContext(AudioIoHandle ioHandle, DeviceAddress device, audio_config config, audio_output_flags_t flags, SourceMetadata sourceMetadata):ioHandle(ioHandle), device(device), config(config), flags(flags), sourceMetadata(sourceMetadata){};
  virtual ~StreamOutContext(){};
};


#endif /* __ANDROID_HAL_STREAM_OUT_HPP__ */

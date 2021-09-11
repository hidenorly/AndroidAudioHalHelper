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

#ifndef __ANDROID_HAL_STREAM_IN_HPP__
#define __ANDROID_HAL_STREAM_IN_HPP__

#include "AndroidHalStream.hpp"
#include <fmq/MessageQueue.h>
#include <vector>

class IStreamIn : public IStream
{
public:
  enum ReadCommand
  {
    READ,
    GET_CAPTURE_POSITION
  };

  struct ReadParameters
  {
    ReadCommand command;
    union Params
    {
      uint64_t read;
    } params;
  };

  struct ReadStatus
  {
    HalResult retval;
    ReadCommand replyTo;
    union Reply
    {
      uint64_t read;
      struct CapturePosition
      {
        uint64_t frames;
        uint64_t time;
      } capturePosition;
    } reply;
  };

  typedef android::hardware::MessageQueue<IStreamIn::ReadCommand, android::hardware::kSynchronizedReadWrite> CommandMQ;
  typedef android::hardware::MessageQueue<IStreamIn::ReadStatus, android::hardware::kSynchronizedReadWrite> StatusMQ;
  typedef android::hardware::MessageQueue<uint8_t, android::hardware::kSynchronizedReadWrite> DataMQ;

  class ReadPipeInfo
  {
  public:
    //TODO: will be update
    CommandMQ commandMQ;
    StatusMQ dataMQ;
    DataMQ statusMQ;
    //ThreadInfo threadInfo;

    ReadPipeInfo(int bufferSize = 4096):commandMQ(1), dataMQ(bufferSize, true), statusMQ(1){};
    virtual ~ReadPipeInfo(){};
  };
protected:
  std::shared_ptr<ReadPipeInfo> mReadPipeInfo;
  std::shared_ptr<ISource> mSource;
  audio_input_flags_t mInputFlags;
  SinkMetadata mSinkMetadata;

public:
  IStreamIn(AudioIoHandle ioHandle = 0, DeviceAddress device=DeviceAddress(), audio_config config={0}, audio_input_flags_t flags=AUDIO_INPUT_FLAG_NONE, SinkMetadata sinkMetadata=SinkMetadata(), std::shared_ptr<StreamSessionHandler> pSessionHandler = nullptr, std::shared_ptr<ISource> pSource = nullptr) : IStream(ioHandle, device, config, pSessionHandler), mSource(pSource), mInputFlags(flags), mSinkMetadata(sinkMetadata){};
  virtual ~IStreamIn(){};

  virtual ReadPipeInfo prepareForReading(uint32_t frameSize, uint32_t framesCount);

  virtual uint32_t getInputFramesLost(void);
  virtual PresentationPosition getCapturePosition(void);

  virtual HalResult setGain(float gain);

  virtual AudioSource getAudioSource(void);

  virtual std::vector<audio_microphone_characteristic_t> getActiveMicrophones(void);
  virtual HalResult setMicrophoneDirection(audio_microphone_direction_t direction);
  virtual HalResult setMicrophoneFieldDimension(float zoom);

  virtual void updateSinkMetadata(SinkMetadata sinkMetadata);
};

#endif /* __ANDROID_HAL_STREAM_IN_HPP__ */


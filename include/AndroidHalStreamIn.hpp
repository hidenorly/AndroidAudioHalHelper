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
#include "Source.hpp"
#include "Sink.hpp"
#include "Strategy.hpp"

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
    std::shared_ptr<CommandMQ> commandMQ;
    std::shared_ptr<DataMQ> dataMQ;
    std::shared_ptr<StatusMQ> statusMQ;
    //ThreadInfo threadInfo;

    ReadPipeInfo(
      std::shared_ptr<CommandMQ> commandMQ = std::make_shared<CommandMQ>(1),
      std::shared_ptr<DataMQ> dataMQ = std::make_shared<DataMQ>(1),
      std::shared_ptr<StatusMQ> statusMQ = std::make_shared<StatusMQ>(4096, true)) : commandMQ(commandMQ), dataMQ(dataMQ), statusMQ(statusMQ){};
    virtual ~ReadPipeInfo(){};
  };

  class AndroidAudioSink : public ISink
  {
  protected:
    std::shared_ptr<DataMQ> mDataMQ;
    AudioFormat mFormat;

  protected:
    void setAudioFormatPrimitive(AudioFormat format){ mFormat = format; };
    virtual void writePrimitive(IAudioBuffer& buf);

  public:
    AndroidAudioSink(std::shared_ptr<DataMQ> dataMQ = nullptr) : mDataMQ(dataMQ){};
    virtual ~AndroidAudioSink(){
      mDataMQ.reset();
    };
    virtual bool isAvailableFormat(AudioFormat format){ return true; };
    virtual std::string toString(void){ return "AndroidAudioSink"; };
    virtual void dump(void){};
    virtual AudioFormat getAudioFormat(void){ return mFormat; };
  };

protected:
  std::shared_ptr<ReadPipeInfo> mReadPipeInfo;
  std::shared_ptr<ISource> mSource;
  std::shared_ptr<AndroidAudioSink> mSink;
  AudioInputFlags mInputFlags;
  SinkMetadata mSinkMetadata;

public:
  IStreamIn(AudioIoHandle ioHandle = 0, DeviceAddress device=DeviceAddress(), AudioConfig config={0}, AudioInputFlags flags=AUDIO_INPUT_FLAG_NONE, SinkMetadata sinkMetadata=SinkMetadata(), std::shared_ptr<StreamSessionHandler> pSessionHandler = nullptr, std::shared_ptr<ISource> pSource = nullptr) : IStream(ioHandle, device, config, pSessionHandler), mSource(pSource), mInputFlags(flags), mSinkMetadata(sinkMetadata){};
  virtual ~IStreamIn(){};

  virtual std::shared_ptr<ReadPipeInfo> prepareForReading(uint32_t frameSize, uint32_t framesCount);

  virtual uint32_t getInputFramesLost(void);
  virtual PresentationPosition getCapturePosition(void);

  virtual HalResult setGain(float gain);

  virtual AudioSource getAudioSource(void);

  virtual std::vector<AudioMicrophoneCharacteristic> getActiveMicrophones(void);
  virtual HalResult setMicrophoneDirection(AudioMicrophoneDirection direction);
  virtual HalResult setMicrophoneFieldDimension(float zoom);

  virtual std::vector<DeviceAddress> getDevices(void);
  virtual HalResult setDevices(std::vector<DeviceAddress> devices);

  virtual void updateSinkMetadata(SinkMetadata sinkMetadata);
};

class StreamInContext : public StrategyContext
{
public:
  AudioIoHandle ioHandle;
  DeviceAddress device;
  AudioConfig config;
  AudioInputFlags flags;
  SinkMetadata sinkMetadata;

public:
  StreamInContext(AudioIoHandle ioHandle, DeviceAddress device, AudioConfig config, AudioInputFlags flags, SinkMetadata sinkMetadata):ioHandle(ioHandle), device(device), config(config), flags(flags), sinkMetadata(sinkMetadata){};
  virtual ~StreamInContext(){};
};

#endif /* __ANDROID_HAL_STREAM_IN_HPP__ */

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

#include "AndroidHalStream.hpp"
#include "AudioFormatHelper.hpp"


IStream::~IStream()
{
  if( mSessionHandler ){
    mSessionHandler->onCloseStream( shared_from_this() );
  }
}


uint64_t IStream::getFrameSize(void)
{
  return 0;
}

uint64_t IStream::getFrameCount(void)
{
  return 0;
}

uint64_t IStream::getBufferSize(void)
{
  return 0;
}


uint32_t IStream::getSampleRate(void)
{
  return AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT;
}

std::vector<uint32_t> IStream::getSupportedSampleRates(audio_format_t androidEncoding)
{
  return std::vector<uint32_t>({AudioFormat::SAMPLING_RATE::SAMPLING_RATE_DEFAULT});
}

HalResult IStream::setSampleRate(uint32_t sampleRateHz)
{
  return HalResult::NOT_SUPPORTED;
}


audio_channel_mask_t IStream::getChannelMask(void)
{
  return (audio_channel_mask_t)AndroidFormatHelper::getAndroidChannelFromChannel( AudioFormat::CHANNEL::CHANNEL_DEFAULT );
}

std::vector<audio_channel_mask_t> IStream::getSupportedChannelMasks(AudioFormat format)
{
  return std::vector<audio_channel_mask_t>({ getChannelMask() });
}

HalResult IStream::setChannelMask(audio_channel_mask_t mask)
{
  return HalResult::NOT_SUPPORTED;
}


audio_format_t IStream::getFormat(void)
{
  return AndroidFormatHelper::getAndroidEncodingFromEncoding( AudioFormat::ENCODING::ENCODING_DEFAULT );
}

std::vector<audio_format_t> IStream::getSupportedFormats(void)
{
  return std::vector<audio_format_t>({ getFormat() });
}

HalResult IStream::setFormat(audio_format_t format)
{
  return HalResult::NOT_SUPPORTED;
}


AudioFormat IStream::getAudioProperties(void)
{
  return AudioFormat();
}


HalResult IStream::addEffect(uint64_t effectId)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStream::removeEffect(uint64_t effectId)
{
  return HalResult::NOT_SUPPORTED;
}


HalResult IStream::standby(void)
{
  return HalResult::NOT_SUPPORTED;
}


HalResult IStream::getDevices(std::vector<DeviceAddress>& devices)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStream::setDevices(std::vector<DeviceAddress> devices)
{
  return HalResult::NOT_SUPPORTED;
}


HalResult IStream::setHwAvSync(AudioHwSync hwAvSync)
{
  return HalResult::NOT_SUPPORTED;
}


HalResult IStream::getParameters(std::vector<std::string> keys, std::vector<ParameterValue>& values)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStream::setParameters(std::vector<ParameterValue> values)
{
  return HalResult::NOT_SUPPORTED;
}


HalResult IStream::start(void)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStream::stop(void)
{
  return HalResult::NOT_SUPPORTED;
}

HalResult IStream::close(void)
{
  return HalResult::NOT_SUPPORTED;
}

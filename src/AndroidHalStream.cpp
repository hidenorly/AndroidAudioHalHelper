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
#include <set>


IStream::~IStream()
{
  if( mSessionHandler ){
    mSessionHandler->onCloseStream( shared_from_this() );
  }
}


audio_config IStream::getSuggestedConfig(void)
{
  return AndroidFormatHelper::getAndroidAudioConfigFromAudioFormat( mPipe ? mPipe->getFilterAudioFormat() : AudioFormat() );
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

AudioFormat IStream::getPipeAudioFormat(void)
{
  AudioFormat format;
  if( mPipe ){
    std::shared_ptr<ISink> pSink = mPipe->getSinkRef();
    if( pSink ){
      format = pSink->getAudioFormat();
    }
  }
  return format;
}


std::vector<AudioFormat> IStream::getPipeSupportedAudioFormats(void)
{
  std::vector<AudioFormat> result;
  if( mPipe ){
    std::shared_ptr<ISink> pSink = mPipe->getSinkRef();
    std::vector<AudioFormat> sinkFormats;
    if( pSink ){
      sinkFormats = pSink->getSupportedAudioFormats();
    }

    std::shared_ptr<ISource> pSource = mPipe->getSourceRef();
    std::vector<AudioFormat> sourceFormats;
    if( pSource ){
      sourceFormats = pSource->getSupportedAudioFormats();
    }
    std::shared_ptr<AudioBase> pCommon;
    if( pSource ) pCommon = pSource;
    if( pSink ) pCommon = pSink;

    if( pCommon ){
      result = pCommon->audioFormatOpOR( sinkFormats, sourceFormats );
    }
  }

  return result;
}


uint32_t IStream::getSampleRate(void)
{
  return getPipeAudioFormat().getSamplingRate();
}

std::vector<uint32_t> IStream::getSupportedSampleRates(audio_format_t androidEncoding)
{
  std::vector<uint32_t> result;
  AudioFormat::ENCODING afwEncoding = AndroidFormatHelper::getEncodingFromAndroidEncoding(androidEncoding);

  auto&& supportedFormats = getPipeSupportedAudioFormats();
  for( auto& aFormat : supportedFormats ){
    if( aFormat.getEncoding() == afwEncoding ){
      result.push_back( aFormat.getSamplingRate() );
    }
  }

  return result;
}

HalResult IStream::setSampleRate(uint32_t sampleRateHz)
{
  HalResult result = HalResult::NOT_INITIALIZED;
  if( mPipe ){
    AudioFormat format = getPipeAudioFormat();
    AudioFormat nextFormat = AudioFormat( format.getEncoding(), sampleRateHz, format.getChannels() );
    result = mPipe->getSinkRef()->setAudioFormat( nextFormat ) ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
  }
  return result;
}


audio_channel_mask_t IStream::getChannelMask(void)
{
  return AndroidFormatHelper::getAndroidChannelFromChannel( getPipeAudioFormat().getChannels() );
}

std::vector<audio_channel_mask_t> IStream::getSupportedChannelMasks(audio_format_t androidEncoding)
{
  std::vector<audio_channel_mask_t> result;
  AudioFormat::ENCODING afwEncoding = AndroidFormatHelper::getEncodingFromAndroidEncoding(androidEncoding);

  auto&& supportedFormats = getPipeSupportedAudioFormats();
  for( auto& aFormat : supportedFormats ){
    if( aFormat.getEncoding() == afwEncoding ){
      result.push_back( AndroidFormatHelper::getAndroidChannelFromChannel( aFormat.getChannels() ) );
    }
  }

  return result;
}

HalResult IStream::setChannelMask(audio_channel_mask_t mask)
{
  HalResult result = HalResult::NOT_INITIALIZED;
  if( mPipe ){
    AudioFormat format = getPipeAudioFormat();
    AudioFormat nextFormat = AudioFormat( format.getEncoding(), format.getSamplingRate(), AndroidFormatHelper::getChannelFromAndroidChannel( mask ) );
    result = mPipe->getSinkRef()->setAudioFormat( nextFormat ) ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
  }
  return result;
}


audio_format_t IStream::getFormat(void)
{
  return AndroidFormatHelper::getAndroidEncodingFromEncoding( getPipeAudioFormat().getEncoding() );
}


std::vector<audio_format_t> IStream::getSupportedFormats(void)
{
  std::vector<audio_format_t> result;
  std::set<AudioFormat::ENCODING> encodings;

  auto&& supportedFormats = getPipeSupportedAudioFormats();
  for( auto& aFormat : supportedFormats ){
    encodings.insert( aFormat.getEncoding() );
  }
  for( auto& anEncoding : encodings ){
      result.push_back( AndroidFormatHelper::getAndroidEncodingFromEncoding( anEncoding ) );
  }

  return result;
}

HalResult IStream::setFormat(audio_format_t androidFormatEncoding)
{
  HalResult result = HalResult::NOT_INITIALIZED;
  if( mPipe ){
    AudioFormat format = getPipeAudioFormat();
    AudioFormat nextFormat = AudioFormat( AndroidFormatHelper::getEncodingFromAndroidEncoding(androidFormatEncoding), format.getSamplingRate(), format.getChannels() );
    result = mPipe->getSinkRef()->setAudioFormat( nextFormat ) ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
  }
  return result;
}


AudioFormat IStream::getAudioProperties(void)
{
  return getPipeAudioFormat();
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
  HalResult result = HalResult::NOT_INITIALIZED;

  if( mPipe ){
    result = HalResult::OK;
    mPipe->run();
  }

  return result;
}

HalResult IStream::stop(void)
{
  HalResult result = HalResult::NOT_INITIALIZED;

  if( mPipe ){
    result = HalResult::OK;
    mPipe->stopAndFlush();
  }

  return result;
}

HalResult IStream::close(void)
{
  HalResult result = HalResult::NOT_INITIALIZED;

  if( mPipe ){
    result = HalResult::OK;
    mPipe->stopAndFlush();
    mPipe.reset();
    if( mSessionHandler ){
      mSessionHandler->onCloseStream( shared_from_this() );
      mSessionHandler.reset();
    }
  }

  return result;
}

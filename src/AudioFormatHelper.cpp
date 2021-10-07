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

#include "AudioFormatHelper.hpp"

// --- encoding
struct AudioEncodingConversionTable
{
public:
  AudioFormat::ENCODING afwEncoding;
  AndroidAudioFormat androidEncoding;

  AudioEncodingConversionTable(AudioFormat::ENCODING afwEncoding, AndroidAudioFormat androidEncoding) : afwEncoding(afwEncoding), androidEncoding(androidEncoding){};
};

AudioEncodingConversionTable* getAudioEncodingConversionTable(void)
{
  static AudioEncodingConversionTable conversionTable[]=
  {
    AudioEncodingConversionTable(AudioFormat::ENCODING::ENCODING_DEFAULT,       AUDIO_FORMAT_DEFAULT),

    AudioEncodingConversionTable(AudioFormat::ENCODING::PCM_8BIT,               AUDIO_FORMAT_PCM_8_BIT),
    AudioEncodingConversionTable(AudioFormat::ENCODING::PCM_16BIT,              AUDIO_FORMAT_PCM_16_BIT),
    AudioEncodingConversionTable(AudioFormat::ENCODING::PCM_24BIT_PACKED,       AUDIO_FORMAT_PCM_24_BIT_PACKED),
    AudioEncodingConversionTable(AudioFormat::ENCODING::PCM_32BIT,              AUDIO_FORMAT_PCM_32_BIT),
    AudioEncodingConversionTable(AudioFormat::ENCODING::PCM_FLOAT,              AUDIO_FORMAT_PCM_FLOAT),

    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_MP3,         AUDIO_FORMAT_MP3),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_MP2,         AUDIO_FORMAT_MP2),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_MAIN),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_LC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_SSR),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_LTP),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_HE_V1),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_HE_V2),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_SCALABLE),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_ERLC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_LD),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_ELD),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC_XHE),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_LATM,    AUDIO_FORMAT_AAC_LATM),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_LATM,    AUDIO_FORMAT_AAC_LATM_LC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_LATM,    AUDIO_FORMAT_AAC_LATM_HE_V1),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_LATM,    AUDIO_FORMAT_AAC_LATM_HE_V2),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_MAIN),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_LC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_SSR),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_LTP),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_HE_V1),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_HE_V2),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_SCALABLE),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_ERLC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_LD),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_ELD),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS_XHE),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADIF,    AUDIO_FORMAT_AAC_ADIF),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_HE_AAC_V1,   AUDIO_FORMAT_HE_AAC_V1),  
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_HE_AAC_V2,   AUDIO_FORMAT_HE_AAC_V2),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_ALAC,        AUDIO_FORMAT_ALAC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_FLAC,        AUDIO_FORMAT_FLAC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_DSD,         AUDIO_FORMAT_DSD),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_VORBIS,      AUDIO_FORMAT_VORBIS),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_OPUS,        AUDIO_FORMAT_OPUS),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_CELT,        AUDIO_FORMAT_CELT),

    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AC3,         AUDIO_FORMAT_AC3),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_E_AC3,       AUDIO_FORMAT_E_AC3),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_E_AC3,       AUDIO_FORMAT_E_AC3_JOC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_AC4,         AUDIO_FORMAT_AC4),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_DOLBY_TRUEHD,AUDIO_FORMAT_DOLBY_TRUEHD),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_MAT,         AUDIO_FORMAT_MAT),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_MAT,         AUDIO_FORMAT_MAT_1_0),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_MAT,         AUDIO_FORMAT_MAT_2_0),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_MAT,         AUDIO_FORMAT_MAT_2_1),

    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_DTS,         AUDIO_FORMAT_DTS),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_DTS_HD,      AUDIO_FORMAT_DTS_HD),

    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_WMA,         AUDIO_FORMAT_WMA),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_WMA_PRO,     AUDIO_FORMAT_WMA_PRO),

    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_SBC,         AUDIO_FORMAT_SBC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_APTX,        AUDIO_FORMAT_APTX),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_APTX_HD,     AUDIO_FORMAT_APTX_HD),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_LDAC,        AUDIO_FORMAT_LDAC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_LHDC,        AUDIO_FORMAT_LHDC),
    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_LHDC_LL,     AUDIO_FORMAT_LHDC_LL),
//    AudioEncodingConversionTable(AudioFormat::ENCODING::COMPRESSED_LC3,         AUDIO_FORMAT_INVALID),  

    AudioEncodingConversionTable(AudioFormat::ENCODING::PCM_UNKNOWN,            AUDIO_FORMAT_INVALID)
  };

  return conversionTable;
}

AudioFormat::ENCODING AndroidFormatHelper::getEncodingFromAndroidEncoding(AndroidAudioFormat androidEncoding)
{
  AudioFormat::ENCODING result = AudioFormat::ENCODING::ENCODING_DEFAULT;
  AudioEncodingConversionTable* pTable = getAudioEncodingConversionTable();

  for(int i=0; (pTable[i].afwEncoding!=AudioFormat::ENCODING::PCM_UNKNOWN) && (pTable[i].androidEncoding!=AUDIO_FORMAT_INVALID); i++){
    if( pTable[i].androidEncoding == androidEncoding ){
      result = pTable[i].afwEncoding;
      break;
    }
  }

  return result;
}

AndroidAudioFormat AndroidFormatHelper::getAndroidEncodingFromEncoding(AudioFormat::ENCODING afwEncoding)
{
  AndroidAudioFormat result = AUDIO_FORMAT_PCM_16_BIT;
  AudioEncodingConversionTable* pTable = getAudioEncodingConversionTable();

  for(int i=0; (pTable[i].afwEncoding!=AudioFormat::ENCODING::PCM_UNKNOWN) && (pTable[i].androidEncoding!=AUDIO_FORMAT_INVALID); i++){
    if( pTable[i].afwEncoding == afwEncoding ){
      result = pTable[i].androidEncoding;
      break;
    }
  }

  return result;
}

// --- channel
struct AudioChannelConversionTable
{
public:
  AudioFormat::CHANNEL afwChannel;
  AudioChannelMask androidChannel;

  AudioChannelConversionTable(AudioFormat::CHANNEL afwChannel, AudioChannelMask androidChannel) : afwChannel(afwChannel), androidChannel(androidChannel){};
};

AudioChannelConversionTable* getAudioChannelConversionTable(void)
{
  static AudioChannelConversionTable conversionTable[]=
  {
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_DEFAULT,          AUDIO_CHANNEL_OUT_STEREO),
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_MONO,             AUDIO_CHANNEL_OUT_MONO),
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_STEREO,           AUDIO_CHANNEL_OUT_STEREO),
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_2_1CH,            AUDIO_CHANNEL_OUT_2POINT1),
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_4CH,              AUDIO_CHANNEL_OUT_QUAD),
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_5CH,              AUDIO_CHANNEL_OUT_PENTA),
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_5_1CH,            AUDIO_CHANNEL_OUT_5POINT1),
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_5_1_2CH,          AUDIO_CHANNEL_OUT_5POINT1POINT2),
//  AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_5_0_2CH,          AUDIO_CHANNEL_OUT_PENTA | AUDIO_CHANNEL_OUT_TOP_SIDE_LEFT | AUDIO_CHANNEL_OUT_TOP_SIDE_RIGHT),  // Android S cannot support 5.0.2
    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_7_1CH,            AUDIO_CHANNEL_OUT_7POINT1),

    AudioChannelConversionTable(AudioFormat::CHANNEL::CHANNEL_UNKNOWN,          AUDIO_CHANNEL_OUT_STEREO)
  };

  return conversionTable;
}

AudioFormat::CHANNEL AndroidFormatHelper::getChannelFromAndroidChannel(AudioChannelMask androidChannel)
{
  AudioFormat::CHANNEL result = AudioFormat::CHANNEL::CHANNEL_DEFAULT;
  AudioChannelConversionTable* pTable = getAudioChannelConversionTable();

  for(int i=0; (pTable[i].afwChannel!=AudioFormat::CHANNEL::CHANNEL_UNKNOWN) && (pTable[i].androidChannel!=AUDIO_CHANNEL_OUT_STEREO); i++){
    if( pTable[i].androidChannel == androidChannel ){
      result = pTable[i].afwChannel;
      break;
    }
  }

  return result;
}

AudioChannelMask AndroidFormatHelper::getAndroidChannelFromChannel(AudioFormat::CHANNEL afwChannel)
{
  assert( afwChannel != AudioFormat::CHANNEL::CHANNEL_5_0_2CH ); // Android S cannot support 5.0.2ch
  AudioChannelMask result = AUDIO_CHANNEL_OUT_STEREO;
  AudioChannelConversionTable* pTable = getAudioChannelConversionTable();

  for(int i=0; (pTable[i].afwChannel!=AudioFormat::CHANNEL::CHANNEL_UNKNOWN) && (pTable[i].androidChannel!=AUDIO_CHANNEL_OUT_STEREO); i++){
    if( pTable[i].afwChannel == afwChannel ){
      result = pTable[i].androidChannel;
      break;
    }
  }

  return result;
}



AudioConfig AndroidFormatHelper::getAndroidAudioConfigFromAudioFormat(AudioFormat afwFormat)
{
  AudioConfig result;

  result.sample_rate = afwFormat.getSamplingRate();
  result.channel_mask = getAndroidChannelFromChannel( afwFormat.getChannels() );
  result.format = getAndroidEncodingFromEncoding( afwFormat.getEncoding() );
  result.offload_info = {0};
  result.frame_count = 0;

  return result;
}

AudioFormat AndroidFormatHelper::getAudioFormatFromAndroidPortConfig(const AudioPortConfig& config)
{
  return AudioFormat( getEncodingFromAndroidEncoding( config.format ), config.sample_rate, getChannelFromAndroidChannel( config.channel_mask ) );
}

AudioFormat AndroidFormatHelper::getAudioFormatFromAndroidAudioConfig(const AudioConfig& config)
{
  return AudioFormat( getEncodingFromAndroidEncoding( config.format ), config.sample_rate, getChannelFromAndroidChannel( config.channel_mask ) );
}

AudioFormat AndroidFormatHelper::getAudioFormatFromAndroidEffectConfig(const EffectBufferConfig& effectConfig)
{
  AudioFormat result(
    getEncodingFromAndroidEncoding(effectConfig.format),
    effectConfig.samplingRateHz,
    getChannelFromAndroidChannel( effectConfig.channels ) );
  return result;
}


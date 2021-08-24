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
#include <vector>

struct AudioFormatConversionTable
{
public:
  AudioFormat::ENCODING afwEncoding;
  audio_format_t androidEncoding;

  AudioFormatConversionTable(AudioFormat::ENCODING afwEncoding, audio_format_t androidEncoding) : afwEncoding(afwEncoding), androidEncoding(androidEncoding){};
};


AudioFormatConversionTable* getAudioFormatConversionTable(void)
{
  static AudioFormatConversionTable conversionTable[]=
  {
    AudioFormatConversionTable(AudioFormat::ENCODING::ENCODING_DEFAULT,       AUDIO_FORMAT_DEFAULT),

    AudioFormatConversionTable(AudioFormat::ENCODING::PCM_8BIT,               AUDIO_FORMAT_PCM_8_BIT),
    AudioFormatConversionTable(AudioFormat::ENCODING::PCM_16BIT,              AUDIO_FORMAT_PCM_16_BIT),
    AudioFormatConversionTable(AudioFormat::ENCODING::PCM_24BIT_PACKED,       AUDIO_FORMAT_PCM_24_BIT_PACKED),
    AudioFormatConversionTable(AudioFormat::ENCODING::PCM_32BIT,              AUDIO_FORMAT_PCM_32_BIT),
    AudioFormatConversionTable(AudioFormat::ENCODING::PCM_FLOAT,              AUDIO_FORMAT_PCM_FLOAT),

    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_MP3,         AUDIO_FORMAT_MP3),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_MP2,         AUDIO_FORMAT_MP2),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC,         AUDIO_FORMAT_AAC),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_LATM,    AUDIO_FORMAT_AAC_LATM),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADTS,    AUDIO_FORMAT_AAC_ADTS),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_AAC_ADIF,    AUDIO_FORMAT_AAC_ADIF),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_HE_AAC_V1,   AUDIO_FORMAT_HE_AAC_V1),  
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_HE_AAC_V2,   AUDIO_FORMAT_HE_AAC_V2),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_ALAC,        AUDIO_FORMAT_ALAC),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_FLAC,        AUDIO_FORMAT_FLAC),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_DSD,         AUDIO_FORMAT_DSD),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_VORBIS,      AUDIO_FORMAT_VORBIS),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_OPUS,        AUDIO_FORMAT_OPUS),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_CELT,        AUDIO_FORMAT_CELT),

    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_AC3,         AUDIO_FORMAT_AC3),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_E_AC3,       AUDIO_FORMAT_E_AC3),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_AC4,         AUDIO_FORMAT_AC4),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_DOLBY_TRUEHD,AUDIO_FORMAT_DOLBY_TRUEHD),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_MAT,         AUDIO_FORMAT_MAT),

    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_DTS,         AUDIO_FORMAT_DTS),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_DTS_HD,      AUDIO_FORMAT_DTS_HD),

    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_WMA,         AUDIO_FORMAT_WMA),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_WMA_PRO,     AUDIO_FORMAT_WMA_PRO),

    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_SBC,         AUDIO_FORMAT_SBC),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_APTX,        AUDIO_FORMAT_APTX),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_APTX_HD,     AUDIO_FORMAT_APTX_HD),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_LDAC,        AUDIO_FORMAT_LDAC),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_LHDC,        AUDIO_FORMAT_LHDC),
    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_LHDC_LL,     AUDIO_FORMAT_LHDC_LL),
//    AudioFormatConversionTable(AudioFormat::ENCODING::COMPRESSED_LC3,         AUDIO_FORMAT_INVALID),  

    AudioFormatConversionTable(AudioFormat::ENCODING::PCM_UNKNOWN,            AUDIO_FORMAT_INVALID)
  };

  return conversionTable;
}

AudioFormat::ENCODING AndroidEncodingHelper::getEncodingFromAndroidEncoding(audio_format_t androidEncoding)
{
  AudioFormat::ENCODING result = AudioFormat::ENCODING::ENCODING_DEFAULT;
  AudioFormatConversionTable* pTable = getAudioFormatConversionTable();

  for(int i=0; (pTable[i].afwEncoding!=AudioFormat::ENCODING::PCM_UNKNOWN) && (pTable[i].androidEncoding!=AUDIO_FORMAT_INVALID); i++){
    if( pTable[i].androidEncoding == androidEncoding ){
      result = pTable[i].afwEncoding;
      break;
    }
  }

  return result;
}

audio_format_t AndroidEncodingHelper::getAndroidEncodingFromEncoding(AudioFormat::ENCODING afwEncoding)
{
  audio_format_t result = AUDIO_FORMAT_PCM_16_BIT;
  AudioFormatConversionTable* pTable = getAudioFormatConversionTable();

  for(int i=0; (pTable[i].afwEncoding!=AudioFormat::ENCODING::PCM_UNKNOWN) && (pTable[i].androidEncoding!=AUDIO_FORMAT_INVALID); i++){
    if( pTable[i].afwEncoding == afwEncoding ){
      result = pTable[i].androidEncoding;
      break;
    }
  }

  return result;
}

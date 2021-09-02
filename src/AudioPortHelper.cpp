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

#include "AudioPortHelper.hpp"
#include "AudioFormatHelper.hpp"
#include "Sink.hpp"
#include <vector>
#include <set>
#include <functional>

void AndroidAudioPortHelper::getAndroidPortFromSourceSink(audio_port* pOutAudioPort, std::shared_ptr<ISourceSinkCommon> pSourceSink, std::string address, audio_module_handle_t hwModule, int androidAudioDeviceType)
{
  if( pOutAudioPort && pSourceSink ){
    pOutAudioPort->id = std::hash<std::string>()( pSourceSink->toString() );
    if( std::dynamic_pointer_cast<ISink>( pSourceSink ) ){
      pOutAudioPort->role = AUDIO_PORT_ROLE_SINK;
    } else {
      pOutAudioPort->role = AUDIO_PORT_ROLE_SOURCE;
      // TODO : e.g. Mic source is Ok but Source might be Android mix port, etc. then need to check.
    }
    pOutAudioPort->type = AUDIO_PORT_TYPE_DEVICE;
    strncpy( pOutAudioPort->name, pSourceSink->toString().c_str(), AUDIO_PORT_MAX_NAME_LEN );

    std::vector supportedAudioFormats = pSourceSink->getSupportedAudioFormats();
    std::set<int> samplingRates;
    std::set<int> channels;
    std::set<audio_format_t> encodings;
    for( auto& anAudioFormat : supportedAudioFormats ){
      samplingRates.insert( anAudioFormat.getSamplingRate() );
      channels.insert( AndroidFormatHelper::getAndroidChannelFromChannel( anAudioFormat.getChannels() ) );
      encodings.insert( AndroidFormatHelper::getAndroidEncodingFromEncoding( anAudioFormat.getEncoding() ) );
    }

    // set supported sampling rates
    int i=0;
    pOutAudioPort->num_sample_rates = samplingRates.size();
    assert( pOutAudioPort->num_sample_rates < AUDIO_PORT_MAX_SAMPLING_RATES );
    for( auto& aSamplingRate : samplingRates ){
        pOutAudioPort->sample_rates[i++] = aSamplingRate;
    }

    // set supported channels
    i = 0;
    pOutAudioPort->num_channel_masks = channels.size();
    assert( pOutAudioPort->num_channel_masks < AUDIO_PORT_MAX_CHANNEL_MASKS );
    for( auto& aChannel : channels ){
        pOutAudioPort->channel_masks[i++] = aChannel;
    }

    // set supported formats (encoding)
    i = 0;
    pOutAudioPort->num_formats = encodings.size();
    assert( pOutAudioPort->num_formats < AUDIO_PORT_MAX_FORMATS );
    for( auto& anEncoding : encodings ){
        pOutAudioPort->formats[i++] = anEncoding;
    }

    // set supported gain
    i = 0;
    pOutAudioPort->num_gains = 0;
    assert( pOutAudioPort->num_gains < AUDIO_PORT_MAX_GAINS );
    // TODO : struct audio_gain gains[AUDIO_PORT_MAX_GAINS] are expected to be configured.

    // set active audio_port_config
    getAndroidPortConfigFromSourceSink( &pOutAudioPort->active_config, pSourceSink, address, hwModule, androidAudioDeviceType);

    pOutAudioPort->ext.device.hw_module = hwModule;
    pOutAudioPort->ext.device.type = androidAudioDeviceType;
    strncpy( pOutAudioPort->ext.device.address, address.c_str(), AUDIO_DEVICE_MAX_ADDRESS_LEN );
  }
}

void AndroidAudioPortHelper::getAndroidPortConfigFromSourceSink(audio_port_config* pOutAudioPort, std::shared_ptr<ISourceSinkCommon> pSourceSink, std::string address, audio_module_handle_t hwModule, int androidAudioDeviceType)
{
  if( pOutAudioPort && pSourceSink ){
    pOutAudioPort->id = std::hash<std::string>()( pSourceSink->toString() );
    if( std::dynamic_pointer_cast<ISink>( pSourceSink ) ){
      pOutAudioPort->role = AUDIO_PORT_ROLE_SINK;
    } else {
      pOutAudioPort->role = AUDIO_PORT_ROLE_SOURCE;
      // TODO : e.g. Mic source is Ok but Source might be Android mix port, etc. then need to check.
    }
    pOutAudioPort->type = AUDIO_PORT_TYPE_DEVICE;

    pOutAudioPort->config_mask = AUDIO_PORT_CONFIG_SAMPLE_RATE | AUDIO_PORT_CONFIG_CHANNEL_MASK | AUDIO_PORT_CONFIG_FORMAT;

    AudioFormat currentFormat = pSourceSink->getAudioFormat();
    pOutAudioPort->sample_rate = currentFormat.getSamplingRate();
    pOutAudioPort->channel_mask = AndroidFormatHelper::getAndroidChannelFromChannel( currentFormat.getChannels() );
    pOutAudioPort->format = AndroidFormatHelper::getAndroidEncodingFromEncoding( currentFormat.getEncoding() );
    memset( &pOutAudioPort->gain, 0, sizeof( struct audio_gain_config) );

    pOutAudioPort->ext.device.hw_module = hwModule;
    pOutAudioPort->ext.device.type = androidAudioDeviceType;
    strncpy( pOutAudioPort->ext.device.address, address.c_str(), AUDIO_DEVICE_MAX_ADDRESS_LEN );
  }
}


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

#include "MicrophoneInfoHelper.hpp"
#include "DeviceAddressHelper.hpp"
#include "SourceSinkManager.hpp"
#include "IMicrophoneSource.hpp"

AudioMicrophoneCharacteristic MicrophoneInfoHelper::getMicrophoneInfo(std::shared_ptr<ISource> pSource)
{
  AudioMicrophoneCharacteristic info;
  memset( &info, 0, sizeof(AudioMicrophoneCharacteristic) );

  if( pSource ){
    strncpy( info.device_id, pSource->toString().c_str(),  AUDIO_MICROPHONE_ID_MAX_LEN );
    info.id = SourceSinkManager::getAudioPortHandle( pSource );
    info.device = SourceSinkManager::getAudioDevice( pSource );
    strncpy( info.address, AndroidDeviceAddressHelper::getStringFromDeviceAddr( SourceSinkManager::getDeviceAddress(pSource) ).c_str(),  AUDIO_DEVICE_MAX_ADDRESS_LEN );

    // FIXME: this index isn't correct.
    int i=0;
    for(auto& aSource : SourceSinkManager::getSourceDevices() ){
      if( aSource == pSource ){
        info.index_in_the_group = i;
        break;
      }
      if( std::dynamic_pointer_cast<IMicrophoneSource>(aSource) ){
        i++;
      }
    }
  }

/*
    audio_microphone_channel_mapping_t channel_mapping[AUDIO_CHANNEL_COUNT_MAX];
    audio_microphone_location_t        location;
    audio_microphone_group_t           group;
    float                              sensitivity;
    float                              max_spl;
    float                              min_spl;
    audio_microphone_directionality_t  directionality;
    unsigned int                       num_frequency_responses;
    float frequency_responses[2][AUDIO_MICROPHONE_MAX_FREQUENCY_RESPONSES];
    struct audio_microphone_coordinate geometric_location;
    struct audio_microphone_coordinate orientation;
*/
  return info;
}

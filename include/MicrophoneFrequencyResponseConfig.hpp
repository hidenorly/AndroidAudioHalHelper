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

#ifndef __MICROPHONE_FREQ_RESPOSONE_CONFIG_HPP__
#define __MICROPHONE_FREQ_RESPOSONE_CONFIG_HPP__

#include <vector>
#include <string>

class MicrophoneFrequencyResponseConfig
{
public:
  struct FrequencyResponse
  {
    float frequencyHz;
    float levelDb;
    FrequencyResponse(std::string stFrequencyHz, std::string stLevelDb){
      frequencyHz = std::stof( stFrequencyHz );
      levelDb = std::stof( stLevelDb );
    }
  };
  static std::vector<FrequencyResponse> load(std::string path);
};

#endif /* __MICROPHONE_FREQ_RESPOSONE_CONFIG_HPP__ */
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

#include "MicrophoneFrequencyResponseConfig.hpp"
#include "Stream.hpp"
#include "StringTokenizer.hpp"
#include "StringUtil.hpp"

std::vector<MicrophoneFrequencyResponseConfig::FrequencyResponse> MicrophoneFrequencyResponseConfig::load(std::string path)
{
  std::vector<MicrophoneFrequencyResponseConfig::FrequencyResponse> result;

  std::shared_ptr<FileStream> pStream = std::make_shared<FileStream>( path );

  if( pStream ){
    std::string aLine;
    while( !pStream->isEndOfStream() ){
      if( pStream->readLine( aLine ) ){
        StringTokenizer tok( aLine, ",");
        if( tok.hasNext() ){
          std::string frequencyHz = StringUtil::trim( tok.getNext() );
          if( tok.hasNext() ){
            std::string levelDb = StringUtil::trim( tok.getNext() );
            MicrophoneFrequencyResponseConfig::FrequencyResponse aResponse( frequencyHz, levelDb );
            result.push_back( aResponse );
          }
        }
      }
    }
    pStream->close();
  }

  return result;
}

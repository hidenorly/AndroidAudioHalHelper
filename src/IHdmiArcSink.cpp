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

#include "IHdmiArcSink.hpp"
#include "HdmiEdidSadHelper.hpp"
#include "ParameterManager.hpp"
#include "StringTokenizer.hpp"
#include <sstream>
#include <vector>

#include <iostream>


#define ADDITIONAL_CAP_ATMOS "JOC_DolbyAtmos"

IHdmiArcSink::IHdmiArcSink(IHdmiArcSink::ArcType arcType):CompressedSink(), mArcType(arcType)
{
  mIsAtmosCapable = false;

  std::shared_ptr<ParameterManager> pParams = ParameterManager::getManager().lock();

  ParameterManager::CALLBACK sadHandler = [&](std::string key, std::string value){
    // parse hex formated SAD(Short Audio Descriptor) packets per 3 bytes (sad packet size is 3 bytes)
    StringTokenizer token( value, "," );
    std::vector<ByteBuffer> sadPackets;
    ByteBuffer aSadPacket;
    while( token.hasNext() ){
      std::stringstream ss( token.getNext() );
      int data = 0;
      ss >> std::hex >> data;
      aSadPacket.push_back( data );
      if( aSadPacket.size() == 3 ){
        sadPackets.push_back( aSadPacket );
        aSadPacket.clear();
      }
    }
    // parse the SAD packet to AudioFormat
    for( auto& aSadPacket : sadPackets ){
      std::vector<AudioFormat> formats = HdmiEdidSadHelper::getAudioFormatsFromSad( aSadPacket );
      mAudioFormats.insert( mAudioFormats.end(), formats.begin(), formats.end() );
      if( ( formats.size() > 0 ) && !formats[0].isEncodingPcm() ){
        std::map<std::string, std::string> addionalCapabilities = HdmiEdidSadHelper::getAdditionalCapabilities( aSadPacket );
        if( addionalCapabilities.contains( ADDITIONAL_CAP_ATMOS ) ){
          mIsAtmosCapable = addionalCapabilities[ ADDITIONAL_CAP_ATMOS ] == "true";
        }
      }
    }
  };
  mParameterHandler = pParams->registerCallback( PARAM_KEY_SAD, sadHandler );
}

IHdmiArcSink::~IHdmiArcSink()
{

}

IHdmiArcSink::ArcType IHdmiArcSink::getArcType(void)
{
  return mArcType;
}


// TODO: will encapsule or use factory template, etc.

static std::shared_ptr<ISink> pSink;

std::shared_ptr<ISink> getHdmiArcSink(void)
{
  return std::make_shared<EncodedSink>( std::make_shared<IHdmiArcSink>(), /* trasncoder */ true );
}
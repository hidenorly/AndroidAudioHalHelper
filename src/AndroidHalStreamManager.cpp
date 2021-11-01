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

#include "AndroidHalStreamManager.hpp"

void AndroidStreamManager::initialize(void)
{
  mStreams.clear();
}

void AndroidStreamManager::terminate(void)
{
  mStreams.clear();
}

void AndroidStreamManager::associateStream(AudioIoHandle ioHandle, std::shared_ptr<IStream> pStream)
{
  mStreams.insert_or_assign( ioHandle, pStream );
}

void AndroidStreamManager::unassociateStream(AudioIoHandle ioHandle)
{
  if( mStreams.contains( ioHandle ) ){
    mStreams.erase( ioHandle );
  }
}

void AndroidStreamManager::unassociateStream(std::shared_ptr<IStream> pStream)
{
  unassociateStream( getAudioIoHandle( pStream ) );
}

AudioIoHandle AndroidStreamManager::getAudioIoHandle(std::shared_ptr<IStream> pStream)
{
  AudioIoHandle theIoHandle = 0;

  for( auto& [ anIoHandle, aStream] : mStreams ){
    if( aStream == pStream ){
      theIoHandle = anIoHandle;
      break;
    }
  }

  return theIoHandle;
}

std::shared_ptr<IStream> AndroidStreamManager::getStream(AudioIoHandle ioHandle)
{
  std::shared_ptr<IStream> result;

  if( mStreams.contains( ioHandle ) ){
    result = mStreams[ ioHandle ];
  }

  return result;
}

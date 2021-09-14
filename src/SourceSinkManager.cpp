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

#include "SourceSinkManager.hpp"
#include "PipedSink.hpp"
#include "PipedSource.hpp"
#include "DeviceAddressHelper.hpp"

void SourceSinkManager::initialize(void)
{
  mSourceSinksByDeviceAddr.clear();
  mSourceSinksByAudioPortHandle.clear();
}

void SourceSinkManager::terminate(void)
{
  mSourceSinksByDeviceAddr.clear();
  mSourceSinksByAudioPortHandle.clear();
}

int SourceSinkManager::getSourceSinkCount()
{
  return mSourceSinksByDeviceAddr.size();
}


void SourceSinkManager::associateByAudioPortConfig(const audio_port_config& audioPortConfig)
{
  mSourceSinksByAudioPortHandle.insert_or_assign( audioPortConfig.id, getSinkFromDevice(AndroidDeviceAddressHelper::getDeviceAddrFromString( audioPortConfig.ext.device.address )));
}


void SourceSinkManager::associateByAudioPort(const audio_port& audioPort)
{
  mSourceSinksByAudioPortHandle.insert_or_assign( audioPort.id, getSinkFromDevice(AndroidDeviceAddressHelper::getDeviceAddrFromString( audioPort.ext.device.address )));
}

void SourceSinkManager::attachSink(DeviceAddress deviceAddr, std::shared_ptr<ISink> pSink)
{
  std::shared_ptr<ISourceSinkCommon> pDevice = pSink;
  if( !std::dynamic_pointer_cast<PipedSink>(pSink) ){
    pDevice = std::make_shared<PipedSink>( pSink );
  }
  if( pDevice ){
    mSourceSinksByDeviceAddr.insert_or_assign( AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr), pDevice );
  }
}

void SourceSinkManager::attachSource(DeviceAddress deviceAddr, std::shared_ptr<ISource> pSource)
{
  std::shared_ptr<ISourceSinkCommon> pDevice = pSource;
  if( !std::dynamic_pointer_cast<PipedSource>(pSource) ){
    std::shared_ptr<PipedSource> pPipedSource = std::make_shared<PipedSource>();
    pPipedSource->attachSource( pSource );
    pDevice = pPipedSource;
  }
  if( pDevice ){
    mSourceSinksByDeviceAddr.insert_or_assign( AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr), pDevice );
  }
}

void SourceSinkManager::detachSourceSinkByDeviceAddr(DeviceAddress deviceAddr)
{
  std::string deviceAddrStr = AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr);
  if( mSourceSinksByDeviceAddr.contains( deviceAddrStr ) ){
    mSourceSinksByDeviceAddr.erase( deviceAddrStr );
  }
}

void SourceSinkManager::detachSourceSink(std::shared_ptr<ISourceSinkCommon> pSourceSink)
{
  std::string targetAddr;
  for( auto& [deviceAddr, pDevice] : mSourceSinksByDeviceAddr ){
    if( pDevice == pSourceSink ){
      targetAddr = deviceAddr;
      break;
    }
  }
  if( mSourceSinksByDeviceAddr.contains( targetAddr ) ){
    mSourceSinksByDeviceAddr.erase( targetAddr );
  }
}

std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSinkFromDevice(DeviceAddress deviceAddr)
{
  std::shared_ptr<ISourceSinkCommon> result;

  std::string deviceAddrStr = AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr);

  if( mSourceSinksByDeviceAddr.contains(deviceAddrStr) ){
    result = mSourceSinksByDeviceAddr[deviceAddrStr];
  }

  return result;
}

std::shared_ptr<ISink> SourceSinkManager::getSinkFromDevice(DeviceAddress deviceAddr)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSinkFromDevice(deviceAddr) );
}

std::shared_ptr<ISource> SourceSinkManager::getSourceFromDevice(DeviceAddress deviceAddr)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSinkFromDevice(deviceAddr) );
}

std::vector<std::shared_ptr<ISink>> SourceSinkManager::getSinkDevices(void)
{
  std::vector<std::shared_ptr<ISink>> result;

  for(auto& [ deviceAddr, pDevice] : mSourceSinksByDeviceAddr ){
    std::shared_ptr<ISink> pSink = std::dynamic_pointer_cast<ISink>( pDevice );
    if( pSink ){
      result.push_back( pSink );
    }
  }

  return result;
}


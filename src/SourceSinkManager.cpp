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

audio_port_handle_t SourceSinkManager::getAudioPortHandle(const audio_port_config& audioPortConfig)
{
  return audioPortConfig.id;
}

audio_port_handle_t SourceSinkManager::getAudioPortHandle(const audio_port& audioPort)
{
  return audioPort.id;
}

void SourceSinkManager::associateByAudioPortConfig(const audio_port_config& audioPortConfig)
{
  mSourceSinksByAudioPortHandle.insert_or_assign( getAudioPortHandle(audioPortConfig), getSourceSink(AndroidDeviceAddressHelper::getDeviceAddrFromString( audioPortConfig.ext.device.address )));
}


void SourceSinkManager::associateByAudioPort(const audio_port& audioPort)
{
  mSourceSinksByAudioPortHandle.insert_or_assign( getAudioPortHandle(audioPort), getSourceSink(AndroidDeviceAddressHelper::getDeviceAddrFromString( audioPort.ext.device.address )));
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

std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSink(DeviceAddress deviceAddr)
{
  std::shared_ptr<ISourceSinkCommon> result;

  std::string deviceAddrStr = AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr);

  if( mSourceSinksByDeviceAddr.contains(deviceAddrStr) ){
    result = mSourceSinksByDeviceAddr[deviceAddrStr];
  }

  return result;
}

DeviceAddress SourceSinkManager::getDeviceAddress(std::shared_ptr<ISourceSinkCommon> pSourceSink)
{
  std::string resultAddr;

  for( auto& [deviceAddr, pDevice] : mSourceSinksByDeviceAddr ){
    if( pDevice == pSourceSink ){
      resultAddr = deviceAddr;
      break;
    }
  }
  return AndroidDeviceAddressHelper::getDeviceAddrFromString( resultAddr);
}


std::shared_ptr<ISink> SourceSinkManager::getSink(DeviceAddress deviceAddr)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(deviceAddr) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(DeviceAddress deviceAddr)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(deviceAddr) );
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

std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSink(audio_port_handle_t audioPortHandle)
{
  std::shared_ptr<ISourceSinkCommon> result;

  if( mSourceSinksByAudioPortHandle.contains(audioPortHandle) ){
    result = mSourceSinksByAudioPortHandle[audioPortHandle];
  }

  return result;
}

std::shared_ptr<ISink> SourceSinkManager::getSink(audio_port_handle_t audioPortHandle)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(audioPortHandle) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(audio_port_handle_t audioPortHandle)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(audioPortHandle) );
}

std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSink(const audio_port_config& audioPortConfig)
{
  return getSourceSink( getAudioPortHandle( audioPortConfig ) );
}

std::shared_ptr<ISink> SourceSinkManager::getSink(const audio_port_config& audioPortConfig)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(audioPortConfig) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(const audio_port_config& audioPortConfig)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(audioPortConfig) );
}

std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSink(const audio_port& audioPort)
{
  return getSourceSink( getAudioPortHandle( audioPort ) );
}

std::shared_ptr<ISink> SourceSinkManager::getSink(const audio_port& audioPort)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(audioPort) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(const audio_port& audioPort)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(audioPort) );
}


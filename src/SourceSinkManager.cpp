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
#include "AudioPortHelper.hpp"

void SourceSinkManager::initialize(void)
{
  mSourceSinksByDeviceAddr.clear();
  mSourceSinksByAudioPortHandle.clear();
  mSourceSinksByAudioDevice.clear();
}

void SourceSinkManager::terminate(void)
{
  mSourceSinksByDeviceAddr.clear();
  mSourceSinksByAudioPortHandle.clear();
  mSourceSinksByAudioDevice.clear();
}

int SourceSinkManager::getSourceSinkCount()
{
  return mSourceSinksByDeviceAddr.size();
}

AudioPortHandle SourceSinkManager::getAudioPortHandle(const AudioPortConfig& audioPortConfig)
{
  return audioPortConfig.id;
}

AudioPortHandle SourceSinkManager::getAudioPortHandle(const AudioPort& audioPort)
{
  return audioPort.id;
}

AudioPortHandle SourceSinkManager::getAudioPortHandle(std::shared_ptr<ISourceSinkCommon> theSourceSink)
{
  AudioPortHandle result = 0;

  for( auto&[ audioPortHandle, pSourceSink ] : mSourceSinksByAudioPortHandle ){
    if( pSourceSink == pSourceSink ){
      result = audioPortHandle;
      break;
    }
  }

  return result;
}


void SourceSinkManager::associateByAudioPortConfig(const AudioPortConfig& audioPortConfig)
{
  switch( audioPortConfig.type ){
    case AUDIO_PORT_TYPE_DEVICE:
      {
        std::shared_ptr<ISourceSinkCommon> pDevice = getSourceSink(AndroidDeviceAddressHelper::getDeviceAddrFromString( audioPortConfig.ext.device.address ));
        mSourceSinksByAudioPortHandle.insert_or_assign( getAudioPortHandle(audioPortConfig), pDevice );
        mSourceSinksByAudioDevice.insert_or_assign( audioPortConfig.ext.device.type, pDevice );
      }
      break;
    case AUDIO_PORT_TYPE_MIX:;
      break;
    case AUDIO_PORT_TYPE_SESSION:
      {
        // TODO: confirm this behaves as it expects or not
        AudioPortHandle portHandle = getAudioPortHandle( audioPortConfig );
        std::shared_ptr<ISourceSinkCommon> pDevice = getSourceSink( portHandle );
        if( pDevice ){
          mSourceSinksByAudioPortHandle.insert_or_assign( portHandle, pDevice );
        }
      }
      break;
    default:;
      break;
  }
}


void SourceSinkManager::associateByAudioPort(const AudioPort& audioPort)
{
  switch( audioPort.type ){
    case AUDIO_PORT_TYPE_DEVICE:
      {
        std::shared_ptr<ISourceSinkCommon> pDevice = getSourceSink(AndroidDeviceAddressHelper::getDeviceAddrFromString( audioPort.ext.device.address ));
        mSourceSinksByAudioPortHandle.insert_or_assign( getAudioPortHandle(audioPort), pDevice );
        mSourceSinksByAudioDevice.insert_or_assign( audioPort.ext.device.type, pDevice );
      }
      break;
    case AUDIO_PORT_TYPE_MIX:;
      break;
    case AUDIO_PORT_TYPE_SESSION:
      {
        // TODO: confirm this behaves as it expects or not
        AudioPortHandle portHandle = getAudioPortHandle( audioPort );
        std::shared_ptr<ISourceSinkCommon> pDevice = getSourceSink( portHandle );
        if( pDevice ){
          mSourceSinksByAudioPortHandle.insert_or_assign( portHandle, pDevice );
        }
      }
      break;
    default:;
      break;
  }
}

void SourceSinkManager::attachSink(DeviceAddress deviceAddr, std::shared_ptr<ISink> pSink)
{
  std::shared_ptr<ISourceSinkCommon> pDevice = pSink;
  if( !std::dynamic_pointer_cast<PipedSink>(pSink) ){
    pDevice = std::make_shared<PipedSink>( pSink );
  }
  if( pDevice ){
    mSourceSinksByDeviceAddr.insert_or_assign( AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr), pDevice );
    mSourceSinksByAudioDevice.insert_or_assign( deviceAddr.device, pDevice );
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
    mSourceSinksByAudioDevice.insert_or_assign( deviceAddr.device, pDevice );
  }
}

void SourceSinkManager::detachSourceSinkByDeviceAddr(DeviceAddress deviceAddr)
{
  std::string deviceAddrStr = AndroidDeviceAddressHelper::getStringFromDeviceAddr(deviceAddr);
  if( mSourceSinksByDeviceAddr.contains( deviceAddrStr ) ){
    mSourceSinksByDeviceAddr.erase( deviceAddrStr );
  }
  if( mSourceSinksByAudioDevice.contains( deviceAddr.device ) ){
    mSourceSinksByAudioDevice.erase( deviceAddr.device );
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

  for( auto& [deviceType, pDevice] : mSourceSinksByAudioDevice ){
    if( pDevice == pSourceSink ){
      mSourceSinksByAudioDevice.erase( deviceType );
      break;
    }
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
  return AndroidDeviceAddressHelper::getDeviceAddrFromString( resultAddr );
}

std::shared_ptr<ISink> SourceSinkManager::getSink(DeviceAddress deviceAddr)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(deviceAddr) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(DeviceAddress deviceAddr)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(deviceAddr) );
}


std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSink(AudioDevice device)
{
  std::shared_ptr<ISourceSinkCommon> result;

  if( mSourceSinksByAudioDevice.contains( device ) ){
    result = mSourceSinksByAudioDevice[ device ];
  }

  return result;
}

std::shared_ptr<ISink> SourceSinkManager::getSink(AudioDevice device)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(device) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(AudioDevice device)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(device) );
}

AudioDevice SourceSinkManager::getAudioDevice(std::shared_ptr<ISourceSinkCommon> theSourceSink)
{
  AudioDevice result = AUDIO_DEVICE_OUT_DEFAULT;

  for(auto& [audioDevice, pSourceSink] : mSourceSinksByAudioDevice ){
    if( pSourceSink == theSourceSink ){
      result = audioDevice;
      break;
    }
  }

  return result;
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

std::vector<std::shared_ptr<ISource>> SourceSinkManager::getSourceDevices(void)
{
  std::vector<std::shared_ptr<ISource>> result;

  for(auto& [ deviceAddr, pDevice] : mSourceSinksByDeviceAddr ){
    std::shared_ptr<ISource> pSource = std::dynamic_pointer_cast<ISource>( pDevice );
    if( pSource ){
      result.push_back( pSource );
    }
  }

  return result;
}

std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSink(AudioPortHandle audioPortHandle)
{
  std::shared_ptr<ISourceSinkCommon> result;

  if( mSourceSinksByAudioPortHandle.contains(audioPortHandle) ){
    result = mSourceSinksByAudioPortHandle[audioPortHandle];
  }

  return result;
}

std::shared_ptr<ISink> SourceSinkManager::getSink(AudioPortHandle audioPortHandle)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(audioPortHandle) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(AudioPortHandle audioPortHandle)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(audioPortHandle) );
}

std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSink(const AudioPortConfig& audioPortConfig)
{
  std::shared_ptr<ISourceSinkCommon> result;
  // try to get with AudioPort
  result = getSourceSink( getAudioPortHandle( audioPortConfig ) );
  // try to get with AudioDevice
  if( !result ){
    result = getSourceSink( AndroidAudioPortHelper::getAudioDeviceFromAudioPortConfig( audioPortConfig ) );
  }
  return result;
}

std::shared_ptr<ISink> SourceSinkManager::getSink(const AudioPortConfig& audioPortConfig)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(audioPortConfig) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(const AudioPortConfig& audioPortConfig)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(audioPortConfig) );
}

std::shared_ptr<ISourceSinkCommon> SourceSinkManager::getSourceSink(const AudioPort& audioPort)
{
  std::shared_ptr<ISourceSinkCommon> result;
  // try to get with AudioPort
  result = getSourceSink( getAudioPortHandle( audioPort ) );
  // try to get with AudioDevice
  if( !result ){
    result = getSourceSink( AndroidAudioPortHelper::getAudioDeviceFromAudioPort( audioPort ) );
  }
  return result;
}

std::shared_ptr<ISink> SourceSinkManager::getSink(const AudioPort& audioPort)
{
  return std::dynamic_pointer_cast<ISink>( getSourceSink(audioPort) );
}

std::shared_ptr<ISource> SourceSinkManager::getSource(const AudioPort& audioPort)
{
  return std::dynamic_pointer_cast<ISource>( getSourceSink(audioPort) );
}

AudioPortConfig SourceSinkManager::getAudioPortConfig(std::shared_ptr<ISourceSinkCommon> pSourceSink, AudioModuleHandle hwModule)
{
  AudioPortConfig portConfig;
  AndroidAudioPortHelper::getAndroidPortConfigFromSourceSink(
    &portConfig,
    pSourceSink,
    getAudioPortHandle( pSourceSink ),
    AndroidDeviceAddressHelper::getStringFromDeviceAddr( getDeviceAddress( pSourceSink ) ),
    hwModule, // TODO: ensure the hwModule (AudioModuleHandle)
    getAudioDevice( pSourceSink )
  );
  return portConfig;
}

AudioPort SourceSinkManager::getAudioPort(std::shared_ptr<ISourceSinkCommon> pSourceSink, AudioModuleHandle hwModule)
{
  AudioPort portConfig;
  AndroidAudioPortHelper::getAndroidPortFromSourceSink(
    &portConfig,
    pSourceSink,
    getAudioPortHandle( pSourceSink ),
    AndroidDeviceAddressHelper::getStringFromDeviceAddr( getDeviceAddress( pSourceSink ) ),
    hwModule, // TODO: ensure the hwModule (AudioModuleHandle)
    getAudioDevice( pSourceSink )
  );
  return portConfig;
}


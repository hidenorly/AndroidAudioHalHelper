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

#include "AudioEffectHelper.hpp"
#include "Stream.hpp"
#include "StringTokenizer.hpp"
#include "StringUtil.hpp"

void AudioEffectHelper::initialize(std::string filterPlugInPath, std::string uuidFilterIdTblPath)
{
  FilterManager::setPlugInPath(filterPlugInPath);
  std::weak_ptr<FilterManager> pWeakManager = FilterManager::getInstance();
  std::shared_ptr<FilterManager> pManager = pWeakManager.lock();
  if( pManager ){
    pManager->initialize();
  }
  mFiltersIdResolver.clear();
  mUuidEffectResolver.clear();
  mEffectIdUuidResolver.clear();

  loadUuidFilterIdTable( uuidFilterIdTblPath );
}

void AudioEffectHelper::terminate(void)
{
  std::weak_ptr<FilterManager> pWeakManager = FilterManager::getInstance();
  std::shared_ptr<FilterManager> pManager = pWeakManager.lock();
  if( pManager ){
    pManager->terminate();
  }
  mFiltersIdResolver.clear();
  mUuidEffectResolver.clear();
  mEffectIdUuidResolver.clear();
}

void AudioEffectHelper::loadUuidFilterIdTable(std::string path)
{
  std::shared_ptr<IStream> pStream = std::make_shared<FileStream>( path );
  std::string aLine;
  while( !pStream->isEndOfStream() ){
    if( pStream->readLine( aLine ) ){
      StringTokenizer tok( aLine, "=");
      if( tok.hasNext() ){
        std::string uuid = StringUtil::trim( tok.getNext() );
        std::string filterId = StringUtil::trim( tok.getNext() );
        associateFilter( uuid, filterId );
      }
    }
  }
  pStream->close();
}

std::vector<std::string> AudioEffectHelper::getInstanciatableEffectsUuids(void)
{
  std::vector<std::string> result;

  for( auto& [uuid, filterId] : mFiltersIdResolver ){
    std::shared_ptr<IFilter> pFilter = getFilterInstance( uuid );
    if( pFilter ){
      result.push_back( uuid );
    }
  }

  return result;
}


void AudioEffectHelper::associateFilter(std::string uuid, std::string effectIdString)
{
  mFiltersIdResolver.insert_or_assign( uuid, effectIdString );
}

void AudioEffectHelper::unassociateFilter(std::string uuid)
{
  if( mFiltersIdResolver.contains( uuid ) ){
    mFiltersIdResolver.erase( uuid );
  }
}

std::string AudioEffectHelper::getUuidFromEffectId(uint64_t effectId)
{
  std::string result;
  if( mEffectIdUuidResolver.contains( effectId ) ){
    result = mEffectIdUuidResolver[ effectId ];
  }
  return result;
}

std::shared_ptr<IFilter> AudioEffectHelper::getFilterInstance(std::string uuid)
{
  std::shared_ptr<IFilter> result;

  if( mFiltersIdResolver.contains( uuid ) ){
    std::string aPlugInId = mFiltersIdResolver[ uuid ];
    std::weak_ptr<FilterManager> pWeakManager = FilterManager::getInstance();
    std::shared_ptr<FilterManager> pManager = pWeakManager.lock();
    if( pManager->hasPlugIn( aPlugInId ) ){
      // TODO: should support the instantiation
      std::shared_ptr<IFilter> pFilter = std::dynamic_pointer_cast<IFilter>( pManager->getPlugIn( aPlugInId ) );
      if( pFilter ){
        result = pFilter;
      }
    }
  }

  return result;
}


std::shared_ptr<IFilter> AudioEffectHelper::getFilterFromEffectInstance(uint64_t effectId)
{
  std::shared_ptr<IFilter> result;

  std::shared_ptr<IEffect> pEffect = getEffect( getUuidFromEffectId(effectId) );
  if( pEffect ){
    result = pEffect->getFilter();
  }

  return result;
}

void AudioEffectHelper::associateEffect(std::shared_ptr<IEffect> pEffect)
{
  if( pEffect ){
    mUuidEffectResolver.insert_or_assign( pEffect->getUuid(), pEffect );
    mEffectIdUuidResolver.insert_or_assign( pEffect->getEffectId(), pEffect->getUuid() );
  }
}

void AudioEffectHelper::unassociateEffect(std::shared_ptr<IEffect> pEffect)
{
  if( pEffect ){
    // remove from mUuidEffectResolver
    std::vector<std::string> removeUuids;
    for( auto& [uuid, anEffect] : mUuidEffectResolver ){
      if( anEffect == pEffect ){
        removeUuids.push_back( uuid );
//        break; // to enumerate all of disposed instances
      }
      if( !anEffect ){
        removeUuids.push_back( uuid );
      }
    }
    for(auto& aUuid : removeUuids){
      mUuidEffectResolver.erase( aUuid );
    }

    // remove from mEffectIdUuidResolver
    std::vector<uint64_t> removeEffectIds;
    for( auto& [effectId, uuid] : mEffectIdUuidResolver ){
      for(auto& theUuid : removeUuids){
        if( uuid == theUuid ){
          removeEffectIds.push_back( effectId );
          break;
        }
      }
    }
    for(auto& anEffectId : removeEffectIds){
      mEffectIdUuidResolver.erase( anEffectId );
    }
  }
}

std::shared_ptr<IEffect> AudioEffectHelper::getEffect(std::string uuid)
{
  std::shared_ptr<IEffect> result;
  if( mUuidEffectResolver.contains( uuid ) ){
    result = mUuidEffectResolver[ uuid ];
  }
  return result;
}

EffectFlags AudioEffectHelper::getDefaultEffectFlags(void)
{
  EffectFlags result =
    EffectFlag::TYPE_POST_PROC |
    EffectFlag::INSERT_ANY |
    EffectFlag::VOLUME_NONE |
    EffectFlag::DEVICE_IND |
    EffectFlag::INPUT_DIRECT |
    EffectFlag::OUTPUT_DIRECT |
    EffectFlag::HW_ACC_TUNNEL |
    EffectFlag::AUDIO_MODE_IND |
    EffectFlag::AUDIO_SOURCE_IND |
    EffectFlag::OFFLOAD_SUPPORTED;

  return result;
}

EffectDescriptor AudioEffectHelper::getDescriptor(Uuid uuid, std::shared_ptr<IFilter> pFilter)
{
  EffectDescriptor result;

  if( !pFilter ){
    pFilter = getFilterInstance( uuid );
  }
  if( pFilter ){
    result.uuid = uuid;
    result.flags = getDefaultEffectFlags();
    result.cpuLoad = pFilter->getExpectedProcessingUSec();
    std::shared_ptr<FilterPlugIn> pPlugIn = std::dynamic_pointer_cast<FilterPlugIn>(pFilter);
    if( pPlugIn ){
      strncpy( (char*)result.name, pPlugIn->getId().c_str(), sizeof( result.name ) );
      strncpy( (char*)result.implementor, pPlugIn->toString().c_str(), sizeof( result.implementor ) );
    }
  }

  return result;
}

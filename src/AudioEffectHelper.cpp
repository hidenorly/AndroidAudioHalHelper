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

//TODO: createEffect (resolve uuid -> FilterPlugIn Id) -> store the effectId to mFiltersIdResolver

void AudioEffectHelper::initialize(std::string filterPlugInPath)
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

void AudioEffectHelper::associateFilter(std::string uuid, std::string effectIdString)
{
  mFiltersIdResolver.insert_or_assign( uuid, effectIdString );
}

std::string AudioEffectHelper::getUuidFromEffectId(uint64_t effectId)
{
  std::string result;
  if( mEffectIdUuidResolver.contains( effectId ) ){
    result = mEffectIdUuidResolver[ effectId ];
  }
  return result;
}


std::string AudioEffectHelper::getEffectIdString(uint64_t effectId)
{
  std::string result;
  std::string uuid = getUuidFromEffectId( effectId );
  if( mFiltersIdResolver.contains( uuid ) ){
    result = mFiltersIdResolver[ uuid ];
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

void AudioEffectHelper::associateEffect(std::weak_ptr<IEffect> pEffect)
{
  std::shared_ptr<IEffect> effect = pEffect.lock();
  if( effect ){
    mUuidEffectResolver.insert_or_assign( effect->getUuid(), effect );
    mEffectIdUuidResolver.insert_or_assign( effect->getEffectId(), effect->getUuid() );
  }
}

void AudioEffectHelper::unassociateEffect(std::weak_ptr<IEffect> pEffect)
{
  std::shared_ptr<IEffect> effect = pEffect.lock();
  if( effect ){
    // remove from mUuidEffectResolver
    std::vector<std::string> removeUuids;
    for( auto& [uuid, anEffect] : mUuidEffectResolver ){
      std::shared_ptr<IEffect> theEffect = anEffect.lock();
      if( theEffect == effect ){
        removeUuids.push_back( uuid );
//        break; // to enumerate all of disposed instances
      }
      if( !theEffect ){
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
    result = mUuidEffectResolver[ uuid ].lock();
  }
  return result;
}

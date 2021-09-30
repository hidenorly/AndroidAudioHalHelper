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
}

void AudioEffectHelper::terminate(void)
{
  std::weak_ptr<FilterManager> pWeakManager = FilterManager::getInstance();
  std::shared_ptr<FilterManager> pManager = pWeakManager.lock();
  if( pManager ){
    pManager->terminate();
  }
  mFiltersIdResolver.clear();
}

void AudioEffectHelper::associateEffect(uint64_t effectId, std::string effectIdString)
{
  mFiltersIdResolver.insert_or_assign( effectId, effectIdString );
}

void AudioEffectHelper::associateEffect(std::shared_ptr<IEffect> pEffect, std::string effectIdString)
{
  if( pEffect ){
    associateEffect( pEffect->getEffectId(), effectIdString );
  }
}


std::string AudioEffectHelper::getEffectIdString(uint64_t effectId)
{
  std::string result;
  if( mFiltersIdResolver.contains(effectId) ){
    result = mFiltersIdResolver[effectId];
  }
  return result;
}

uint64_t AudioEffectHelper::getEffectIdFromEffectIdString(std::string effectIdString)
{
  uint64_t result = 0;
  for( auto& [anEffectId, anEffectIdString] : mFiltersIdResolver ){
    if( anEffectIdString == effectIdString ){
      result = anEffectId;
      break;
    }
  }
  return result;
}

std::shared_ptr<IFilter> AudioEffectHelper::getEffect(uint64_t effectId)
{
  std::shared_ptr<IFilter> result;
  std::weak_ptr<FilterManager> pWeakManager = FilterManager::getInstance();
  std::shared_ptr<FilterManager> pManager = pWeakManager.lock();
  if( pManager ){
    std::string aPlugInId = getEffectIdString( effectId );
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

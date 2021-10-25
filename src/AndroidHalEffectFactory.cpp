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

#include "AndroidHalEffectFactory.hpp"
#include "AudioEffectHelper.hpp"

EffectDescriptor IEffectsFactory::getDescriptor(Uuid uuid)
{
  return AudioEffectHelper::getDescriptor( uuid );
}

std::vector<EffectDescriptor> IEffectsFactory::getAllDescriptors(void)
{
  std::vector<EffectDescriptor> result;

  std::vector<std::string> uuids = AudioEffectHelper::getInstanciatableEffectsUuids();
  for( auto& aUuid : uuids ){
    result.push_back( getDescriptor( aUuid ) );
  }

  return result;
}


std::shared_ptr<IEffect> IEffectsFactory::createEffect(Uuid uuid, AudioSession session, AudioIoHandle ioHandle, AudioPortHandle device)
{
  std::shared_ptr<IEffect> pEffect = AudioEffectHelper::getEffect( uuid );

  if( pEffect ){
    std::shared_ptr<IFilter> pFilter = pEffect->getFilter();
    if( !pFilter ){
      pEffect->associateFilter( AudioEffectHelper::getFilterInstance( uuid ) );
    }

    // TODO : associate session, ioHandle, device to the pEffect
  }

  return pEffect;
}

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

#include "AndroidHalEffectDownmix.hpp"
#include "AndroidHalEffectNoiseSupression.hpp"
#include "AndroidHalEffectAcousticEchoCanceler.hpp"
#include "AndroidHalEffectEnvironmentalReverb.hpp"
#include "AndroidHalEffectPresetReverb.hpp"
#include "AndroidHalEffectAutomaticGainControl.hpp"
#include "AndroidHalEffectEqualizer.hpp"
#include "AndroidHalEffectVirtualizer.hpp"
#include "AndroidHalEffectBassBoost.hpp"
#include "AndroidHalEffectVisualizer.hpp"
#include "AndroidHalEffectLoudnessEnhancer.hpp"

std::vector<EffectDescriptor> IEffectsFactory::getAllDescriptors(void)
{
  std::vector<EffectDescriptor> result;

  std::vector<std::string> uuids = AudioEffectHelper::getInstanciatableEffectsUuids();
  for( auto& aUuid : uuids ){
    result.push_back( getDescriptor( aUuid ) );
  }

  return result;
}

EffectDescriptor IEffectsFactory::getDescriptor(Uuid uuid)
{
  return AudioEffectHelper::getDescriptor( uuid );
}

std::shared_ptr<IEffect> IEffectsFactory::createEffect(Uuid uuid, AudioSession session, AudioIoHandle ioHandle, AudioPortHandle device)
{
  std::shared_ptr<IEffect> pEffect = AudioEffectHelper::getEffect( uuid );
  if( !pEffect ){
    std::shared_ptr<IFilter> pFilter = AudioEffectHelper::getFilterInstance( uuid );
    if( pFilter ){
      if( uuid == IAcousticEchoCancelerEffect::UUID ){
        pEffect = std::make_shared<IAcousticEchoCancelerEffect>();
      } else if (uuid == IAutomaticGainControlEffect::UUID ){
        pEffect = std::make_shared<IAutomaticGainControlEffect>();
      } else if (uuid == IBassBoostEffect::UUID ){
        pEffect = std::make_shared<IBassBoostEffect>();
      } else if (uuid == IDownmixEffect::UUID ){
        pEffect = std::make_shared<IDownmixEffect>();
      } else if (uuid == IEnvironmentalReverbEffect::UUID ){
        pEffect = std::make_shared<IEnvironmentalReverbEffect>();
      } else if (uuid == IEqualizerEffect::UUID ){
        pEffect = std::make_shared<IEqualizerEffect>();
      } else if (uuid == ILoudnessEnhancerEffect::UUID ){
        pEffect = std::make_shared<ILoudnessEnhancerEffect>();
      } else if (uuid == INoiseSuppressionEffect::UUID ){
        pEffect = std::make_shared<INoiseSuppressionEffect>();
      } else if (uuid == IPresetReverbEffect::UUID ){
        pEffect = std::make_shared<IPresetReverbEffect>();
      } else if (uuid == IVirtualizerEffect::UUID ){
        pEffect = std::make_shared<IVirtualizerEffect>();
      } else if (uuid == IVisualizerEffect::UUID ){
        pEffect = std::make_shared<IVisualizerEffect>();
      }
      if( pEffect ){
        // TODO : associate session, ioHandle, device to the pEffect
      }
    }
  }

  return pEffect;
}

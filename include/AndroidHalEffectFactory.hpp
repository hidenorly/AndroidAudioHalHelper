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

#ifndef __ANDROID_HAL_EFFECT_FACTORY_HPP__
#define __ANDROID_HAL_EFFECT_FACTORY_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"
#include <vector>
#include <memory>

class IEffectsFactory
{
public:
  static std::vector<EffectDescriptor> getAllDescriptors(void);
  static EffectDescriptor getDescriptor(Uuid uuid);
  static std::shared_ptr<IEffect> createEffect(Uuid uuid, AudioSession session, AudioIoHandle ioHandle, AudioPortHandle device);
};

#endif /* __ANDROID_HAL_EFFECT_FACTORY_HPP__ */

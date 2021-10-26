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

#include "AndroidHalHelper.hpp"

#include "DeviceAddressHelper.hpp"
#include "SourceSinkManager.hpp"

#include "AudioEffectHelper.hpp"
#include "AndroidHalEffectEnvironmentalReverb.hpp"
#include "AndroidHalEffectPresetReverb.hpp"
#include "AndroidHalEffectEqualizer.hpp"
#include "AndroidHalEffectBassBoost.hpp"
#include "AndroidHalEffectVirtualizer.hpp"
#include "AndroidHalEffectAutomaticGainControl.hpp"
#include "AndroidHalEffectAcousticEchoCanceler.hpp"
#include "AndroidHalEffectNoiseSuppression.hpp"
#include "AndroidHalEffectLoudnessEnhancer.hpp"
#include "AndroidHalEffectDownmix.hpp"
#include "AndroidHalEffectVisualizer.hpp"

void AndroidHalHelper::initialize(std::string filterPlugInPath, std::string uuidFilterIdTblPath)
{
  // --- initialize AndroidDeviceAddressHelper
  AndroidDeviceAddressHelper::initialize();

  // --- initialize SourceSinkManager
  SourceSinkManager::initialize();

  // --- initialize EffectHelper
  AudioEffectHelper::initialize(filterPlugInPath, uuidFilterIdTblPath);

  // associating(registering instatiable effects) with the UUID
  std::make_shared<IEnvironmentalReverbEffect>();
  std::make_shared<IPresetReverbEffect>();
  std::make_shared<IEqualizerEffect>();
  std::make_shared<IBassBoostEffect>();
  std::make_shared<IVirtualizerEffect>();
  std::make_shared<IAutomaticGainControlEffect>();
  std::make_shared<IAcousticEchoCancelerEffect>();
  std::make_shared<INoiseSuppressionEffect>();
  std::make_shared<ILoudnessEnhancerEffect>();
  std::make_shared<IDownmixEffect>();
  std::make_shared<IVisualizerEffect>();
}

void AndroidHalHelper::terminate(void)
{
  // --- terminate EffectHelper
  AudioEffectHelper::terminate();

  // --- terminate SourceSinkManager
  SourceSinkManager::terminate();

  // --- terminate AndroidDeviceAddressHelper
  AndroidDeviceAddressHelper::terminate();
}

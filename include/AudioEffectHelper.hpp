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

#ifndef __AUDIOEFFECT_HELPER_HPP__
#define __AUDIOEFFECT_HELPER_HPP__

#include <memory>
#include <string>
#include "Filter.hpp"
#include "AndroidHalEffect.hpp"

class AudioEffectHelper
{
protected:
  static inline std::map<std::string, std::string> mFiltersIdResolver; // uuid, effectIdString
  static inline std::map<std::string, std::shared_ptr<IEffect>> mUuidEffectResolver; // uuid, IEffect
  static inline std::map<uint64_t, std::string> mEffectIdUuidResolver; //effectId, uuid

public:
  static void initialize(std::string filterPlugInPath, std::string uuidFilterIdTblPath = "uuidFilterTable.ini");
  static void terminate(void);

  // Filter
  static void loadUuidFilterIdTable(std::string path = "uuidFilterTable.ini");
  static void associateFilter(std::string uuid, std::string effectIdString);
  static void unassociateFilter(std::string uuid);
  static std::shared_ptr<IFilter> getFilterInstance(std::string uuid);

  static std::vector<std::string> getInstanciatableEffectsUuids(void);

  // Effect
  static void associateEffect(std::shared_ptr<IEffect> pEffect);
  static void unassociateEffect(std::shared_ptr<IEffect> pEffect);
  static std::shared_ptr<IEffect> getEffect(std::string uuid);

  static std::string getUuidFromEffectId(uint64_t effectId);
  static std::shared_ptr<IFilter> getFilterFromEffectInstance(uint64_t effectId);

  static EffectFlags getDefaultEffectFlags(void);
  static EffectDescriptor getDescriptor(Uuid uuid, std::shared_ptr<IFilter> pFilter = nullptr );
};

#endif /* __AUDIOEFFECT_HELPER_HPP__ */

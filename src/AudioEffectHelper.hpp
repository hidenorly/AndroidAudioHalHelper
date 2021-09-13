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

#include <string>
#include "Filter.hpp"

class AudioEffectHelper
{
protected:
  static inline std::map<uint64_t, std::string> mFiltersIdResolver;

public:
  static void initialize(std::string filterPlugInPath);
  static void terminate(void);

  static std::string getEffectIdString(uint64_t effectId);
  static uint64_t getEffectIdFromEffectIdString(std::string effectIdString);

  static std::shared_ptr<IFilter> getEffect(uint64_t effectId);
};

#endif /* __AUDIOEFFECT_HELPER_HPP__ */

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

#ifndef __PARAMETER_HELPER_HPP__
#define __PARAMETER_HELPER_HPP__

#include "AndroidHalTypes.hpp"
#include "ParameterManager.hpp"

class ParameterManagerHelper
{
public:
  static HalResult getParameters(std::vector<std::string> keys, std::vector<ParameterValue>& values);
  static HalResult setParameters(std::vector<ParameterValue> values);
};

#endif /* __PARAMETER_HELPER_HPP__ */

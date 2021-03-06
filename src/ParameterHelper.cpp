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

#include "ParameterHelper.hpp"

HalResult ParameterManagerHelper::getParameters(std::vector<std::string> keys, std::vector<ParameterValue>& values)
{
  HalResult result = HalResult::OK;
  std::shared_ptr<ParameterManager> pParams = ParameterManager::getManager().lock();

  for( auto& aKey : keys ){
    std::string value = pParams->getParameter( aKey );
    if( value.empty() ){
      result = HalResult::INVALID_ARGUMENTS;
    } else {
      values.push_back( ParameterValue(aKey, value) );
    }
  }

  return HalResult::OK;
}

HalResult ParameterManagerHelper::setParameters(std::vector<ParameterValue> values)
{
  bool result = true;
  std::shared_ptr<ParameterManager> pParams = ParameterManager::getManager().lock();

  for( auto& aParam : values ){
    result &= pParams->setParameter( aParam.key, aParam.value );
  }

  return result ? HalResult::OK : HalResult::INVALID_ARGUMENTS;
}

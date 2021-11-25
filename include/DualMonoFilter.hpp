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

#include "Filter.hpp"
#include "AndroidHalTypes.hpp"

class DualMonoFilter : public PassThroughFilter
{
protected:
  DualMonoMode mDualMonoMode;

public:
  DualMonoFilter();
  virtual ~DualMonoFilter();
  virtual bool isAvailableFormat(AudioFormat format){ return format.isEncodingPcm(); };
  virtual void setDualMonoMode(DualMonoMode dualMonoMode);
  virtual DualMonoMode getDualMonoMode(void){ return mDualMonoMode; };
  virtual void process(AudioBuffer& inBuf, AudioBuffer& outBuf);
  virtual std::string toString(void){ return "DualMonoFilter"; };
};
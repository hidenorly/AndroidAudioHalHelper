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

#include "IHdmiArcSink.hpp"

// TODO: will encapsule or use factory template, etc.

static std::shared_ptr<ISink> pSink;

std::shared_ptr<ISink> getHdmiArcSink(void)
{
  return std::make_shared<EncodedSink>( std::make_shared<IHdmiArcSink>(), /* trasncoder */ true );
}
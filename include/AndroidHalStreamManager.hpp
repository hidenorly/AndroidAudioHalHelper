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

#ifndef __ANDROID_HAL_STREAM_MANAGER_HPP__
#define __ANDROID_HAL_STREAM_MANAGER_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalStream.hpp"
#include <memory>
#include <map>

class AndroidStreamManager
{
protected:
  static inline std::map<AudioIoHandle, std::shared_ptr<IStream>> mStreams;

public:
  static void initialize(void);
  static void terminate(void);

  static void associateStream(AudioIoHandle ioHandle, std::shared_ptr<IStream> pStream);
  static void unassociateStream(AudioIoHandle ioHandle);
  static void unassociateStream(std::shared_ptr<IStream> pStream);

  static AudioIoHandle getAudioIoHandle(std::shared_ptr<IStream> pStream);
  static std::shared_ptr<IStream> getStream(AudioIoHandle ioHandle);
};

#endif /* __ANDROID_HAL_STREAM_MANAGER_HPP__ */

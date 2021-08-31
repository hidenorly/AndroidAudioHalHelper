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

#ifndef ANDROI_AUDIO_HAL_HELPER_STANDALONE_BUILD
#define ANDROI_AUDIO_HAL_HELPER_STANDALONE_BUILD 1
#endif /* ANDROI_AUDIO_HAL_HELPER_STANDALONE_BUILD */

// the following is dummy implementation to build this AndroidAudioHalHelper as standalone
// Should #define ANDROI_AUDIO_HAL_HELPER_STANDALONE_BUILD 0
// if you integrate AndroidAudioHalHelper as actual Android HAL
#if ANDROI_AUDIO_HAL_HELPER_STANDALONE_BUILD

#include <string>
#include "AndroidHalTypes.hpp"
#include <fmq/MessageQueue.h>

namespace android {
namespace hardware {

status_t EventFlag::createEventFlag(int fd, off_t offset, EventFlag** flag)
{
  return BAD_VALUE;
}

status_t EventFlag::createEventFlag(std::atomic<uint32_t>* fwAddr, EventFlag** flag)
{
  return BAD_VALUE;
}

status_t EventFlag::deleteEventFlag(EventFlag** evFlag)
{
  return BAD_VALUE;
}

namespace details {

void logAlwaysFatal(const char *message)
{

}

void logError(const std::string &message)
{

}

void check(bool bCheck)
{

}


}
}
}

extern "C"
{
int ashmem_valid(int fd)
{
 return 0;
}

int ashmem_create_region(const char *name, size_t size)
{
  return 0;
}

int ashmem_set_prot_region(int fd, int prot)
{
  return 0;
}

int ashmem_pin_region(int fd, size_t offset, size_t len)
{
  return 0;
}

int ashmem_unpin_region(int fd, size_t offset, size_t len)
{
  return 0;
}

int ashmem_get_size_region(int fd)
{
  return 0;
}

#include "cutils/native_handle.h"

int native_handle_close(const native_handle_t* h)
{
  return 0;
}

native_handle_t* native_handle_init(char* storage, int numFds, int numInts)
{
  return 0;
}

native_handle_t* native_handle_create(int numFds, int numInts)
{
  return 0;
}

native_handle_t* native_handle_clone(const native_handle_t* handle)
{
  return 0;
}

int native_handle_delete(native_handle_t* h)
{
  return 0;
}

}
#endif /* ANDROI_AUDIO_HAL_HELPER_STANDALONE_BUILD */
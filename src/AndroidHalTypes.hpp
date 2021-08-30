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

#ifndef __ANDROID_HAL_TYPES_HPP__
#define __ANDROID_HAL_TYPES_HPP__

#include <string>

enum HalResult {
  OK,
  NOT_INITIALIZED,
  INVALID_ARGUMENTS,
  INVALID_STATE,
  NOT_SUPPORTED
};

typedef uint8_t MacAddress[6];

struct DeviceAddress {
  uint32_t device;
  union Address {
    MacAddress mac;
    uint8_t ipv4[4];
    struct Alsa {
      int32_t card;
      int32_t device;
    } alsa;
  } address;
  std::string busAddress;
  std::string rSubmixAddress;
};

struct ParameterValue
{
  std::string key;
  std::string value;
};

typedef uint32_t AudioHwSync;

#endif /* __ANDROID_HAL_TYPES_HPP__ */

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

#ifndef __DEVICE_ADDRESS_HELPER_HPP__
#define __DEVICE_ADDRESS_HELPER_HPP__

#include <system/audio.h>
#include <stdint.h>
#include "AndroidHalTypes.hpp"
#include <map>
#include <tuple>

class AndroidDeviceAddressHelper
{
protected:
  static std::string getMacAddressString(MacAddress macAddress);
  static inline std::map<std::tuple<AudioDevice, std::string, std::string>, std::tuple<DeviceAddress, bool>> mDeviceConnected;
  static std::tuple<AudioDevice, std::string, std::string> getTupleFromDeviceAddreess(DeviceAddress deviceAddress);

public:
  static std::string getStringFromDeviceAddr(DeviceAddress deviceAddress);
  static DeviceAddress getDeviceAddrFromString(std::string deviceAddrString);

  static void setDeviceConnected(DeviceAddress deviceAddress, bool isConnected);
  static bool getDeviceConnected(DeviceAddress deviceAddress);

  static void initialize(void);
  static void terminate(void);
};

#endif /* __DEVICE_ADDRESS_HELPER_HPP__ */

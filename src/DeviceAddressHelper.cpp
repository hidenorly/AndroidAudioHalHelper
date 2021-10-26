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

#include "DeviceAddressHelper.hpp"
#include <sstream>

std::string AndroidDeviceAddressHelper::getMacAddressString(MacAddress macAddress)
{
  std::stringstream ss;

  for( int i=0; i<sizeof(MacAddress); i++ ){
    ss << std::hex << int(macAddress[i]);
  }

  return ss.str();
}

std::tuple<AudioDevice, std::string, std::string> AndroidDeviceAddressHelper::getTupleFromDeviceAddreess(DeviceAddress deviceAddress)
{
  return std::make_tuple( deviceAddress.device, getMacAddressString(deviceAddress.address.mac), deviceAddress.busAddress );
}

std::string AndroidDeviceAddressHelper::getStringFromDeviceAddr(DeviceAddress deviceAddress)
{
  // TODO : FIX this
  auto&& id = getTupleFromDeviceAddreess(deviceAddress);
  if( !mDeviceConnected.contains( id ) ){
    mDeviceConnected.insert_or_assign( id, std::make_tuple(deviceAddress, false) );
  }

  return deviceAddress.busAddress;
}

DeviceAddress AndroidDeviceAddressHelper::getDeviceAddrFromString(std::string deviceAddrString)
{
  DeviceAddress deviceAddr;
  deviceAddr.busAddress = deviceAddrString;
  return deviceAddr;
}

void AndroidDeviceAddressHelper::setDeviceConnected(DeviceAddress deviceAddress, bool isConnected)
{
  auto&& id = getTupleFromDeviceAddreess(deviceAddress);
  mDeviceConnected.insert_or_assign( id, std::make_tuple(deviceAddress, isConnected) );
}

bool AndroidDeviceAddressHelper::getDeviceConnected(DeviceAddress deviceAddress)
{
  bool result = false;

  auto&& id = getTupleFromDeviceAddreess(deviceAddress);
  if( mDeviceConnected.contains( id ) ){
    result = std::get<1>( mDeviceConnected[ id ] );
  }

  return result;
}

void AndroidDeviceAddressHelper::initialize(void)
{
  mDeviceConnected.clear();
}

void AndroidDeviceAddressHelper::terminate(void)
{
  mDeviceConnected.clear();
}

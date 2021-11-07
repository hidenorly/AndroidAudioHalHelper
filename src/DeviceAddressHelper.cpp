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
#include "StringTokenizer.hpp"
#include <sstream>

std::string AndroidDeviceAddressHelper::getMacAddressString(MacAddress macAddress)
{
  std::stringstream ss;

  for( int i=0; i<sizeof(MacAddress); i++ ){
    ss << std::hex << int(macAddress[i]);
  }

  return ss.str();
}

struct stDeviceString
{
  AudioDevice device;
  std::string description;
  stDeviceString(AudioDevice device, std::string description):device(device), description(description){};
};

const static stDeviceString tbl[] =
{
  stDeviceString(AUDIO_DEVICE_OUT_DEFAULT,        "AUDIO_DEVICE_OUT_DEFAULT"),
  stDeviceString(AUDIO_DEVICE_OUT_LINE,           "AUDIO_DEVICE_OUT_LINE"),
  stDeviceString(AUDIO_DEVICE_OUT_SPDIF,          "AUDIO_DEVICE_OUT_SPDIF"),
  stDeviceString(AUDIO_DEVICE_OUT_HDMI_EARC,      "AUDIO_DEVICE_OUT_HDMI_EARC"),
  stDeviceString(AUDIO_DEVICE_OUT_HDMI_ARC,       "AUDIO_DEVICE_OUT_HDMI_ARC"),
  stDeviceString(AUDIO_DEVICE_OUT_USB_DEVICE,     "AUDIO_DEVICE_OUT_USB_DEVICE"),
  stDeviceString(AUDIO_DEVICE_OUT_HDMI,           "AUDIO_DEVICE_OUT_HDMI"),
  stDeviceString(AUDIO_DEVICE_OUT_BLUETOOTH_A2DP, "AUDIO_DEVICE_OUT_BLUETOOTH_A2DP"),
  stDeviceString(AUDIO_DEVICE_OUT_SPEAKER,        "AUDIO_DEVICE_OUT_SPEAKER"),
  stDeviceString(AUDIO_DEVICE_OUT_BLUETOOTH_SCO,  "AUDIO_DEVICE_OUT_BLUETOOTH_SCO"),
  stDeviceString(AUDIO_DEVICE_IN_BUILTIN_MIC,     "AUDIO_DEVICE_IN_BUILTIN_MIC"),
  stDeviceString(AUDIO_DEVICE_IN_HDMI,            "AUDIO_DEVICE_IN_HDMI"),
  stDeviceString(AUDIO_DEVICE_IN_USB_DEVICE,      "AUDIO_DEVICE_IN_USB_DEVICE"),
  stDeviceString(AUDIO_DEVICE_IN_LINE,            "AUDIO_DEVICE_IN_LINE"),
  stDeviceString(AUDIO_DEVICE_IN_DEFAULT,         "AUDIO_DEVICE_IN_DEFAULT"),
  stDeviceString(AUDIO_DEVICE_IN_DEFAULT,         ""),
};


std::string AndroidDeviceAddressHelper::getDeviceString(AudioDevice device)
{
  std::string result;

  for( int i=0; tbl[i].device != AUDIO_DEVICE_IN_DEFAULT && !tbl[i].description.empty(); ){
    if( tbl[i].device == device ){
      result = tbl[i].description;
      break;
    }
  }

  return result;
}

AudioDevice AndroidDeviceAddressHelper::getDeviceFromDeviceString(std::string deviceString)
{
  AudioDevice result = AUDIO_DEVICE_OUT_DEFAULT;

  for( int i=0; tbl[i].device != AUDIO_DEVICE_IN_DEFAULT && !tbl[i].description.empty(); ){
    if( tbl[i].description == deviceString ){
      result = tbl[i].device;
      break;
    }
  }

  return result;
}

std::tuple<AudioDevice, std::string, std::string> AndroidDeviceAddressHelper::getTupleFromDeviceAddreess(DeviceAddress deviceAddress)
{
  return std::make_tuple( deviceAddress.device, getMacAddressString(deviceAddress.address.mac), deviceAddress.busAddress );
}

std::string AndroidDeviceAddressHelper::getStringFromDeviceAddr(DeviceAddress deviceAddress)
{
  auto&& id = getTupleFromDeviceAddreess(deviceAddress);
  if( !mDeviceConnected.contains( id ) ){
    mDeviceConnected.insert_or_assign( id, std::make_tuple(deviceAddress, false) );
  }

  // TODO : FIX this. Add the other identifier such as MacAddress, etc.
  return deviceAddress.busAddress.empty() ? getDeviceString(deviceAddress.device) : getDeviceString(deviceAddress.device) + ":" + deviceAddress.busAddress;
}

DeviceAddress AndroidDeviceAddressHelper::getDeviceAddrFromAudioPortConfig(const AudioPortConfig& port)
{
  DeviceAddress deviceAddr;
  if( port.type == AUDIO_PORT_TYPE_DEVICE ){
    deviceAddr.device = port.ext.device.type;
    deviceAddr.busAddress = port.ext.device.address;
  }
  return deviceAddr;
}

DeviceAddress AndroidDeviceAddressHelper::getDeviceAddrFromAudioPort(const AudioPort& port)
{
  DeviceAddress deviceAddr;
  if( port.type == AUDIO_PORT_TYPE_DEVICE ){
    deviceAddr.device = port.ext.device.type;
    deviceAddr.busAddress = port.ext.device.address;
  }
  return deviceAddr;
}


DeviceAddress AndroidDeviceAddressHelper::getDeviceAddrFromString(std::string deviceAddrString)
{
  DeviceAddress deviceAddr;
  StringTokenizer tok(deviceAddrString, ":");
  if( tok.hasNext() ){
    deviceAddr.device = getDeviceFromDeviceString( tok.getNext() );
    if( tok.hasNext() ){
      deviceAddr.busAddress = tok.getNext();
    }
  } else {
    deviceAddr.device = getDeviceFromDeviceString( deviceAddrString );
  }

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

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

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffectVisualizer.hpp"

IVisualizerEffect::IVisualizerEffect():IEffect( IVisualizerEffect::UUID ),mCaptureSize( 0 ),mScalingMode( IVisualizerEffect::ScalingMode::NORMALIZED ),mLatencyMs( 0 ),mMeasurementMode( IVisualizerEffect::MeasurementMode::NONE )
{

}

IVisualizerEffect::~IVisualizerEffect()
{

}

HalResult IVisualizerEffect::setCaptureSize(uint16_t captureSize)
{
  HalResult result = HalResult::OK;

  mCaptureSize = captureSize;

  return result;
}

uint16_t IVisualizerEffect::getCaptureSize(void)
{
  return mCaptureSize;
}

HalResult IVisualizerEffect::setScalingMode(ScalingMode scalingMode)
{
  HalResult result = HalResult::OK;

  mScalingMode = scalingMode;

  return result;
}

IVisualizerEffect::ScalingMode IVisualizerEffect::getScalingMode(void)
{
  return mScalingMode;
}

HalResult IVisualizerEffect::setLatency(uint32_t latencyMs)
{
  HalResult result = HalResult::OK;

  mLatencyMs = latencyMs;

  return result;
}

uint32_t IVisualizerEffect::getLatency(void)
{
  return mLatencyMs;
}

HalResult IVisualizerEffect::setMeasurementMode(MeasurementMode measurementMode)
{
  HalResult result = HalResult::OK;

  mMeasurementMode = measurementMode;

  return result;
}

IVisualizerEffect::MeasurementMode IVisualizerEffect::getMeasurementMode(void)
{
  return mMeasurementMode;
}

std::vector<uint8_t> IVisualizerEffect::capture(void)
{
  std::vector<uint8_t> result;

  return result;
}

IVisualizerEffect::Measurement IVisualizerEffect::measure(void)
{
  IVisualizerEffect::Measurement result;

  return result;
}


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

#ifndef __ANDROID_HAL_VISUALIZER_EFFECT_HPP__
#define __ANDROID_HAL_VISUALIZER_EFFECT_HPP__

#include "AndroidHalTypes.hpp"
#include "AndroidHalEffect.hpp"

class IVisualizerEffect : public IEffect
{
public:
  IVisualizerEffect();
  virtual ~IVisualizerEffect();

  static inline const char* UUID = "37cc2c00-dddd-11db-8577-0002a5d5c51b";
  _EffectGetInstanceIfUuidMatch(IVisualizerEffect);

public:
  enum CaptureSizeRange
  {
    MAX = 1024,
    MIN = 128
  };

  enum ScalingMode
  {
    NORMALIZED = 0,
    AS_PLAYED = 1
  };

  enum MeasurementMode
  {
    NONE = 0x0,
    PEAK_RMS = 0x1
  };

  struct Measurement {
    MeasurementMode mode;
    union Values {
      struct PeakAndRms {
        int32_t peakMb;
        int32_t rmsMb;
        PeakAndRms():peakMb(0), rmsMb(0){};
      } peakAndRms;
      Values(){};
    } value;
    Measurement():mode(MeasurementMode::NONE){};
  };

protected:
  uint16_t mCaptureSize;
  ScalingMode mScalingMode;
  uint32_t mLatencyMs;
  MeasurementMode mMeasurementMode;

public:
  virtual HalResult setCaptureSize(uint16_t captureSize);
  virtual uint16_t getCaptureSize(void);
  virtual HalResult setScalingMode(ScalingMode scalingMode);
  virtual ScalingMode getScalingMode(void);
  virtual HalResult setLatency(uint32_t latencyMs);
  virtual uint32_t getLatency(void);
  virtual HalResult setMeasurementMode(MeasurementMode measurementMode);
  virtual MeasurementMode getMeasurementMode(void);
  virtual std::vector<uint8_t> capture(void);
  virtual Measurement measure(void);
};

#endif /* __ANDROID_HAL_VISUALIZER_EFFECT_HPP__ */

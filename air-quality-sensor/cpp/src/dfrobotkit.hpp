/*
* Copyright (c) 2015 - 2016 Intel Corporation.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DFROBOTKIT_HPP_
#define DFROBOTKIT_HPP_

#include <mraa.hpp>
#include "mq2.hpp"

#define WARNING_THRESHOLD 200

using namespace std;

struct Devices
{
  mraa::Gpio* buzzer;
  upm::MQ2* sensor;

  Devices() {
  };

  // Initialization function
  void init() {
  	// buzzer connected to digital 15 aka A1
    buzzer = new mraa::Gpio(15);
    buzzer->dir(mraa::DIR_OUT);
    stop_buzzing();

    // air sensor connected to analog A2
    sensor = new upm::MQ2(2);
  };

  // Starts the buzzer making noise
  void start_buzzing() {
    buzzer->write(1);
  }

  // Stops the buzzer making noise
  void stop_buzzing() {
    buzzer->write(0);
  }

  // Sounds an audible alarm
  void alarm() {
    start_buzzing();
    // sleep for 1s
    usleep(1000000);
    stop_buzzing();
  }

  // Cleanup on exit
  void cleanup() {
    delete buzzer;
    delete sensor;
  }

  // How is the air quality?
  string air_quality(uint16_t value)
  {
    if(value < 30) return "Fresh Air";
    if(value < 100) return "Normal Indoor Air";
    if(value < 200) return "Low Pollution";
    if(value < 300) return "High Pollution - Action Recommended";
    return "Very High Pollution - Take Action Immediately";
  }

  // Check the air qulity by reading the sensor
  uint16_t checkAirQuality(){
    // read sensor value
    uint16_t value = sensor->getSample();

    // read CO ppm (can vary slightly from previous read)
    fprintf(stdout, "Air quality: %4d   %s\n", value, air_quality(value).c_str());

    return value;
  }
};

#endif /* DFROBOTKIT_HPP_ */

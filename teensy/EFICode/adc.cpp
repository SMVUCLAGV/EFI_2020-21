#include "adc.h"

#include "Arduino.h"
#include "Constants.h"

ADC::ADC() {
  // Attach Interrupt

  // Setup SPI

  // MAX11624  Setup
}

void ADC::refresh() {
  convTime = micros();

  // Pull CNVST low for atleast 40 ns to start conversion
  validVals = false;
}

const int * ADC::getChannels() {
  if (validVals) {
    if (fetchVals) {
      // GET ALL VALUES FROM ADC AND STORE IN valChannel
      fetchVals = false;
    }

    return valChannel;
  }

  return ERRORCHANNELS; // obvious error (hopefully)
}

void ADC::handle_EOC() {
  validVals = true;
  fetchVals = true;
}

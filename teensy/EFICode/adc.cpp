#include "adc.h"

#include "Arduino.h"
#include "SPI.h"
#include "Constants.h"

ADC::ADC() {

  // Conversion Start Pin
  pinMode(ADC_nCNVST_PIN, OUTPUT);
  digitalWrite(ADC_nCNVST_PIN, HIGH);

  // End-of-Conversion Pin, Attach Interrupt
  attachInterrupt(digitalPinToInterrupt(ADC_nEOC_PIN), this.dummy, FALLING);
  detachInterrupt(digitalPinToInterrupt(ADC_nEOC_PIN));
  attachInterrupt(digitalPinToInterrupt(ADC_nEOC_PIN), this.handle_EOC, FALLING);

  // Setup SPI
  pinMode(ADC_nCS_PIN, OUTPUT);
  digitalWrite(ADC_nCS_PIN, HIGH);

  SPI.begin();

  // MAX11624  Setup
  SPI.beginTransaction(SPI_SETTINGS);

    // Select ADC
  digitalWrite(ADC_nCS_PIN, LOW);
    // Write to conversion, setup, and avg registers
  SPI.transfer(MAX11624_CONV_REG);
  SPI.transfer(MAX11624_SETUP_REG);
  SPI.transfer(MAX11624_AVG_REG);
    // De-Select ADC
  digitalWrite(ADC_nCS_PIN, HIGH);

  SPI.endTransaction();
}


void ADC::refresh() {

  // Get Conversion Time
  convTime = micros();

  // Pull CNVST low for atleast 40 ns to start conversion
  digitalWrite(ADC_nCNVST_PIN, LOW);
  int temp = micros();
  while( micros < temp + 1); // waiting 100 ns (CHANGE TO TIMER?)
  digitalWrite(ADC_nCNVST_PIN, HIGH);
  validVals = false;
}

const int * ADC::getChannels() {
  if (validVals) {
    if (fetchVals) {
      // Get and store values from ADC FIFO
      SPI.beginTransaction(SPI_SETTINGS);

        // Select ADC
      digitalWrite(ADC_nCS_PIN, LOW);

        // Read from ADC's FIFO
      int data = 0;
      for (int i = 0; i < ADC_CHANNELS; i++) {
        data |= (int(SPI.transfer(0)) << 8); // MSBs first
        data |= int(SPI.transfer(0));        // LSBs next
        valChannel[i] = data;
      }

        // De-Select ADC
      digitalWrite(ADC_nCS_PIN, HIGH);

      SPI.endTransaction();
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

void ADC::dummy() {;}

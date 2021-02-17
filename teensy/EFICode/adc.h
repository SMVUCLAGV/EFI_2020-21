// Author: Arhison

// MAX11624 ADC Class:
//  Description:
//  - refreshVals()
//    - update all sensor values, make sensor values invalid
//  - validVals()
//    - check if the values available are valid (since refreshVals was last called)
//  - getChannel(int n)
//    - get most recent value from Channel N of the ADC

// MAX11624 Theory of Operation:
//  Here we use the Clock Mode option 00. This lets us use CNVST to start a 
//    conversion for all channels (done by calling refresh()). Then we wait 
//    for EOC (end-of-conversion) (captured by interrupt) and read from the adc.
//    Use validVals() to learn when EOC has arrived and new values are 
//    available.

#ifndef ADC_H
#define ADC_H

#include "Constants.h"
#include "SPI.h"

// ADC SPI Communication Pins
const int ADC_nCNVST_Pin = 9;
const int ADC_nCS_Pin    = 10;
const int ADC_nEOC_Pin   = 29;
const int SPI_SCLK_Pin   = 27; // THIS MAY NOT WORK, MIGHT NEED TO BODGE THIS CONNECTION TO PIN GPIO 13
const int SPI_MOSI_Pin   = 11;
const int SPI_MISO_Pin   = 12;

// MAX11624 Register Values:
const char MAX11624_CONV_REG  = (0b1<<7)|(0b1111<<3)|(0b00<<0);  
                                // (CONV_REG)|(CHAN N = 15)|(SCAN THROUGH CHAN N)
const char MAX11624_SETUP_REG = (0b01<<6)|(0b00<<4)|(0b01<<2)|(0b00<<0);  
                                // (SETUP_REG)|(CLOCK MODE 00)|(EXT REF)|(D.C)
const char MAX11624_AVG_REG   = (0b001<<5)|(0b1<<4)|(0b00<<2)|(0b00<<0);  
                                // (AVG_REG)|(ENABLE AVG)|(AVG 4X)|(D.C)

class ADC {
public:
  // Constructor
  ADC();

  // Refresh all ADC values
  void refresh();

  // See if values are valid
  inline int validVals() {
    return validVals;
  }

  // Getter for value at channel n
  const int * getChannels();
  inline int ADC::getConvTime() {
    return convTime;
  }
  
private:
  void handle_EOC(); // EOC interrupt
  volatile bool validVals;
  volatile bool fetchVals;
  int convTime;
  int valChannel[16];
  int ERRORCHANNELS[16] = { -1, -1, -1, -1, -1, -1, -1, -1, 
                            -1, -1, -1, -1, -1, -1, -1, -1};
};

#endif

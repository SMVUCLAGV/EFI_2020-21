// Author: Arhison

// MAX11624 ADC Class:
//  Description:
//  - refreshVals()
//    - update all sensor values, make sensor values invalid
//  - validVals()
//    - check if the values available are valid (since refreshVals was last called)
//  - getChannel(int n)
//    - get most recent value from Channel N of the ADC

#ifndef ADC_H
#define ADC_H

#include "Constants.h"
#include "SPI.h"

// ADC SPI Communication Pins
const int ADC_nCNVST_Pin = 0;
const int ADC_nCS_Pin = 0;
const int ADC_nEOC_Pin = 0;
const int SPI_SCLK_Pin = 0;
const int SPI_MOSI_Pin = 0;
const int SPI_MISO_Pin = 0;

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
  int getChannel(int n);
  
private:
  bool validVals;
  int valChannel[16];
};

#endif

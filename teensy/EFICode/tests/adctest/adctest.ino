// Test file for adc class
#include "spi_adc.h"

SPI_ADC* a;
void setup() {
  Serial.begin(9600);
  a = new SPI_ADC();
}

void loop() {
  delay(20);
  a->refresh();

  // Data should be invalid here
  const int * data = a->getChannels();
  for (int i = 0; i < ADC_CHANNELS; i++) {
    Serial.print(data[i]);
    Serial.print(", ");
  }
  Serial.print("\n");

  delay(20);
  
  // Get Data once then print it
  data = a->getChannels();
  for (int i = 0; i < ADC_CHANNELS; i++) {
    Serial.print(data[i]);
    Serial.print(", ");
  }
  Serial.print("\n");

  // Get Data agains to make sure it doesn't change
  data = a->getChannels();
  for (int i = 0; i < ADC_CHANNELS; i++) {
    Serial.print(data[i]);
    Serial.print(", ");
  }
  Serial.print("\n");

  Serial.println(a->getConvTime());
}

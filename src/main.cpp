#include <Arduino.h>

// -----Global variables-----
const bool shouldPrintDebug = true; // If true, debug values can be printed to the console via the serial interface.
const int adcPin = A0;              // Pint A0 of the Wemos D1 Mini will be used as an ADC.

// -----Declerations-----
void serialSetup(int baudRate);
void serialPrintDebug(String s);
void serialPrintLineDebug(String s);
void testAdc(int sleepTime, uint8_t adcReadPin);

/**
 * The Wemos D1 mini is setup using this code block
 *
 * @return void
 */
void setup()
{
  serialSetup(115200);

  serialPrintLineDebug("Board setup complete...");
}

/**
 * Main loop
 *
 * @return void
 */
void loop()
{
  testAdc(2000, adcPin);
}

/**
 * Setup the serial interface of the board to the specified baudrate.
 *
 * @param baudRate The baudrate to use.
 * @return void
 */
void serialSetup(int baudRate)
{
  Serial.begin(baudRate);
  serialPrintLineDebug("");
  serialPrintLineDebug("Serial setup done.");
}

/**
 * Prints a string to the console. 
 * A global debug flag is checked to determine if output should be generated.
 *
 * @param s The string that will be printed to the console
 * @return void
 */
void serialPrintDebug(String s)
{
  if (shouldPrintDebug)
    Serial.print(s);
}

/**
 * Prints a string to the console and appends a new line. 
 * A global debug flag is checked to determine if output should be generated.
 *
 * @param s The string that will be printed to the console
 * @return void
 */
void serialPrintLineDebug(String s)
{
  if (shouldPrintDebug)
    Serial.println(s);
}

/**
 * Take an ADC measurement for the given pin, display the value and sleep for a given time.
 *
 * @param sleepTime sleep time in ms after reading and printing result
 * @param adcReadPin Pin on the Wemos D1 mini to read
 * @return void
 */
void testAdc(int sleepTime, uint8_t adcReadPin)
{
  int adcValue = analogRead(adcReadPin);
  serialPrintLineDebug("ADC value: " + String(adcValue));
  delay(sleepTime);
}
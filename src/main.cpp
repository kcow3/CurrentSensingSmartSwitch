#include <Arduino.h>

// Global variables
bool shouldPrintDebug = true;

// Declerations
void serialSetup(int baudRate);
void serialPrintDebug(String s);
void serialPrintLineDebug(String s);

void setup()
{
  serialSetup(115200);
  
  serialPrintLineDebug("Board setup complete...");
}

void loop()
{
}

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

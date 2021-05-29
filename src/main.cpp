#include <Arduino.h>
#include <FastLED.h>

// -----Pin Assignments-----
#define ADC_PIN A0 // Pin A0 of the Wemos D1 Mini will be used as an ADC.
#define LED_PIN 4  // Pin D4 will be used to control a single NEO-Pixel LED.

// -----Global variables-----
const bool SHOULD_PRINT_DEBUG = true; // If true, debug values can be printed to the console via the serial interface.
#define NUM_LEDS 1                    // Number of Neo-pixel LEDs to use
CRGB leds[NUM_LEDS];                  // Define LED array that will be used to control an indicator LED.

// -----Declerations-----
void serialSetup(int baudRate);
void serialPrintDebug(String s);
void serialPrintLineDebug(String s);
void testAdc(int sleepTime, uint8_t adcReadPin);
void setupIndicatorLed();

/**
 * The Wemos D1 mini is setup using this code block
 *
 * @return void
 */
void setup()
{
  serialSetup(115200);
  setupIndicatorLed();

  serialPrintLineDebug("Board setup complete...");
}

/**
 * Main loop
 *
 * @return void
 */
void loop()
{
  testAdc(500, ADC_PIN);
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
  if (SHOULD_PRINT_DEBUG)
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
  if (SHOULD_PRINT_DEBUG)
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

  if (adcValue >= 0 && adcValue <= 20)
  {
    leds[0] = CRGB::Green;
    FastLED.show();
    delay(10);
  }

  if (adcValue > 20 && adcValue <= 100)
  {
    leds[0] = CRGB::Blue;
    FastLED.show();
    delay(10);
  }

  if (adcValue > 100)
  {
    leds[0] = CRGB::Red;
    FastLED.show();
    delay(10);
  }

  delay(sleepTime);
}

void setupIndicatorLed()
{
  serialPrintLineDebug("Setup Fast LED in order GRB...");
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  leds[0] = CRGB::Black;
  FastLED.show();
  serialPrintLineDebug("Fast LED setup done...");
}
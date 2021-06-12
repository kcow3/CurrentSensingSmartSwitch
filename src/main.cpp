#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// -----Pin Assignments-----
#define ADC_PIN A0 // Pin A0 of the Wemos D1 Mini will be used as an ADC.
#define LED_PIN 4  // Pin D4 will be used to control a single NEO-Pixel LED.

// -----Global variables-----
const bool SHOULD_PRINT_DEBUG = true; // If true, debug values can be printed to the console via the serial interface.
#define NUM_LEDS 1                    // Number of Neo-pixel LEDs to use
CRGB leds[NUM_LEDS];                  // Define LED array that will be used to control an indicator LED.

// -----Network setup-----
WiFiClient espClient;
const char *ssid = "ssid here";
const char *password = "password here";
const char *mqtt_server = "ip address here";
const String hostName = "unique hostname here";

// -----MQTT setup-----
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// -----Declerations-----
void Callback(char *topic, byte *payload, unsigned int length);
void SerialSetup(int baudRate);
void SerialPrintDebug(String s);
void SerialPrintLineDebug(String s);
void TestAdc(int sleepTime, uint8_t adcReadPin);
void SetupIndicatorLed();
void SetIndicatorLedColour(CRGB colour);
void SetupWiFi();
void Reconnect();
void GetWifiHeartbeat();
String IpAddressToString(const IPAddress &ipAddress);

/**
 * The Wemos D1 mini is setup using this code block
 *
 * @return void
 */
void setup()
{
  // Setup serial communication
  SerialSetup(115200);

  // Setup LED indicator
  SetupIndicatorLed();

  // Setup Networking
  pinMode(LED_BUILTIN, OUTPUT);
  SetupWiFi();

  SerialPrintLineDebug("Board setup complete...");
}

/**
 * Main loop
 *
 * @return void
 */
void loop()
{
  TestAdc(2000, ADC_PIN);
  GetWifiHeartbeat();
}

/**
 * Setup the serial interface of the board to the specified baudrate.
 *
 * @param baudRate The baudrate to use.
 * @return void
 */
void SerialSetup(int baudRate)
{
  Serial.begin(baudRate);
  SerialPrintLineDebug("");
  SerialPrintLineDebug("Serial setup done...");
}

/**
 * Prints a string to the console. 
 * A global debug flag is checked to determine if output should be generated.
 *
 * @param s The string that will be printed to the console
 * @return void
 */
void SerialPrintDebug(String s)
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
void SerialPrintLineDebug(String s = "")
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
void TestAdc(int sleepTime, uint8_t adcReadPin)
{
  int adcValue = analogRead(adcReadPin);
  SerialPrintLineDebug("ADC value: " + String(adcValue));
  delay(sleepTime);
}

/**
 * Setup the Neo-Pixel LED indicator.
 *
 * @return void
 */
void SetupIndicatorLed()
{
  SerialPrintLineDebug("Setup Fast LED in order GRB...");
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  SetIndicatorLedColour(CRGB::Black);
  SerialPrintLineDebug("Fast LED setup done...");
}

/**
 * Set the colour of the connected LED indicator as specified.
 *
 * @param colour Set the indicator to colour (CRGB::<Colour here)
 * @return void
 */
void SetIndicatorLedColour(CRGB colour)
{
  leds[0] = colour;
  FastLED.show();
}

/**
 * Setup the WiFi connection for the Wemos D1 mini.
 *
 * @return void
 */
void SetupWiFi()
{
  SerialPrintLineDebug("Starting WiFi setup...");

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostName.c_str());
  WiFi.begin(ssid, password);

  SerialPrintDebug("Connecting...");

  while (WiFi.status() != WL_CONNECTED)
  {
    SerialPrintDebug(".");
    delay(1000);
    SetIndicatorLedColour(CRGB::Red);
  }

  // Persist WiFi connection details
  WiFi.persistent(true);

  //  Print out connection details
  SerialPrintLineDebug();
  SerialPrintLineDebug("WiFi connection established...");
  SerialPrintDebug("IP address:\t");
  SerialPrintLineDebug(IpAddressToString(WiFi.localIP()));
}

/**
 * Update the indicator LED based on the status of the WiFi connection
 *
 * @return void
 */
void GetWifiHeartbeat()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    SetIndicatorLedColour(CRGB::Red);
  }
  else
  {
    SetIndicatorLedColour(CRGB::Green);
  }
}

// TODO: MQTT
void Callback(char *topic, byte *payload, unsigned int length)
{
}

// TODO: MQTT
void Reconnect()
{
}

/**
 * Prints out an IP address in the format xxx.xxx.xxx.xxx
 * @param ipAddress The IP Address to print
 * @return void
 */
String IpAddressToString(const IPAddress &ipAddress)
{
  if (!ipAddress.isSet())
  {
    return "No ip address set";
  }

  return String(ipAddress[0]) + String(".") +
         String(ipAddress[1]) + String(".") +
         String(ipAddress[2]) + String(".") +
         String(ipAddress[3]);
}
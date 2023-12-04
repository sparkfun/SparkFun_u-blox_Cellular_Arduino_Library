#include "SparkFun_u-blox_Cellular_Arduino_Library.h"

// Uncomment the line below that you need for Serial on your platform
#define mySerial Serial1
// SoftwareSerial mySerial(16, 17);

// Uncomment the module you're using. If your module is not listed below, then
// it's not supported for this example
UBX_CELL myModule; // This example works with all modules, so the base class can be used
// SARA_R5 myModule; // Base SARA-R5 class
// SARA_R500S myModule;
// SARA_R500S_01B myModule;
// SARA_R500S_61B myModule;
// SARA_R510M8S_61B myModule;
// SARA_R510S myModule;
// LARA_R6 myModule; // Base LARA-R6 class
// LARA_R6001 myModule;
// LARA_R6001D myModule;
// LARA_R6401 myModule;
// LARA_R6401D myModule;
// LARA_R6801_00B myModule;
// LARA_R6801D myModule;

String pingMe = ""; // The name of the server we are going to ping

// processPingResult is provided to the u-blox cellular library via a 
// callback setter -- setPingCallback. (See the end of setup())
void processPingResult(int retry, int p_size, String remote_hostname, IPAddress ip, int ttl, long rtt)
{
  Serial.println();
  Serial.print(F("Ping Result:  Retry #:"));
  Serial.print(retry);
  Serial.print(F("  Ping Size (Bytes):"));
  Serial.print(p_size);
  Serial.print(F("  Remote Host:\""));
  Serial.print(remote_hostname);
  Serial.print(F("\"  IP Address:\""));
  Serial.print(String(ip[0]));
  Serial.print(F("."));
  Serial.print(String(ip[1]));
  Serial.print(F("."));
  Serial.print(String(ip[2]));
  Serial.print(F("."));
  Serial.print(String(ip[3]));
  Serial.print(F("\"  Time To Live (hops):"));
  Serial.print(ttl);
  Serial.print(F("  Round Trip (ms):"));
  Serial.print(rtt);
  Serial.println();
}

void setup()
{
  String currentOperator = "";

  Serial.begin(115200); // Start the serial console

  // Wait for user to press key to begin
  Serial.println(F("u-blox Cellular Example 5 - Ping"));
  Serial.println(F("Press any key to begin"));
  
  while (!Serial.available()) // Wait for the user to press a key (send any serial character)
    ;
  while (Serial.available()) // Empty the serial RX buffer
    Serial.read();

  Serial.println(F("Beginning..."));

  // myModule.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  // For the MicroMod Asset Tracker, we need to invert the power pin so it pulls high instead of low
  // Uncomment the next line if required
  // myModule.invertPowerPin(true); 

  // Initialize the module
  if (myModule.begin(mySerial, UBX_CELL_DEFAULT_BAUD_RATE) )
  {
    Serial.println(F("Module connected!"));
  }
  else
  {
    Serial.println(F("Unable to communicate with the module."));
    Serial.println(F("Manually power-on (hold the module's On button for 3 seconds) and try again."));
    while (1) ; // Loop forever on fail
  }
  Serial.println();

  // First check to see if we're connected to an operator:
  if (myModule.getOperator(&currentOperator) == UBX_CELL_SUCCESS)
  {
    Serial.print(F("Connected to: "));
    Serial.println(currentOperator);
  }
  else
  {
    Serial.print(F("The module is not yet connected to an operator. Please use the previous examples to connect. Or wait and retry. Freezing..."));
    while (1)
      ; // Do nothing more
  }

  Serial.println();
  Serial.println(F("*** Set the Serial Monitor line ending to Newline ***"));

  Serial.println();
  Serial.println(F("Enter the name of the server you want to ping (followed by LF / Newline): "));
  Serial.println(F("Example: \"www.google.com\""));

  // Set a callback to process the Ping result
  myModule.setPingCallback(&processPingResult);
}

void loop()
{
  if (Serial.available())
  {
    char c = Serial.read();
    if (c == '\n')
    {
      // Newline received so let's do that ping!
      Serial.println("Pinging " + pingMe + "...");
      myModule.ping(pingMe); // Use the default parameters
      
      // Use custom parameters
      //int retries = 4; // number of retries
      //int p_size = 32; // packet size (bytes)
      //unsigned long timeout = 5000; // timeout (ms)
      //int ttl = 32; // Time To Live
      //myModule.ping(pingMe, retries, p_size, timeout, ttl);
      
      pingMe = ""; // Clear the server name for the next try
    }
    else
    {
      // Add serial characters to the server address
      pingMe += c;
    }
  }
  
  myModule.poll(); // Keep processing data from the module so we can catch the Ping result
}
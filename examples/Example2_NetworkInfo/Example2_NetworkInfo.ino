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

// Map registration status messages to more readable strings
String registrationString[] =
{
  "Not registered",                         // 0
  "Registered, home",                       // 1
  "Searching for operator",                 // 2
  "Registration denied",                    // 3
  "Registration unknown",                   // 4
  "Registered, roaming",                    // 5
  "Registered, home (SMS only)",            // 6
  "Registered, roaming (SMS only)",         // 7
  "Registered, emergency service only",     // 8
  "Registered, home, CSFB not preferred",   // 9
  "Registered, roaming, CSFB not prefered"  // 10
};

// If you are based in Europe, you will (probably) need to select MNO_STD_EUROPE
const mobile_network_operator_t MOBILE_NETWORK_OPERATOR = MNO_GLOBAL;

void setup()
{
  Serial.begin(115200); // Start the serial console

  // Wait for user to press key to begin
  Serial.println(F("u-blox Cellular Example 2 - Network Info"));
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

  if (!myModule.setNetworkProfile(MOBILE_NETWORK_OPERATOR))
  {
    Serial.println(F("Error setting network. Try cycling the power."));
    while (1) ;
  }
  
  Serial.println(F("Network profile set. Ready to go!"));
  
  // RSSI: Received signal strength:
  Serial.println("RSSI: " + String(myModule.rssi()));
  // Registration Status
  int regStatus = myModule.registration();
  if ((regStatus >= 0) && (regStatus <= 10))
  {
    Serial.println("Network registration: " + registrationString[regStatus]);
  }

  // Print the Context IDs, Access Point Names and IP Addresses
  Serial.println(F("Available PDP (Packet Data Protocol) APNs (Access Point Names) and IP Addresses:"));
  Serial.println(F("Context ID:\tAPN Name:\tIP Address:"));
  for (int cid = 0; cid < UBX_CELL_NUM_PDP_CONTEXT_IDENTIFIERS; cid++)
  {
    String apn = "";
    IPAddress ip(0, 0, 0, 0);
    myModule.getAPN(cid, &apn, &ip);
    if (apn.length() > 0)
    {
      Serial.print(cid);
      Serial.print(F("\t\t"));
      Serial.print(apn);
      Serial.print(F("\t"));
      Serial.println(ip);
    }
  }

  Serial.println();
  
  if (regStatus > 0)
  {
    Serial.println(F("All set. Go to the next example!"));
  }
}

void loop()
{
  // Do nothing. Now that we're registered move on to the next example.
}
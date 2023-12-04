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

void setup()
{
  String currentOperator = "";

  Serial.begin(115200); // Start the serial console

  // Wait for user to press key to begin
  Serial.println(F("u-blox Cellular Example 6 - Receive SMS"));
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

  while (Serial.available()) // Empty the serial RX buffer
    Serial.read();
}

void loop()
{
  static bool printReadMessages = true; // Print all messages once. Then only print new messages. Unless a message is deleted.
  static int previousUsed = -1; // Store the previous number of used memory locations
  
  // Read the number of used and total messages
  int used;
  int total;
  if (myModule.getPreferredMessageStorage(&used, &total) != UBX_CELL_SUCCESS)
  {
    Serial.println(F("An error occurred when trying to read ME memory!"));
  }
  else
  {
    if ((used != previousUsed) || printReadMessages) // Has a new message arrived? Or was the delete menu opened?
    {
      Serial.print(F("\r\nNumber of used memory locations: "));
      Serial.println(used);
      Serial.print(F("Total number of memory locations: "));
      Serial.println(total);
      Serial.println();

      int memoryLocation = 0;
      int foundMessages = 0;
      // Keep reading until we find all the messages or we reach the end of the memory
      while ((foundMessages < used) && (memoryLocation <= total))
      {
        String unread = "";
        String from = "";
        String dateTime = "";
        String message = "";
        // Read the message from this location. Reading from empty message locations returns an ERROR
        // unread can be: "REC UNREAD", "REC READ", "STO UNSENT", "STO SENT"
        // If the location is empty, readSMSmessage will return a UBX_CELL_ERROR_UNEXPECTED_RESPONSE
        if (myModule.readSMSmessage(memoryLocation, &unread, &from, &dateTime, &message) == UBX_CELL_SUCCESS)
        {
          if (printReadMessages || (unread == "REC UNREAD")) 
          {
            Serial.print(F("Message location: "));
            Serial.println(memoryLocation);
            Serial.print(F("Status: "));
            Serial.println(unread);
            Serial.print(F("Originator: "));
            Serial.println(from);
            Serial.print(F("Date and time: "));
            Serial.println(dateTime);
            Serial.println(message);
            Serial.println();
          }
          foundMessages++; // We found a message
        }
        memoryLocation++; // Move on to the next memory location
      }

      printReadMessages = false;
      previousUsed = used; // Update previousUsed

      Serial.println(F("Waiting for a new message..."));
      Serial.println();
      Serial.println(F("Hit any key to delete a message..."));
      Serial.println();
    }
  }

  int delayCount = 0;
  while (delayCount < 5000)
  {
    delay(1); // Delay for five seconds, unless the user presses a key
    delayCount++;

    if (Serial.available())
    {
      Serial.println(F("To delete a single message:                        enter its location followed by LF / Newline"));
      Serial.println(F("To delete all read messages:                       enter r followed by LF / Newline"));
      Serial.println(F("To delete all read and sent messages:              enter s followed by LF / Newline"));
      Serial.println(F("To delete all read, sent and unsent messages:      enter u followed by LF / Newline"));
      Serial.println(F("To delete all messages, including unread messages: enter a followed by LF / Newline"));
      Serial.println(F("To exit:                                           enter LF / Newline"));

      Serial.read(); // Read and discard the char that opened the menu

      int location = 0;
      bool selected = false;
      while (!selected)
      {
        while (!Serial.available()) ; // Wait for a character to arrive
        char c = Serial.read(); // Read it
        if (c == '\n') // Is it a LF?
        {
          if ((location >= 1) && (location <= total)) // Delete a single message at location
          {
            if (myModule.deleteSMSmessage(location) == UBX_CELL_SUCCESS)
            {
              Serial.println(F("\r\nMessage deleted!\r\n"));
              printReadMessages = true;
            }
            else
            {
              Serial.println(F("\r\nMessage not deleted!\r\n"));
            }
          }
          else if (location == 1001) // r
          {
            if (myModule.deleteReadSMSmessages() == UBX_CELL_SUCCESS)
            {
              Serial.println(F("\r\nRead messages deleted!\r\n"));
              printReadMessages = true;
            }
            else
            {
              Serial.println(F("\r\nMessages not deleted!\r\n"));
            }
          }
          else if (location == 1002) // s
          {
            if (myModule.deleteReadSentSMSmessages() == UBX_CELL_SUCCESS)
            {
              Serial.println(F("\r\nRead and sent messages deleted!\r\n"));
              printReadMessages = true;
            }
            else
            {
              Serial.println(F("\r\nMessages not deleted!\r\n"));
            }
          }
          else if (location == 1003) // u
          {
            if (myModule.deleteReadSentUnsentSMSmessages() == UBX_CELL_SUCCESS)
            {
              Serial.println(F("\r\nRead, sent and unsent messages deleted!\r\n"));
              printReadMessages = true;
            }
            else
            {
              Serial.println(F("\r\nMessages not deleted!\r\n"));
            }
          }
          else if (location == 1004) // a
          {
            if (myModule.deleteAllSMSmessages() == UBX_CELL_SUCCESS)
            {
              Serial.println(F("\r\nAll messages deleted!\r\n"));
              printReadMessages = true;
            }
            else
            {
              Serial.println(F("\r\nMessages not deleted!\r\n"));
            }
          }
          else
            Serial.println(F("\r\nExit...\r\n"));
          selected = true;
        }
        else if ((c >= '0') && (c <= '9'))
        {
          location *= 10; // Multiply by 10
          location += c - '0'; // Add the digit
        }
        else if (c == 'r')
        {
          location = 1001;
        }
        else if (c == 's')
        {
          location = 1002;
        }
        else if (c == 'u')
        {
          location = 1003;
        }
        else if (c == 'a')
        {
          location = 1004;
        }
      }

      delayCount = 5000;
    }
  }
}
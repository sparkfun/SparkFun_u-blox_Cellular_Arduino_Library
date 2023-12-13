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
String registrationString[] = {
    "Not registered",                        // 0
    "Registered, home",                      // 1
    "Searching for operator",                // 2
    "Registration denied",                   // 3
    "Registration unknown",                  // 4
    "Registered, roaming",                   // 5
    "Registered, home (SMS only)",           // 6
    "Registered, roaming (SMS only)",        // 7
    "Registered, emergency service only",    // 8
    "Registered, home, CSFB not preferred",  // 9
    "Registered, roaming, CSFB not prefered" // 10
};

// If you are based in Europe, you will (probably) need to select MNO_STD_EUROPE
const mobile_network_operator_t MOBILE_NETWORK_OPERATOR = MNO_GLOBAL;

const String MOBILE_NETWORK_STRINGS[] = {"default (Undefined/Regulatory)",
                                         "SIM ICCID",
                                         "AT&T",
                                         "Verizon",
                                         "Telstra",
                                         "T-Mobile US",
                                         "China Telecom",
                                         "Sprint",
                                         "Vodafone",
                                         "NTT DoCoMo",
                                         "Telus",
                                         "SoftBank",
                                         "Deutsche Telekom",
                                         "US Cellular",
                                         "SKT",
                                         "global (factory default)",
                                         "standard Europe",
                                         "standard Europe No-ePCO",
                                         "NOT RECOGNIZED"};

// Convert the operator number into an index for MOBILE_NETWORK_STRINGS
int convertOperatorNumber(mobile_network_operator_t mno)
{
    switch (mno)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return ((int)mno);
    case 8:
        return 7;
    case 19:
        return 8;
    case 20:
        return 9;
    case 21:
        return 10;
    case 28:
        return 11;
    case 31:
        return 12;
    case 32:
        return 13;
    case 39:
        return 14;
    case 90:
        return 15;
    case 100:
        return 16;
    case 101:
        return 17;
    default: // NOT RECOGNIZED
        return 18;
    }
}

// This defines the size of the ops struct array. To narrow the operator
// list, set MOBILE_NETWORK_OPERATOR to AT&T, Verizon etc. instead
// of MNO_SW_DEFAULT.
#define MAX_OPERATORS 10

// Uncomment this line if you want to be able to communicate directly with the module in the main loop
// #define DEBUG_PASSTHROUGH_ENABLED

void setup()
{
    int opsAvailable;
    struct operator_stats ops[MAX_OPERATORS];
    String currentOperator = "";
    bool newConnection = true;

    Serial.begin(115200); // Start the serial console

    // Wait for user to press key to begin
    Serial.println(F("u-blox Cellular Example 3 - Register Operator"));
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
    if (myModule.begin(mySerial, UBX_CELL_DEFAULT_BAUD_RATE))
    {
        Serial.println(F("Module connected!"));
    }
    else
    {
        Serial.println(F("Unable to communicate with the module."));
        Serial.println(F("Manually power-on (hold the module's On button for 3 seconds) and try again."));
        while (1)
            ; // Loop forever on fail
    }
    Serial.println();

    // First check to see if we're already connected to an operator:
    if (myModule.getOperator(&currentOperator) == UBX_CELL_SUCCESS)
    {
        Serial.print(F("Already connected to: "));
        Serial.println(currentOperator);
        // If already connected provide the option to type y to connect to new operator
        Serial.println(F("Press y to connect to a new operator, or any other key to continue.\r\n"));
        while (!Serial.available())
            ;
        if (Serial.read() != 'y')
        {
            newConnection = false;
        }
        else
        {
            myModule.deregisterOperator(); // Deregister from the current operator so we can connect to a new one
        }
        while (Serial.available())
            Serial.read();
    }

    if (newConnection)
    {
        // Set MNO to either Verizon, T-Mobile, AT&T, Telstra, etc.
        // This will narrow the operator options during our scan later
        Serial.println(F("Setting mobile-network operator"));
        if (myModule.setNetworkProfile(MOBILE_NETWORK_OPERATOR))
        {
            Serial.print(F("Set mobile network operator to "));
            Serial.println(MOBILE_NETWORK_STRINGS[convertOperatorNumber(MOBILE_NETWORK_OPERATOR)] + "\r\n");
        }
        else
        {
            Serial.println(F("Error setting MNO. Try cycling the power. Freezing..."));
            while (1)
                ;
        }

        // Wait for user to press button before initiating network scan.
        Serial.println(F("Press any key scan for networks.."));
        serialWait();

        Serial.println(F("Scanning for networks...this may take up to 3 minutes\r\n"));
        // myModule.getOperators takes in a operator_stats struct pointer and max number of
        // structs to scan for, then fills up those objects with operator names and numbers
        opsAvailable = myModule.getOperators(ops, MAX_OPERATORS); // This will block for up to 3 minutes

        if (opsAvailable > 0)
        {
            // Pretty-print operators we found:
            Serial.println("Found " + String(opsAvailable) + " operators:");
            printOperators(ops, opsAvailable);
            Serial.println(String(opsAvailable + 1) + ": use automatic selection");
            Serial.println();

            // Wait until the user presses a key to initiate an operator connection
            Serial.println("Press 1-" + String(opsAvailable + 1) + " to select an operator.");
            char c = 0;
            bool selected = false;
            while (!selected)
            {
                while (!Serial.available())
                    ;
                c = Serial.read();
                int selection = c - '0';
                if ((selection >= 1) && (selection <= (opsAvailable + 1)))
                {
                    selected = true;
                    Serial.println("Connecting to option " + String(selection));
                    if (selection == (opsAvailable + 1))
                    {
                        if (myModule.automaticOperatorSelection() == UBX_CELL_SUCCESS)
                        {
                            Serial.println("Automatic operator selection: successful\r\n");
                        }
                        else
                        {
                            Serial.println(
                                F("Automatic operator selection: error. Reset and try again, or try another network."));
                        }
                    }
                    else
                    {
                        if (myModule.registerOperator(ops[selection - 1]) == UBX_CELL_SUCCESS)
                        {
                            Serial.println("Network " + ops[selection - 1].longOp + " registered\r\n");
                        }
                        else
                        {
                            Serial.println(
                                F("Error connecting to operator. Reset and try again, or try another network."));
                        }
                    }
                }
            }
        }
        else
        {
            Serial.println(F("Did not find an operator. Double-check SIM and antenna, reset and try again, or try "
                             "another network."));
            while (1)
                ;
        }
    }

    // At the very end print connection information
    printInfo();
}

void loop()
{
    // Loop provides a debugging interface.
    if (mySerial.available())
    {
        Serial.write((char)mySerial.read());
    }
#ifdef DEBUG_PASSTHROUGH_ENABLED
    if (Serial.available())
    {
        mySerial.write((char)Serial.read());
    }
#endif
}

void printInfo(void)
{
    String currentApn = "";
    IPAddress ip(0, 0, 0, 0);
    String currentOperator = "";

    Serial.println(F("Connection info:"));
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

    // Operator name or number
    if (myModule.getOperator(&currentOperator) == UBX_CELL_SUCCESS)
    {
        Serial.print(F("Operator: "));
        Serial.println(currentOperator);
    }

    // Received signal strength
    Serial.println("RSSI: " + String(myModule.rssi()));
    Serial.println();
}

void printOperators(struct operator_stats *ops, int operatorsAvailable)
{
    for (int i = 0; i < operatorsAvailable; i++)
    {
        Serial.print(String(i + 1) + ": ");
        Serial.print(ops[i].longOp + " (" + String(ops[i].numOp) + ") - ");
        switch (ops[i].stat)
        {
        case 0:
            Serial.print(F("UNKNOWN"));
            break;
        case 1:
            Serial.print(F("AVAILABLE"));
            break;
        case 2:
            Serial.print(F("CURRENT"));
            break;
        case 3:
            Serial.print(F("FORBIDDEN"));
            break;
        }
        switch (ops[i].act)
        {
        case 0:
            Serial.print(F(" - GSM"));
            break;
        case 2:
            Serial.print(F(" - UTRAN"));
            break;
        case 3:
            Serial.print(F(" - GSM/GPRS with EDGE"));
            break;
        case 7:
            Serial.print(F(" - LTE")); // SARA-R5 only supports LTE
            break;
        }
        Serial.println();
    }
    Serial.println();
}

void serialWait()
{
    while (Serial.available())
        Serial.read();
    while (!Serial.available())
        ;
    delay(100);
    while (Serial.available())
        Serial.read();
}
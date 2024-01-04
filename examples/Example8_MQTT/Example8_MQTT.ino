#include "SparkFun_u-blox_Cellular_Arduino_Library.h"

// Uncomment the line below that you need for Serial on your platform
#define mySerial Serial1
// SoftwareSerial mySerial(16, 17);

// Uncomment the module you're using. If your module is not listed below, then
// it's not supported for this example
SparkFun_ublox_Cellular myModule; // This example works with all modules, so the base class can be used
// SparkFun_ublox_SARA_R5 myModule; // Base SARA-R5 class
// SparkFun_ublox_SARA_R500S myModule;
// SparkFun_ublox_SARA_R500S_01B myModule;
// SparkFun_ublox_SARA_R500S_61B myModule;
// SparkFun_ublox_SARA_R510M8S_61B myModule;
// SparkFun_ublox_SARA_R510S myModule;
// SparkFun_ublox_LARA_R6 myModule; // Base LARA-R6 class
// SparkFun_ublox_LARA_R6001 myModule;
// SparkFun_ublox_LARA_R6001D myModule;
// SparkFun_ublox_LARA_R6401 myModule;
// SparkFun_ublox_LARA_R6401D myModule;
// SparkFun_ublox_LARA_R6801_00B myModule;
// SparkFun_ublox_LARA_R6801D myModule;

// You can change the Quality of Service (QoS) here if you want
int qos = 0;

// Topics that will be used for publishing and subscribing
String publishTopic;
String subscribeTopic;

// Whether we're connected to the MQTT broker
bool mqttConnected = false;

// Callback function for handling MQTT responses from the module
void mqttCallback(int command, int result)
{
    if (command == UBX_CELL_MQTT_COMMAND_LOGIN && result == 1)
    {
        // Connected to broker
        mqttConnected = true;
        Serial.println(F("Connected to broker!"));
    }
    else if (command == UBX_CELL_MQTT_COMMAND_LOGOUT && result == 1)
    {
        // Disconnected from broker
        mqttConnected = false;
        Serial.println(F("Disconnected from broker!"));
    }
    else if (command == UBX_CELL_MQTT_COMMAND_SUBSCRIBE && result == 1)
    {
        // Topic subscription successful
        Serial.println(F("Subscribed to topic!"));
        Serial.println(F("Enter any text to post to the topic"));
        Serial.println();
    }
    else if (command == UBX_CELL_MQTT_COMMAND_READ)
    {
        // New message available
        Serial.print(F("A new message is available! Total messages to read: "));
        Serial.println(result);
        Serial.println(F("Enter a blank line to read the oldest message"));
        Serial.println();
    }
    else
    {
        // Other response
        Serial.print(F("Unknown MQTT reponse! Command: "));
        Serial.print(command);
        Serial.print(F(" Result: "));
        Serial.println(result);
    }
}

void setup()
{
    String currentOperator = "";

    Serial.begin(115200); // Start the serial console

    // Wait for user to press key to begin
    Serial.println(F("u-blox Cellular Example 8 - MQTT"));
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

    // First check to see if we're connected to an operator:
    if (myModule.getOperator(&currentOperator) == UBX_CELL_SUCCESS)
    {
        Serial.print(F("Connected to: "));
        Serial.println(currentOperator);
    }
    else
    {
        Serial.print(F("The module is not yet connected to an operator. Please use the previous examples to connect. "
                       "Or wait and retry. Freezing..."));
        while (1)
            ; // Do nothing more
    }

    Serial.println();

    // Make sure any previous MQTT connection is closed
    myModule.disconnectMQTT();

    // Set callback for any MQTT responses from the module
    myModule.setMQTTCommandCallback(mqttCallback);

    Serial.println(F("Enter the MQTT broker server name"));

    // Clear any previous input and wait for new input
    while (Serial.available())
        Serial.read();
    while (!Serial.available())
        ;

    // Get the broker server name
    String serverName = Serial.readStringUntil('\n');
    Serial.print(F("Server name: "));
    Serial.println(serverName);
    Serial.println();

    Serial.println(F("Enter the MQTT broker server port number"));
    Serial.println(F("(or enter nothing for default port of 1883)"));

    // Clear any previous input and wait for new input
    while (Serial.available())
        Serial.read();
    while (!Serial.available())
        ;

    // Get the broker server port
    String serverPort = Serial.readStringUntil('\n');

    // Attempt to parse the port number. If it fails, just set 1883
    int port = serverPort.toInt();
    if (port == 0)
        port = 1883;

    Serial.print(F("Server port: "));
    Serial.println(port);
    Serial.println();

    // Now set the MQTT server
    myModule.setMQTTserver(serverName, port);

    Serial.println(F("Enter the client ID"));

    // Clear any previous input and wait for new input
    while (Serial.available())
        Serial.read();
    while (!Serial.available())
        ;

    // Get the client ID
    String clientID = Serial.readStringUntil('\n');
    Serial.print(F("Client ID: "));
    Serial.println(clientID);
    Serial.println();

    // Set the client ID
    myModule.setMQTTclientId(clientID);

    Serial.println(F("Connecting to MQTT broker..."));
    myModule.connectMQTT();

    // Wait for module to connect
    while (!mqttConnected)
        myModule.poll();

    Serial.println();
    Serial.println(F("Enter a topic to publish to"));

    // Clear any previous input and wait for new input
    while (Serial.available())
        Serial.read();
    while (!Serial.available())
        ;

    // Get the topic name
    publishTopic = Serial.readStringUntil('\n');
    Serial.print(F("Publish topic: "));
    Serial.println(publishTopic);
    Serial.println();

    Serial.println();
    Serial.println(F("Enter a topic to subscribe to"));

    // Clear any previous input and wait for new input
    while (Serial.available())
        Serial.read();
    while (!Serial.available())
        ;

    // Get the topic name
    subscribeTopic = Serial.readStringUntil('\n');
    Serial.print(F("Subscribe topic: "));
    Serial.println(subscribeTopic);
    Serial.println();

    // Subscribe to the topic
    myModule.subscribeMQTTtopic(qos, subscribeTopic);
}

void loop()
{
    // Need to call poll() frequently to receive updates from the module.
    myModule.poll();

    // Check for user input
    if (Serial.available())
    {
        // Get user's input
        String inputString = Serial.readStringUntil('\n');

        // Clear any remaining input
        while (Serial.available())
            Serial.read();

        // Check whether the user entered anything
        if (inputString.length() > 0)
        {
            // Publish the user's input to the topic
            Serial.println(F("Publishing message:"));
            Serial.println(inputString);
            Serial.println();
            myModule.mqttPublishTextMsg(publishTopic, inputString.c_str());
        }
        else
        {
            // Read next received message
            uint8_t buffer[MAX_MQTT_DIRECT_MSG_LEN];
            int bytesRead = 0;
            myModule.readMQTT(&qos, &subscribeTopic, buffer, MAX_MQTT_DIRECT_MSG_LEN, &bytesRead);

            // Print out message
            Serial.println(F("Received message:"));
            for (int i = 0; i < bytesRead; i++)
                Serial.print((char)buffer[i]);
            Serial.println();
            Serial.println();
        }
    }
}
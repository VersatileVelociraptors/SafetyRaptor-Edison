
/*
  Control servos off of a web server

  A lot of this code was taken from examples
*/

#include <Servo.h>
#include <SPI.h>
#include <WiFi.h>

#define LEFT_SERVO 9
#define RIGHT_SERVO 10
#define FLAME_SENSOR A0

char ssid[] = "UHWireless"; //  your network SSID (name)

int status = WL_IDLE_STATUS;

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(172, 25, 75, 108); // numeric IP (no DNS)
//char server[] = "www.google.com";    // name address (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

Servo left_drive;
Servo right_drive;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to open network
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10 * 1000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 2585)) {
    Serial.println("connected to server");
    // Make request:
    client.println("118");
  }

  left_drive.attach(LEFT_SERVO);// initialize servo on pin 9
  right_drive.attach(RIGHT_SERVO);// initialize second servo on pin 10
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  char input = client.read();
  Serial.println(input);
  if (input == 'L') {
    // left turn
    left_drive.write(-180);
    right_drive.write(-180);
  } else if (input == 'R') {
    // right turn
    left_drive.write(180);
    right_drive.write(180);
  } else if (input == 'F') {
    // forward
    left_drive.write(180);
    right_drive.write(-180);
  } else if (input == 'B') {
    // backward
    left_drive.write(-180);
    right_drive.write(180);
  }

  delay(500);

  if (analogRead(FLAME_SENSOR) > 700) {
    Serial.println("ROBOT ON FIRE! RUN AWAY!");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


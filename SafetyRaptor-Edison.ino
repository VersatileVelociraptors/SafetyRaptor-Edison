
/*
  Control servos off of a web server

  A lot of this code was taken from examples
*/

#include <Servo.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define LEFT_SERVO 6
#define RIGHT_SERVO 9
#define LEFT_TILT_SERVO 5
#define RIGHT_TILT_SERVO 3
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
WiFiUDP client;

Servo left_drive;
Servo right_drive;
Servo left_tilt;
Servo right_tilt;

/// run once before loop
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
  client.begin(2587);

  do {
    client.beginPacket(server, 2587);
    client.write(118);
    client.endPacket();
    client.flush();
    client.parsePacket();
  } while (!client.available());
  
  Serial.println("connected to server");

  left_drive.attach(LEFT_SERVO);// initialize servo
  right_drive.attach(RIGHT_SERVO);// initialize second servo
  left_tilt.attach(LEFT_TILT_SERVO);
  right_tilt.attach(RIGHT_TILT_SERVO);
}

/// run frequently as long as the board is on
void loop() {
  client.parsePacket();
  // if there are incoming bytes available
  // from the server, read them and print them:
  byte input = 0;
  if (client.available()){
    input = client.read();
  }
  Serial.println(input);
  if (input == 1) {
    // left turn
    if (!left_drive.attached()){
      left_drive.attach(LEFT_SERVO);
    }
    if (!right_drive.attached()){
      right_drive.attach(RIGHT_SERVO);
    }
    left_drive.write(-180);
    right_drive.write(-180);
    left_tilt.detach();
    right_tilt.detach();
  } else if (input == 2) {
    // right turn
    if (!left_drive.attached()){
      left_drive.attach(LEFT_SERVO);
    }
    if (!right_drive.attached()){
      right_drive.attach(RIGHT_SERVO);
    }
    left_drive.write(180);
    right_drive.write(180);
    left_tilt.detach();
    right_tilt.detach();
  } else if (input == 3) {
    // forward
    if (!left_drive.attached()){
      left_drive.attach(LEFT_SERVO);
    }
    if (!right_drive.attached()){
      right_drive.attach(RIGHT_SERVO);
    }
    left_drive.write(180);
    right_drive.write(-180);
    left_tilt.detach();
    right_tilt.detach();
  } else if (input == 4) {
    // backward
    if (!left_drive.attached()){
      left_drive.attach(LEFT_SERVO);
    }
    if (!right_drive.attached()){
      right_drive.attach(RIGHT_SERVO);
    }
    left_drive.write(-180);
    right_drive.write(180);
    left_tilt.detach();
    right_tilt.detach();
  } else if (input == 5) {
    if (!left_tilt.attached()){
      left_tilt.attach(LEFT_TILT_SERVO);
    }
    if (!right_tilt.attached()){
      right_tilt.attach(RIGHT_TILT_SERVO);
    }
    left_tilt.write(180);
    right_tilt.write(-180);
    left_drive.detach();
    right_drive.detach();
  } else if (input == 6) {
    if (!left_tilt.attached()){
      left_tilt.attach(LEFT_TILT_SERVO);
    }
    if (!right_tilt.attached()){
      right_tilt.attach(RIGHT_TILT_SERVO);
    }
    left_tilt.write(-180);
    right_tilt.write(180);
    left_drive.detach();
    right_drive.detach();
  } else if (input == 7 || input == 0) {
    // stop motors
    left_drive.detach();
    right_drive.detach();
    left_tilt.detach();
    right_tilt.detach();
  }

  delay(5);

  if (analogRead(FLAME_SENSOR) > 900) {
    Serial.println("ROBOT ON FIRE! RUN AWAY!");
  }
}

/// print the status of wireless connection to serial
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


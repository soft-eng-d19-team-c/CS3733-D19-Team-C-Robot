#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

//Settings
const int max_moves = 100;
const int dirMotorA = 13; //Motor pin for channel A, HIGH = forward
const int dirMotorB = 12; //Motor pin for channel B, HIGH = forward
const int velMotorA = 3; //Motor pin for channel A, 255 = full speed
const int velMotorB = 11; //Motor pin for channel B, 255 = full speed
const int brakeA = 9; //Brake pin for channel A, HIGH = engaged
const int brakeB = 8; //Brake pin for channel B, HIGH = engaged
const int turnTime = 2200;
IPAddress ip(192, 168, 43, 192); //Static IP of the arduino


char ssid[] = SECRET_SSID;        // Network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status
String thePOST;
int len;
int distances[max_moves];
char directions[max_moves];
int numMoves;

WiFiServer server(80);

void setup() {

  Serial.begin(9600);
  delay(1000); // wait for serial port to connect


  // ********* Network Setup ********* //

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  WiFi.config(ip); //Force WiFi.begin to use the static IP

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to open SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  server.begin();
  printCurrentNet();
  printWifiData();

  // ********* End Network Setup ********* //

  //String fakePOST = "S,120,L,50,S,15,L,150,S,2460,R,350,L,910,S,717,L,450";
  String fakerPOST = "S,120,L,0,L,120,R,0,R,0,R,0,L,0,L,0,L,0";
  thePOST = fakerPOST; //For now...

  //Motor setup
  //Setup Channel A
  pinMode(dirMotorA, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brakeA, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(dirMotorB, OUTPUT); //Initiates Motor Channel B pin
  pinMode(brakeB, OUTPUT);  //Initiates Brake Channel B pin

  Serial.println("Setup Complete");

}


void loop() {

  thePOST = ""; //Reset to empty string. Comment this out to use a fake test string

  getInfoFromApp();
  Serial.println("The instructions recieved are:");
  Serial.println(thePOST);

  len = thePOST.length();
  numMoves = count_moves(thePOST);
  populateMovementData(); //Parse data into arrays of directon/distance

  //Parse directions and distances
  for (int i = 0; i <= numMoves; i++) {
    switch (directions[i]) {
      case 'S':
        driveStraight(distances[i]);
        break;
      case 'L':
        turnLeft();
        driveStraight(distances[i]);
        break;
      case 'R':
        turnRight();
        driveStraight(distances[i]);
        break;
    }
  }

  if (numMoves != 0) { //If we actually did something and aren't just waiting for instructions
    Serial.println("Arrived at destination");
    delay(30000); //Wait 30 seconds
  }
}

//Counts the number of moves contained in the data
int count_moves(String theData) {

  int count = 0;
  int i = 0;
  int s = 0;
  int moves;

  if (len == 0) {
    return 0;
  }
  
  else {

    for (i = 0; i < len; i++)
    {
      if (theData.charAt(i) == ',')
      {
        count++;
      }
    }

    moves = (count - 1) / 2;
    return moves;
    
  }
}

void populateMovementData() {


  int distCount = 0;
  int dirCount = 1;
  char *str_array = thePOST.c_str();
  char* token = strtok(str_array, ",");
  directions[0] = token[0];
  char* token2;


  for (int j = 1; j <= ((numMoves * 2) + 1); j++) {

    token2 = strtok(NULL, ",");

    if (j % 2 != 0) { //If i is odd, it is a distance
      distances[distCount] = String(token2).toInt(); //Its as char, needs to be an int...
      distCount++;
    }
    else {
      directions[dirCount] = token2[0];
      dirCount++;
    }
  }
}

void driveStraight(int inches) {

  Serial.println("Going straight");
  double driveTime = (inches / 6) * 1000;
  digitalWrite(dirMotorA, HIGH); //Establishes forward direction of Channel A
  digitalWrite(dirMotorB, HIGH);  //Establishes forward direction of Channel B
  digitalWrite(brakeA, LOW);   //Disengage the Brake for Channel A
  digitalWrite(brakeB, LOW);   //Disengage the Brake for Channel B
  //This is where a while loop or some sort of control goes
  analogWrite(velMotorA, 255);   //Spins the motor on Channel A at full speed
  analogWrite(velMotorB, 255);   //Spins the motor on Channel B at full speed

  delay(driveTime); //In the absence of knowing how far

  analogWrite(velMotorA, 0);   //Stops powering the motor on Channel A
  analogWrite(velMotorB, 0);   //Stops powering the motor on Channel B
  digitalWrite(brakeA, HIGH);   //Engage the Brake for Channel A
  digitalWrite(brakeB, HIGH);   //Engage the Brake for Channel B

}

void turnLeft() {
  Serial.println("Turning Left");
  //Should always be 90 deg
  digitalWrite(dirMotorA, LOW); //Establishes backwards direction of Channel A
  digitalWrite(dirMotorB, HIGH);  //Establishes forwards direction of Channel B
  digitalWrite(brakeA, LOW);   //Disengage the Brake for Channel A
  digitalWrite(brakeB, LOW);   //Disengage the Brake for Channel B
  //This is where a while loop or some sort of control goes
  analogWrite(velMotorA, 255);   //Spins the motor on Channel A at full speed
  analogWrite(velMotorB, 255);   //Spins the motor on Channel B at full speed

  delay(turnTime); //About 90

  analogWrite(velMotorA, 0);   //Stops powering the motor on Channel A
  analogWrite(velMotorB, 0);   //Stops powering the motor on Channel B
  digitalWrite(brakeA, HIGH);   //Engage the Brake for Channel A
  digitalWrite(brakeB, HIGH);   //Engage the Brake for Channel B
}

void turnRight() {
  Serial.println("Turning Right");
  //Should always be 90 deg
  digitalWrite(dirMotorA, HIGH); //Establishes forwards direction of Channel A
  digitalWrite(dirMotorB, LOW);  //Establishes backwards direction of Channel B
  digitalWrite(brakeA, LOW);   //Disengage the Brake for Channel A
  digitalWrite(brakeB, LOW);   //Disengage the Brake for Channel B
  //This is where a while loop or some sort of control goes
  analogWrite(velMotorA, 255);   //Spins the motor on Channel A at full speed
  analogWrite(velMotorB, 255);   //Spins the motor on Channel B at full speed

  delay(turnTime); //About 90

  analogWrite(velMotorA, 0);   //Stops powering the motor on Channel A
  analogWrite(velMotorB, 0);   //Stops powering the motor on Channel B
  digitalWrite(brakeA, HIGH);   //Engage the Brake for Channel A
  digitalWrite(brakeB, HIGH);   //Engage the Brake for Channel B
}

void getInfoFromApp() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        thePOST += String(c); //Add the information to the list of instructions
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          Serial.println("Sending a response");
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("hello team C");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

//Network Info Stuff

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

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
const int turnTime = 2600;
/*IPAddress ip(192, 168, 43, 192); //Static IP of the arduino
  IPAddress gateway(192, 168, 50, 1);
  IPAddress dns(8, 8, 8, 8);
  IPAddress subnet(255, 255, 255, 0);*/



char ssid[] = SECRET_SSID;        // Network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status
String thePOST;
int len;
int distances[max_moves];
char directions[max_moves];
int numMoves;
String realRequest;
String data;


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

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    //WiFi.config(ip, dns, gateway, subnet); //Force WiFi.begin to use the static IP
    status = WiFi.begin(ssid, pass);
    //WiFi.config(ip, dns, gateway, subnet); //Force WiFi.begin to use the static IP
    // wait 10 seconds for connection:
    delay(10000);
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

  data = ""; //Reset to empty string. Comment this out to use a fake test string

  getInfoFromApp();

  if (data == "") {
    Serial.println("Awaiting instructions from application");
  }

  len = data.length();
  numMoves = count_moves(data);
  populateMovementData(); //Parse data into arrays of directon/distance

  //Parse directions and distances
  for (int i = 0; i <= numMoves; i++) {
    digitalWrite(brakeA, LOW);   //Disengage the Brake for Channel A
    digitalWrite(brakeB, LOW);   //Disengage the Brake for Channel B
    switch (directions[i]) {
      case 'S':
        //turnRight();
        //Serial.println((distances[i] * 2));
        Serial.println("Going for in:");
        Serial.println(distances[i]);
        driveStraight((distances[i] / 2));
        break;
      case 'L':
        turnLeft();
        Serial.println("Going for in:");
        Serial.println(distances[i]);
        driveStraight((distances[i] / 2));
        break;
      case 'R':
        //turnLeft();
        turnRight();
        Serial.println("Going for in:");
        Serial.println(distances[i]);
        driveStraight((distances[i] / 2));
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
  char *str_array = data.c_str();
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
  delay(1);
  Serial.println("Going straight");
  delay(1);
  int ips = inches / 11;
  Serial.println("ips:");
  Serial.println(ips);
  int driveTime = ips * 1000;
  Serial.println("Drive time:");
  Serial.println(/*(int)*/ driveTime);
  digitalWrite(dirMotorA, HIGH); //Establishes forward direction of Channel A
  Serial.println("Motor A HIGH");
  digitalWrite(dirMotorB, HIGH);  //Establishes forward direction of Channel B
  Serial.println("Motor B HIGH");
  //digitalWrite(brakeA, LOW);   //Disengage the Brake for Channel A
  //digitalWrite(brakeB, LOW);   //Disengage the Brake for Channel B
  //This is where a while loop or some sort of control goes
  analogWrite(velMotorA, 255);   //Spins the motor on Channel A at full speed
  analogWrite(velMotorB, 254);   //Spins the motor on Channel B at full speed
  Serial.println("Go motors");
  Serial.println("Delaying for:");
  Serial.println(driveTime);
  delay(driveTime); //In the absence of knowing how far
  Serial.println("Delay over");


  analogWrite(velMotorA, 0);   //Stops powering the motor on Channel A
  analogWrite(velMotorB, 0);   //Stops powering the motor on Channel B
  Serial.println("Stopped motors");
  //digitalWrite(brakeA, HIGH);   //Engage the Brake for Channel A
  //digitalWrite(brakeB, HIGH);   //Engage the Brake for Channel B
  delay(1);
  Serial.println("Done going straight");
}

void turnLeft() {
  delay(10);
  Serial.println("Turning Left");
  delay(10);
  //Should always be 90 deg
  digitalWrite(dirMotorA, LOW); //Establishes backwards direction of Channel A
  digitalWrite(dirMotorB, HIGH);  //Establishes forwards direction of Channel B
  //digitalWrite(brakeA, LOW);   //Disengage the Brake for Channel A
  //digitalWrite(brakeB, LOW);   //Disengage the Brake for Channel B
  //This is where a while loop or some sort of control goes
  analogWrite(velMotorA, 255);   //Spins the motor on Channel A at full speed
  analogWrite(velMotorB, 255);   //Spins the motor on Channel B at full speed

  delay(turnTime); //About 90

  analogWrite(velMotorA, 0);   //Stops powering the motor on Channel A
  analogWrite(velMotorB, 0);   //Stops powering the motor on Channel B
  //digitalWrite(brakeA, HIGH);   //Engage the Brake for Channel A
  //digitalWrite(brakeB, HIGH);   //Engage the Brake for Channel B
  Serial.println("Done turning Left");
  delay(500);
}

void turnRight() {
  delay(10);
  Serial.println("Turning Right");
  delay(10);
  //Should always be 90 deg
  digitalWrite(dirMotorA, HIGH); //Establishes forwards direction of Channel A
  digitalWrite(dirMotorB, LOW);  //Establishes backwards direction of Channel B
  //digitalWrite(brakeA, LOW);   //Disengage the Brake for Channel A
  //digitalWrite(brakeB, LOW);   //Disengage the Brake for Channel B
  //This is where a while loop or some sort of control goes
  analogWrite(velMotorA, 255);   //Spins the motor on Channel A at full speed
  analogWrite(velMotorB, 255);   //Spins the motor on Channel B at full speed

  delay(turnTime); //About 90

  analogWrite(velMotorA, 0);   //Stops powering the motor on Channel A
  analogWrite(velMotorB, 0);   //Stops powering the motor on Channel B
  //digitalWrite(brakeA, HIGH);   //Engage the Brake for Channel A
  //digitalWrite(brakeB, HIGH);   //Engage the Brake for Channel B
  Serial.println("Done turning Right");
  delay(1);
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
        thePOST += c;
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          ///////////////////////////////////////
          String requestBody;

          while (client.available()) {
            requestBody += (char)client.read();
          }

          if (requestBody.length()) {
            Serial.println("Request body = ");
            Serial.println(requestBody);
            realRequest = urldecode(requestBody);
            Serial.println(realRequest);
            Serial.println();
          }
          ///////////////////////////////////////

          // send a standard http response header
          Serial.println("Sending a response");
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<p>Hello team C. Thank you for the instructions.<P/><br/>");
          client.println("</html>");
          client.println("Connection: close");  // the connection will be closed after completion of the response
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
    //Get rid of key
    data = realRequest.substring(5);
    Serial.println("*********************DATA***********************************:");
    Serial.println(data);
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

String urldecode(String str)
{

  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == '+') {
      encodedString += ' ';
    } else if (c == '%') {
      i++;
      code0 = str.charAt(i);
      i++;
      code1 = str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString += c;
    } else {

      encodedString += c;
    }

    yield();
  }

  return encodedString;
}

unsigned char h2int(char c)
{
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

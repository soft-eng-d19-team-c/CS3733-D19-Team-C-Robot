#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;        // Network SSID (name)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

const int max_moves = 100;
String thePOST;
int len;
int distances[max_moves];
char directions[max_moves];
int numMoves;

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
    Serial.print("Attempting to connect to open SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

  // ********* End Network Setup ********* //

  String fakePOST = "S,2000,L,50,S,15,L,150,S,2460,R,350,L,910,S,717,L,450";
  thePOST = fakePOST; //For now...
  len = thePOST.length();
  numMoves = count_moves(thePOST);
  populateMovementData(); //Parse data into array of directon/distance


  Serial.println("Setup Complete");
}


void loop() {
  // put your main code here, to run repeatedly:

  //Iterate through array
  //Make each move
  //Wait until finished
  //Onto the next
}

//Counts the number of moves contained in the data
int count_moves(String theData) {

  int count = 0;
  int i = 0;
  int s = 0;
  int moves;

  for (i = 0; i < len; i++)
  {
    if (theData.charAt(i) == ',')
    {
      count++;
    }
  }

  moves = (count - 1) / 2;
  Serial.println("Moves:" + moves);

}

void populateMovementData() {


  int distCount = 0;
  int dirCount = 1;
  char str_array[len];
  thePOST.toCharArray(str_array, len);
  char* token = strtok(str_array, ",");
  directions[0] = token;
  char* token2;


  while (token != NULL) { //While there is still unparsed direction data in thePOST
    for (int i = 1; i <= numMoves; i++) {

      token2 = strtok(NULL, ",");
      Serial.println("token2:");
      Serial.println(token2);

      if (i % 2 != 0) { //If i is odd, it is a distance
        distances[distCount] = token2;
        distCount++;
      }
      else {
        directions[dirCount] = token2;
        dirCount++;
      }
    }
  }
}

void driveStraight(int inches){
  //servo.writes or whatever
  //for x time? For encoder distance?
}

void turnLeft(){
  //servo.writes or whatever
  //for x time? For encoder distance?
  //Should always be 90 deg
}

void turnRight(){
  //servo.writes or whatever
  //for x time? For encoder distance?
  //Should always be 90 deg
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


/*void populateDistances() {

  char str_array[len];
  thePOST.toCharArray(str_array, len);
  char* token = strtok(str_array, ",");
  char* token2;

  while (token != NULL) {
    for (int i = 0; i <= numMoves; i + 2) {
      token2 = strtok(NULL, ",");
      Serial.println("token2:");
      Serial.println(token2);
      //distances[i] = token2;
    }
  }
  }*/



//Helper functions for:
//driveStraight()
//turnLeft()
//turnRight()

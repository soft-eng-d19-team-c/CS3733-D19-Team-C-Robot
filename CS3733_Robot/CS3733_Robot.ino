const int max_moves = 100;
String thePOST;
int len;
int distances[max_moves];
char directions[max_moves];
int numMoves;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  //Get POST
  /*http magic*/
  String fakePOST = "S,2000,L,50,S,15,L,150,S,2460,R,350,L,910,S,717,L,450";
  thePOST = fakePOST; //For now...
  len = thePOST.length();
  numMoves = count_moves(thePOST);
  //Parse data into array of directon/distance
  populateMovementData();



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

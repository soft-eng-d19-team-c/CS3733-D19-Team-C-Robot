const int max_moves = 100;
String thePOST;
int len;
double distances[max_moves];
char directions[max_moves];
int numMoves;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  //Get POST
  /*http magic*/
  String fakePOST = "S,20.0,L,50.0,S,15.4,L,150.0,S,20.0,R,30.0,L,10.0,S,17.0,L,4.5";
  thePOST = fakePOST; //For now...
  len = thePOST.length();
  numMoves = count_moves(thePOST);
  //Parse data into array of directon/distance



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

void populateDistances(){

  for (i = 0; i <= numMoves; i + 2){

    token = strtok(thePOST,",");
    distances[i] = token;
    
  }
  
}



//Helper functions for:
//driveStraight()
//turnLeft()
//turnRight()

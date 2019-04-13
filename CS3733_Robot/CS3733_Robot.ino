void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  //Get POST
  /*http magic*/
  String fakePOST = "S,20.0,L,50.0,S,15.4,L,150.0,S,20.0,R,30.0,L,10.0,S,17.0,L,4.5"
  String thePOST = fakePOST //For now...
  
  //Parse data into array of directon/distance

  int count_chars(const char* thePOST, char ch){
    
    int count = 0;
    int i;
    int length = strlen(string);

    for (i = 0; i < length; i++)
    {
        if (string[i] == ch)
        {
            count++;
        }
    }

    return count;
  }
  
  Serial.println(Setup Complete)
}

void loop() {
  // put your main code here, to run repeatedly:
  //Iterate through array
  //Make each move
  //Wait until finished
  //Onto the next 

}

//Helper functions for:
//driveStraight()
//turnLeft()
//turnRight()

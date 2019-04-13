# CS3733-D19-Team-C-Robot
Code for the robot

## POST data format
Comma-seperated String: "turn, distance, turn, distance..."

turn: String containing either L, R, or S

distance: double containing distance to drive straight in feet

### Example

Starting at Hallway 7 Floor L1
Take a left at Hallway 8 Floor L1
Continue straight past Elevator K Floor L1
Take a left at Hallway 9 Floor L1
Continue straight past Vending Machine 1 L1
Take a right at Hallway 12 Floor L1
Take a left at Hallway 13 Floor L1
Continue straight past Day Surgery Family Waiting Floor L1
Take a left at Day Surgery Family Waiting Exit Floor L1
Finally, arrive at Hallway 14 Floor L1

With made up distance numbers should look like:

```
"S,20.0,L,50.0,S,15.4,L,150.0,S,20.0,R,30.0,L,10.0,S,17.0,L,4.5"
```

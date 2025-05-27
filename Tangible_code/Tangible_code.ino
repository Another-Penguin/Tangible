// include adafruit neopixel library for use with the neopixel rings
#include <Adafruit_NeoPixel.h>

//declare the pins used aas outputs to drive the neopixel rings
#define LED_PIN 2
#define MEDIUM_LED_PIN 3
#define LARGE_LED_PIN 4

//declaring the amount of LEDs on each ring
#define LED_COUNT 12
#define MEDIUM_LED_COUNT 16
#define LARGE_LED_COUNT 24

//initialising 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel mediumStrip = Adafruit_NeoPixel(MEDIUM_LED_COUNT, MEDIUM_LED_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel largeStrip = Adafruit_NeoPixel(LARGE_LED_COUNT, LARGE_LED_PIN, NEO_GRB + NEO_KHZ800);

#define northButtonPin 21
#define eastButtonPin 20
#define southButtonPin 19
#define westButtonPin 18
#define actionButtonPin 17
byte actionButtonState = LOW;
byte northButtonState = LOW;
byte eastButtonState = LOW;
byte southButtonState = LOW;
byte westButtonState = LOW;
//LEAVE PIN 26 FOR RANDOM

//this is the array that stores the map of the game, 1 means a corridor, 0 means a wall, use ctrl + f to make it easier to see and edit
//there are 141 traversable tiles
bool wallMap[25][25] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
//this array stores if the player has already been in a specific position, this is to stop a player from getting encounters on a tile multiple times
bool visitMap[25][25] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

//this is an array that stores specific coordinates that get randomly picked for the player's spawn at the start of the game
int spawnNodes[5][2] = {
  {17, 4},
  {5, 5},
  {4, 18},
  {14, 22},
  {15, 23}
};

// Active nodes for combat phase
int attackNode1, attackNode2;
int defendNode1, defendNode2;
int inventoryNode;
int mode = 0;
bool isFighting = false;
int isTreasure;
int usedNodes[4];
int treasureNode;
int playerSpin;
bool defend = false;

// stats for player and enemy. Order in arrays is health, damage, speed
int playerStats[3] = {12, 1, 1};
int enemyStats[3] = {16, 1, 1};

//tracks the battles won, gets incremented once after every fight
int battlesWon = 0;

// track distance travelled and which exits are currently active on wheel. Order for exits array is left, forward, right
int tilesPassed = 1;
bool exits[3] = {false, false, false};

// inventory. Order in array is fast potion, slow, health, damage
int potions[4] = {0, 0, 0, 0};

// map management. playerPos array: {y, x}. map array: {y, x, isWall, isVisited}
int playerPos[2] = {0, 0};
int winPos[2] = {0, 0};

bool hasPressed = false;
bool hasEnded = false;

// time handler for combat
int timer;
unsigned long targetMillis;
unsigned long currentMillis;
const unsigned long period = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(26));

  //clear neopixel rings ready to run again after a reset
  strip.begin();
  mediumStrip.begin();
  largeStrip.begin();

  strip.clear();
  mediumStrip.clear();
  largeStrip.clear();

  strip.begin();
  strip.clear();
  strip.show();

  mediumStrip.begin();
  mediumStrip.clear();
  mediumStrip.show();

  largeStrip.begin();
  largeStrip.clear();
  largeStrip.show();

  //Select starting node
  int start = random(5);
  playerPos[0] = spawnNodes[start][0];
  playerPos[1] = spawnNodes[start][1];

  pinMode(westButtonPin, INPUT_PULLUP);
  pinMode(southButtonPin, INPUT_PULLUP);
  pinMode(eastButtonPin, INPUT_PULLUP);
  pinMode(northButtonPin, INPUT_PULLUP);
  pinMode(actionButtonPin, INPUT_PULLUP);
  //let the games begin
  findEnd();
  updateHealth();
}

//function to find
void findEnd(){
  bool findIt = true;
  int temp, temp2;
  while(findIt){
    temp = random(25);
    temp2 = random(25);
    if(wallMap[temp][temp2]){
      findIt = false;
      winPos[0] = temp;
      winPos[1] = temp2;
    }
  }
}

void loop() {

  currentMillis = millis();

  actionButtonState = digitalRead(actionButtonPin);
  northButtonState = digitalRead(northButtonPin);
  eastButtonState = digitalRead(eastButtonPin);
  southButtonState = digitalRead(southButtonPin);
  westButtonState = digitalRead(westButtonPin);
  wheelSpin();
  //check if the player is in the win position to take the player to the endscreen function
  if(playerPos[0] == winPos[0] && playerPos[1] == winPos[1]){
    mode = 6;
  }
  //check if the player is on 0 health to take the player to the lose function
  if (playerStats[0] == 0){
    mode = 7;
  }
}

void wheelSpin(){
  largeStrip.clear();
  //Exploring dungeon
  if (mode == 0){
    navigation();
  }
  //Inventory wheel
  if (mode == 1){
    inventory();
  }
  //Combat wheel
  if (mode == 2){
    combat();
  }
  //Defend wheel
  if (mode == 3){
    combatPrep();
  }
  //Inventory wheel
  if (mode == 4){
    treasure();
  }
  //check room
  if (mode == 5){
    enterRoom();
  }
  //win screen
  if(mode == 6){
    endScreen();
  }
  //lose screen
  if(mode == 7){
    lose();
  }
}
//******************************************************************NAVIGATION**************************************************************************
void navigation() {
  //Serial.println(mode);
  //Serial.println("Navigating");
  bool canNorth = false;
  bool canEast = false;
  bool canSouth = false;
  bool canWest = false;
  //north red
  if(wallMap[playerPos[0]-1][playerPos[1]]){
    for(int i=0; i< 6; i++){
      largeStrip.setPixelColor(i, strip.Color(10, 0, 0));
      largeStrip.show();
    }
    canNorth = true;
  }
  //south green
  if(wallMap[playerPos[0]+1][playerPos[1]]){
    for(int i=12; i< 18; i++){
      largeStrip.setPixelColor(i, strip.Color(0, 10, 0));
      largeStrip.show();
    }
    canSouth = true;
  }
  //east blue
  if(wallMap[playerPos[0]][playerPos[1]+1]){
    for(int i=6; i< 12; i++){
      largeStrip.setPixelColor(i, strip.Color(0, 0, 10));
      largeStrip.show();
    }
    canEast = true;
  }
  //west White
  if(wallMap[playerPos[0]][playerPos[1]-1]){
    for(int i=18; i< 24; i++){
      largeStrip.setPixelColor(i, strip.Color(10, 10, 10));
      largeStrip.show();
    }
    canWest = true;
  }
  visitMap[playerPos[0]][playerPos[1]] = true;

  //Flash selected direction
  //North
  if(northButtonState == LOW && canNorth && !hasPressed){
    Serial.println("Start North");
    for(int i = 0; i < 4; i++){
      largeStrip.clear();
      for(int j=0; j< 6; j++){
        largeStrip.setPixelColor(j, strip.Color(10, 0, 0));
        largeStrip.show();
      }
    }
    hasPressed = true;
    Serial.println("Went North");
    playerPos[0] = playerPos[0]-1;
    largeStrip.clear();
    mode = 5;
  }
  //South
  if(southButtonState == LOW && canSouth && !hasPressed){
    Serial.println("Start South");
    for(int i = 0; i < 4; i++){
      largeStrip.clear();
      for(int j=12; j< 18; j++){
        largeStrip.setPixelColor(j, strip.Color(0, 10, 0));
        largeStrip.show();
      }
    }
    hasPressed = true;
    Serial.println("Went South");
    playerPos[0] = playerPos[0]+1;
    largeStrip.clear();
    mode = 5;
  }
  //East
  if(eastButtonState == LOW && canEast && !hasPressed){
    Serial.println("Start East");
    for(int i = 0; i < 4; i++){
      largeStrip.clear();

      for(int j=6; j< 12; j++){
        largeStrip.setPixelColor(j, strip.Color(0, 0, 10));
        largeStrip.show();
      }
    }
    hasPressed = true;
    Serial.println("Went East");
    playerPos[1] = playerPos[1]+1;
    largeStrip.clear();
    mode = 5;
  }
  //West
  if(westButtonState == LOW && canWest && !hasPressed){
    Serial.println("Start West");
    for(int i = 0; i < 4; i++){
      largeStrip.clear();

      for(int j=18; j< 24; j++){
        largeStrip.setPixelColor(j, strip.Color(10, 10, 10));
        largeStrip.show();
      }
    }
    hasPressed = true;
    Serial.println("Went West");
    playerPos[1] = playerPos[1]-1;
    largeStrip.clear();
    mode = 5;
  }
  if(northButtonState == 1 && eastButtonState == 1 && southButtonState == 1 && westButtonState == 1){
    hasPressed = false;
  }
  else{
    hasPressed = true;
  }
}
//******************************************************************Inventory**************************************************************************

void inventory() {
  Serial.println(mode);
  Serial.println("inventory");
}

void combat() {
  Serial.println("combat");

  if (targetMillis <= currentMillis){
    defend = !defend;
    mode = 3;
  }

  if (enemyStats[0] == 0){
      enemyStats[0] = 16;
      playerStats[0] ++;
      battlesWon++
      mode = 0;
    }

  updateHealth();
  if(!defend){
    for (int i = 0; i < 24; i++){
      largeStrip.setPixelColor(i, strip.Color(10, 0, 0));
    }
  }
  if(defend){
    for (int i = 0; i < 24; i++){
      largeStrip.setPixelColor(i, strip.Color(0, 10, 0));
    }
  }
  largeStrip.show();
  if((digitalRead(actionButtonPin) == LOW && !hasPressed)){
    largeStrip.clear();
        if(!defend){
          enemyStats[0] -= 1;
        }
      else{
          playerStats[0] -= 1;
        }

  }
  if(actionButtonState == 1){
    hasPressed = false;
  }
  else{
    hasPressed = true;
  }
}

//******************************************************************Pre Combat**************************************************************************

void combatPrep(){
  bool nodesFilled = false;
  //5% chance for a treasure node in combat
  isTreasure = random(1, 20);
  //possible treasure node
  treasureNode = random(0, 23);

  updateHealth();

  targetMillis = currentMillis + random(2000) + 250;

  //create combat nodes
  for (int i = 0; i < 3; i++){
    bool loop = true;
    int dupeCheck;
    //loop through a single element until it is unique
    while (loop){
      dupeCheck = 0;
      usedNodes[i] = random(0, 23);
      //check if node overlaps with active treasure node
      if(usedNodes[i] != treasureNode && isTreasure == 20){
        loop = false;
      }
      if (isTreasure != 20){
        loop = false;
      }
      //if node is not unique, reroll
      for (int j = 0; j < 3; j++){
        if (usedNodes[i] == usedNodes[j]){
          dupeCheck += 1;
        }
        if (dupeCheck > 1){
          loop = true;
        }
      }
    }
  }
  //display all nodes
  for (int i = 0; i < 3; i++){
    largeStrip.setPixelColor(usedNodes[i], (10, 0, 0));
    largeStrip.show();
  }
  if (isTreasure == 20){
    largeStrip.setPixelColor(treasureNode, (10, 10, 0));
    largeStrip.show();
  }
  mode = 2;
}

//*****************************************************************Treasure**************************************************************************

void treasure(){
  int temp = random(0, 3);
  potions[temp]+=1;
 if (potions[temp] > 3){
  potions[temp] = 3;
 }
 mode = 0;
}

//function that runs when entering a room for the first time
void enterRoom() {
  if (!visitMap[playerPos[0]][playerPos[1]]){
    tilesPassed++;
    int temp = random(1, 5);
    if (temp == 1){
      //combat
      Serial.println("Combat");
      isFighting = true;
      //PUT ONE TIME COMBAT STUFF HERE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      Serial.println("okey!");
      enemyStats[0] = 16;
      mode = 3;
    }
    if (temp == 2){
      //nothing
      Serial.println("nav");
      mode = 0;
    }
    if (temp == 3){
      //treasure
      Serial.println("loot");
      mode = 0;
    }
  }
  else{
    mode = 0;
  }
}

//*****************************************************************Health**************************************************************************

//function to update and display player and enemy health on the neopixel rings
void updateHealth() {
  strip.clear();
  mediumStrip.clear();
  //Set player health display
  for(int i=0; i < playerStats[0]; i++){
    strip.setPixelColor(i, strip.Color(0, 10, 0));
    strip.show();
  }
  
  //Set enemy Health display
  if(isFighting){
    for(int i=0; i < enemyStats[0]; i++){
     mediumStrip.setPixelColor(i, strip.Color(10, 0, 0, 0));
     mediumStrip.show();
   }
  }
}

//win function, happens when player steps onto the win tile
void endScreen(){
  if(!hasEnded){
    Serial.println("ended");
    Serial.println(tilesPassed);
    Serial.println(battlesWon);
    largeStrip.clear();
    mediumStrip.clear();
    strip.clear();
    hasEnded = true;
  }
    //sets all neopixel rings to white
    for (int i = 0; i < 24; i++){
      largeStrip.setPixelColor(i, largeStrip.Color(10, 10, 10));
      largeStrip.show();
    }

    for (int i = 0; i < 16; i++){
      mediumStrip.setPixelColor(i, mediumStrip.Color(10, 10, 10, 0));
      mediumStrip.show();
    }
    
    for (int i = 0; i < 12; i++){
      strip.setPixelColor(i, strip.Color(10, 10, 10));
      strip.show();
    }
    
}

//losing function, happens when the player health drops to 0
void lose(){
  if(!hasEnded){
    Serial.println("ended");
    Serial.println(tilesPassed);
    Serial.println(battlesWon;
    largeStrip.clear();
    mediumStrip.clear();
    strip.clear();
    hasEnded = true;
    //sets all neopixel rings to red
    for (int i = 0; i < 24; i++){
      largeStrip.setPixelColor(i, largeStrip.Color(10, 0, 0));
      largeStrip.show();
    }
    
    for (int i = 0; i < 16; i++){
      mediumStrip.setPixelColor(i, mediumStrip.Color(10, 0, 0, 0));
      mediumStrip.show();
    }

    for (int i = 0; i < 12; i++){
      strip.setPixelColor(i, strip.Color(10, 0, 0));
      strip.show();
    }
  }
}
#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define MEDIUM_LED_PIN 3
#define LARGE_LED_PIN 4

#define LED_COUNT 12
#define MEDIUM_LED_COUNT 16
#define LARGE_LED_COUNT 24
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel mediumStrip = Adafruit_NeoPixel(MEDIUM_LED_COUNT, MEDIUM_LED_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel largeStrip = Adafruit_NeoPixel(LARGE_LED_COUNT, LARGE_LED_PIN, NEO_GRB + NEO_KHZ800);

#define northButtonPin 21
#define eastButtonPin 20
#define southButtonPin 19
#define westButtonPin 18
#define actionButtonPin 17
int buttonState = 0;
//LEAVE PIN 26 FOR RANDOM

//this is the array that stores the map of the game, 1 means a corridor, 0 means a wall, use ctrl + f to make it easier to see and edit
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
int battlesWon = 0;

// track distance travelled and which exits are currently active on wheel. Order for exits array is left, forward, right
int tilesPassed = 1;
bool exits[3] = {false, false, false};

// inventory. Order in array is fast potion, slow, health, damage
int potions[4] = {0, 0, 0, 0};

// map management. playerPos array: {y, x}. map array: {y, x, isWall, isVisited}
int playerPos[2] = {0, 0};
int checkPos[2] = {0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(26));

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

  checkPos[0] = playerPos[0];
  checkPos[1] = playerPos[1];
  pinMode(westButtonPin, INPUT_PULLUP);
  pinMode(southButtonPin, INPUT_PULLUP);
  pinMode(eastButtonPin, INPUT_PULLUP);
  pinMode(northButtonPin, INPUT_PULLUP);
  pinMode(actionButtonPin, INPUT_PULLUP);
  //let the games begin
  updateHealth();
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (playerPos != checkPos){
    checkPos[0] = playerPos[0];
    checkPos[1] = playerPos[1];
  }*/
  
  //Serial.println(playerPos[0]);
  //Serial.println(playerPos[1]);

  wheelSpin();
}

void wheelSpin(){
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
  if (mode == 5){
    enterRoom();
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
  if((digitalRead(northButtonPin) == LOW) && canNorth){
    for(int i; i < 4; i++){
      largeStrip.clear();
      delay(100);
      for(int j=0; j< 6; j++){
        largeStrip.setPixelColor(j, strip.Color(10, 0, 0));
        largeStrip.show();
      }
      delay(100);
    }
    Serial.println("Went North");
    playerPos[0] = playerPos[0]-1;

    mode = 5;
  }
  //South
  if((digitalRead(southButtonPin) == LOW) && canSouth){
    for(int i; i < 4; i++){
      largeStrip.clear();
      delay(100);
      for(int j=12; j< 18; j++){
        largeStrip.setPixelColor(j, strip.Color(0, 10, 0));
        largeStrip.show();
      }
      delay(100);
    }
    Serial.println("Went South");
    playerPos[0] = playerPos[0]+1;
    
    mode = 5;
  }
  //East
  if((digitalRead(eastButtonPin) == LOW) && canEast){
    for(int i; i < 4; i++){
      largeStrip.clear();
      delay(100);
      for(int j=6; j< 12; j++){
        largeStrip.setPixelColor(j, strip.Color(0, 0, 10));
        largeStrip.show();
      }
      delay(100);
    }
    Serial.println("Went East");
    playerPos[1] = playerPos[1]+1;
    
    mode = 5;
  }
  //West
  if((digitalRead(westButtonPin) == LOW) && canWest){
    for(int i; i < 4; i++){
      largeStrip.clear();
      delay(100);
      for(int j=18; j< 24; j++){
        largeStrip.setPixelColor(j, strip.Color(10, 10, 10));
        largeStrip.show();
      }
      delay(100);
    }
    Serial.println("Went West");
    playerPos[1] = playerPos[1]-1;
    
    mode = 5;
  }
  
}
//******************************************************************Inventory**************************************************************************

void inventory() {
  Serial.println(mode);
  Serial.println("inventory");
}

void combat() {

    Serial.println(mode);
  Serial.println("combat");

  updateHealth();
  playerSpin = 0;
  int red, blue, green;
  if(!defend){
    red = 10;
    blue = 0;
    green = 0;
  }
  else{
    red = 0;
    blue = 10;
    green = 0;
  }
  for (int i = 0; i < 4; i++){
    largeStrip.setPixelColor(i, strip.Color(red, green, blue));
  }
  if (isTreasure == 20){
    largeStrip.setPixelColor(treasureNode, strip.Color(10, 10, 0));
  }
  largeStrip.setPixelColor(playerSpin, strip.Color(0, 10, 0));
  largeStrip.show();
  playerSpin++;
  if((digitalRead(actionButtonPin) == LOW)){
    for(int i = 0; i < 4; i++){
      if (playerSpin == usedNodes[i]){
        if(!defend){
          enemyStats[0] -= 1;
        }
      }
      else if (playerSpin == treasureNode && isTreasure == 20){
        mode = 1;
      }
      else{
        if(defend){
          playerStats[0] -= 1;
        }
      }
    }
    mode = 3;
    if (enemyStats[0] == 0){
      enemyStats[0] = 16;
      mode = 0;
    }
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

void enterRoom() {
  /*if (!visitMap[playerPos[0]][playerPos[1]]){
    Serial.println(visitMap[playerPos[0]][playerPos[1]]);
    int temp = random(1, 4);
    if (temp == 1){
      //combat
      Serial.println("Combat");
      isFighting = true;
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
      mode = 4;
    }
  }*/
  mode = 0;
}

//*****************************************************************Health**************************************************************************

void updateHealth() {
  //Set player health display
  for(int i=0; i < playerStats[0]; i++){
    strip.setPixelColor(i, strip.Color(0, 10, 0));
    strip.show();
  }
  
  //Set enemy Health display
  if(isFighting){
    for(int i=0; i < enemyStats[0]; i++){
     mediumStrip.setPixelColor(i, strip.Color(0, 10, 0, 0));
     mediumStrip.show();
   }
  }
}
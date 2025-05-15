#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 2
#define MEDIUM_LED_PIN 3
#define LARGE_LED_PIN 4
#define LED_COUNT 12
#define MEDIUM_LED_COUNT 16
#define LARGE_LED_COUNT 24
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel mediumStrip = Adafruit_NeoPixel(MEDIUM_LED_COUNT, MEDIUM_LED_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel largeStrip = Adafruit_NeoPixel(LARGE_LED_COUNT, LARGE_LED_PIN, NEO_GRB + NEO_KHZ800);

int actionButtonPin = 0;
int northButtonPin = 1;
int eastButtonPin = 2;
int southButtonPin = 3;
int westButtonPin = 4;
//LEAVE PIN 5 FOR RANDOM

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
  strip.show();
  mediumStrip.begin();
  mediumStrip.show();
  largeStrip.begin();
  largeStrip.show();

  //Select starting node
  int start = random(5);
  playerPos[0] = spawnNodes[start][0];
  playerPos[1] = spawnNodes[start][1];

  //let the games begin
  updateHealth();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (playerPos != checkPos){
    mode = 5
    checkPos = playerPos;
  }
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
    defend();
  }
  //Inventory wheel
  if (mode == 4){
    treasure();
  }
  if (mode == 5){
    enterRoom();
  }
}

void navigation() {
  bool canNorth = false, canEast = false, canSouth = false, canWest = false;
  //north
  if(wallMap[playerPos[0]-1][playerPos[1]]){
    for(int i=0; i< 6; i++){
      largeStrip.setPixelColor(i, strip.Color(10, 0, 0));
      largeStrip.show();
    }
    canNorth = true;
  }
  //south
  if(wallMap[playerPos[0]+1][playerPos[1]]){
    for(int i=12; i< 18; i++){
      largeStrip.setPixelColor(i, strip.Color(0, 10, 0));
      largeStrip.show();
    }
    canSouth = true;
  }
  //east
  if(wallMap[playerPos[0]][playerPos[1]+1]){
    for(int i=6; i< 12; i++){
      largeStrip.setPixelColor(i, strip.Color(0, 0, 10));
      largeStrip.show();
    }
    canEast = true;
  }
  //west
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
  if(pinMode(northButtonPin, INPUT_PULLUP) && canNorth){
    for(int i; i < 4; i++){
      largeStrip.clear();
      delay(100);
      for(int j=0; j< 6; j++){
        largeStrip.setPixelColor(j, strip.Color(10, 0, 0));
        largeStrip.show();
      }
      delay(100);
    }
    playerPos[0] = playerPos[0]-1;
  }
  //South
  if(pinMode(southButtonPin, INPUT_PULLUP) && canSouth){
    for(int i; i < 4; i++){
      largeStrip.clear();
      delay(100);
      for(int j=12; j< 18; j++){
        largeStrip.setPixelColor(j, strip.Color(0, 10, 0));
        largeStrip.show();
      }
      delay(100);
    }
    playerPos[0] = playerPos[0]+1;
  }
  //East
  if(pinMode(eastButtonPin, INPUT_PULLUP) && canEast){
    for(int i; i < 4; i++){
      largeStrip.clear();
      delay(100);
      for(int j=6; j< 12; j++){
        largeStrip.setPixelColor(j, strip.Color(0, 0, 10));
        largeStrip.show();
      }
      delay(100);
    }
    playerPos[1] = playerPos[1]+1;
  }
  //West
  if(pinMode(westButtonPin, INPUT_PULLUP) && canWest){
    for(int i; i < 4; i++){
      largeStrip.clear();
      delay(100);
      for(int j=18; j< 24; j++){
        largeStrip.setPixelColor(j, strip.Color(10, 10, 10));
        largeStrip.show();
      }
      delay(100);
    }
    playerPos[1] = playerPos[1]-1;
  }
}

void inventory() {

}

void combat() {
  int isTreasure = random(0, 20);
  int temp = random(0, 23);
  updateHealth();
}

void defend(){
  bool nodesFilled = false;
  //5% chance for a treasure node in combat
  int isTreasure = random(1, 20);
  //list of active nodes for round
  int usedNodes[4];
  //possible treasure node
  int treasureNode = random(0, 23);

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
}

void treasure(){
  int temp = random(0, 3);
  potions[temp]+=1;
 if (potions[temp] > 3){
  potions[temp] = 3;
 }
}

void enterRoom() {
  if (visitMap[playerPos[0]][playerPos[1]]){
    int temp = random(1, 3);
    if (temp == 1){
      //combat
      isFighting = true;
      enemyStats[0] = 16;
      mode == 2;
    }
    if (temp == 2){
      //nothing
      mode == 0;
    }
    if (temp == 3){
      //treasure
      mode == 4;
    }
  }
}

void updateHealth() {
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
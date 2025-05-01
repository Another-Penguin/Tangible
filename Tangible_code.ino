void setup() {
  // put your setup code here, to run once:

  // Active nodes for combat phase
  int attackNode1, attackNode2;
  int defendNode1, defendNode2;
  int inventoryNode;

  // stats for player and enemy. Order in arrays is health, damage, speed
  int playerStats[3] = {12, 1, 1};
  int enemyStats[3] = {16, 1, 1};
  int battlesWon = 0;

  // track distance travelled and which exits are currently active on wheel. Order for exits array is left, forward, right
  int tilesPassed = 1;
  bool exits[3] = {false, false, false};

  // inventory. Order in array is fast potion, slow, health, damage
  int potions[4] = {0, 0, 0, 0};

  // map management. playerPos array: {x, y}. map array: {x, y, isWall, isVisited}
  int playerPos[2] = {0, 0};
  bool map[24][24][2] = {};
}

void loop() {
  // put your main code here, to run repeatedly:

}

void inventory() {}

void combat() {}

void navigation() {} 

void enterRoom() {}
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BTN 2
#define GRAVITY 1
#define JUMP_FORCE -8  // jump nhỏ
#define GROUND_Y 48

int dinoY = GROUND_Y;
int dinoVY = 0;
bool jumping = false;
int runFrame = 0; // frame lăn tròn

int obstacleX = SCREEN_WIDTH;
int score = 0;
bool gameOver = false;

unsigned long lastFrameTime = 0;

void setup() {
  pinMode(BTN, INPUT_PULLUP);
  Serial.begin(9600);
  
  if(!display.begin(0x3C, true)){
    Serial.println(F("SH1106 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.display();
}

void loop() {
  if(gameOver){
    displayGameOver();
    if(digitalRead(BTN) == LOW){
      resetGame();
    }
    delay(100);
    return;
  }

  handleInput();
  updateDino();
  updateObstacle();
  checkCollision();
  drawGame();

  delay(50);
}

void handleInput(){
  if(digitalRead(BTN) == LOW && !jumping){
    dinoVY = JUMP_FORCE;
    jumping = true;
  }
}

void updateDino(){
  dinoY += dinoVY;
  dinoVY += GRAVITY;
  if(dinoY >= GROUND_Y){
    dinoY = GROUND_Y;
    dinoVY = 0;
    jumping = false;
  }

  // Animation lăn tròn mỗi 150ms
  if(millis() - lastFrameTime > 150 && !jumping){
    runFrame = (runFrame + 1) % 4;
    lastFrameTime = millis();
  }
}

void updateObstacle(){
  obstacleX -= 4;
  if(obstacleX < -16){
    obstacleX = SCREEN_WIDTH;
    score++;
  }
}

void checkCollision(){
  if(obstacleX < 26 && obstacleX + 16 > 10 && dinoY + 16 > GROUND_Y){
    gameOver = true;
  }
}

void drawGame(){
  display.clearDisplay();
  
  // Vẽ ground là dải sao 4x4 pixel
  for(int x=0; x<SCREEN_WIDTH; x+=6){
    display.fillRect(x, GROUND_Y+16, 4, 4, SH110X_WHITE);
  }

  // Vẽ Dino hình tròn 16x16, thêm vệt nhỏ để thấy lăn
  int dinoX = 10;
  display.fillCircle(dinoX + 8, dinoY + 8, 8, SH110X_WHITE);
  // vệt lăn (line) để thấy chuyển động
  int lineOffset = runFrame * 2;
  display.drawLine(dinoX + 8 + lineOffset%16, dinoY + 4, dinoX + 8 + lineOffset%16, dinoY + 12, SH110X_BLACK);

  // Vẽ cây sương rồng tam giác, chạm ground
  int triHeight = 16;
  int triBase = 16;
  int x0 = obstacleX;
  int y0 = GROUND_Y + 16;
  display.fillTriangle(x0, y0, x0 + triBase, y0, x0 + triBase/2, y0 - triHeight, SH110X_WHITE);

  // Vẽ điểm
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(100,0);
  display.print(score);
  
  display.display();
}

void displayGameOver(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10,20);
  display.println("GAME OVER");
  display.setTextSize(1);
  display.setCursor(10,50);
  display.println("Press BTN to restart");
  display.display();
}

void resetGame(){
  dinoY = GROUND_Y;
  dinoVY = 0;
  obstacleX = SCREEN_WIDTH;
  score = 0;
  gameOver = false;
  jumping = false;
  runFrame = 0;
}

#include <Adafruit_GFX.h>          // Thư viện vẽ đồ họa 2D cho OLED / Graphics library for OLED
#include <Adafruit_SH110X.h>       // Thư viện dành cho màn hình SH1106 / Library for SH1106 OLED
#include <Wire.h>                  // Thư viện giao tiếp I2C / I2C communication library

#define SCREEN_WIDTH 128           // Chiều rộng màn hình / Screen width
#define SCREEN_HEIGHT 64           // Chiều cao màn hình / Screen height
#define OLED_RESET -1              // Không dùng chân reset / No reset pin used

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  
// Tạo đối tượng màn hình SH1106 / Create SH1106 display object

#define BTN 2                      // Nút nhấn tại chân D2 / Button pin D2
#define GRAVITY 1                  // Trọng lực kéo dino xuống / Gravity force
#define JUMP_FORCE -8              // Lực nhảy / Jump force
#define GROUND_Y 48                // Vị trí mặt đất / Ground position Y

int dinoY = GROUND_Y;              // Vị trí Y của dino / Dino Y position
int dinoVY = 0;                    // Vận tốc theo Y / Vertical velocity
bool jumping = false;              // Trạng thái đang nhảy / Dino jumping flag
int runFrame = 0;                  // Frame animation khi lăn / Running animation frame

int obstacleX = SCREEN_WIDTH;      // Vị trí X của chướng ngại vật / Obstacle X position
int score = 0;                     // Điểm / Score
bool gameOver = false;             // Trạng thái Game Over / Game over flag

unsigned long lastFrameTime = 0;   // Thời gian frame cuối / Last frame timestamp

void setup() {
  pinMode(BTN, INPUT_PULLUP);      // Nút nhấn dùng điện trở kéo lên / Button input pull-up
  Serial.begin(9600);              // Bật Serial debug / Start serial monitor
  
  if(!display.begin(0x3C, true)){  // Khởi động OLED tại địa chỉ I2C 0x3C / Init display
    Serial.println(F("SH1106 allocation failed")); // Lỗi màn hình / Display error
    for(;;);                       // Dừng chương trình / Stop
  }
  
  display.clearDisplay();          // Xóa màn hình / Clear display
  display.display();               // Cập nhật / Update
}

void loop() {
  if(gameOver){                    // Nếu thua / If game over
    displayGameOver();             // Hiển thị Game Over / Show screen
    if(digitalRead(BTN) == LOW){   // Nhấn nút để chơi lại / Press to restart
      resetGame();
    }
    delay(100);
    return;
  }

  handleInput();                   // Xử lý nút nhấn / Handle button
  updateDino();                    // Cập nhật chuyển động dino / Update dino physics
  updateObstacle();                // Cập nhật obstacle / Move obstacle
  checkCollision();                // Kiểm tra va chạm / Collision check
  drawGame();                      // Vẽ game / Draw frame

  delay(50);                       // Tốc độ khung hình / Frame delay
}

void handleInput(){
  if(digitalRead(BTN) == LOW && !jumping){  
    // Nếu nhấn nút và chưa nhảy / Button pressed and not jumping
    dinoVY = JUMP_FORCE;           // Bật lên / Jump up
    jumping = true;                // Đánh dấu đang nhảy / Set jumping flag
  }
}

void updateDino(){
  dinoY += dinoVY;                 // Cập nhật vị trí theo vận tốc / Update Y by velocity
  dinoVY += GRAVITY;               // Gravity / Apply gravity
  
  if(dinoY >= GROUND_Y){           // Chạm đất / Hit ground
    dinoY = GROUND_Y;
    dinoVY = 0;
    jumping = false;               // Không còn nhảy / No longer jumping
  }

  // Animation lăn tròn mỗi 150ms / Rolling animation
  if(millis() - lastFrameTime > 150 && !jumping){
    runFrame = (runFrame + 1) % 4; // 4 frame animation
    lastFrameTime = millis();
  }
}

void updateObstacle(){
  obstacleX -= 4;                  // Di chuyển từ phải qua trái / Move left
  if(obstacleX < -16){             // Qua khỏi màn hình / Off screen
    obstacleX = SCREEN_WIDTH;      // Reset về phải / Reset to right
    score++;                       // Tăng điểm / Add score
  }
}

void checkCollision(){
  // Kiểm tra va chạm đơn giản / Simple collision check
  if(obstacleX < 26 && obstacleX + 16 > 10 && dinoY + 16 > GROUND_Y){
    gameOver = true;
  }
}

void drawGame(){
  display.clearDisplay();          // Xóa màn hình / Clear

  // Vẽ mặt đất bằng ô vuông / Ground dots
  for(int x=0; x<SCREEN_WIDTH; x+=6){
    display.fillRect(x, GROUND_Y+16, 4, 4, SH110X_WHITE);
  }

  // Vẽ dino hình tròn 16x16 / Draw dino as circle
  int dinoX = 10;
  display.fillCircle(dinoX + 8, dinoY + 8, 8, SH110X_WHITE);

  // Vệt lăn để thấy chuyển động / Rolling indicator line
  int lineOffset = runFrame * 2;
  display.drawLine(dinoX + 8 + lineOffset%16, dinoY + 4,
                   dinoX + 8 + lineOffset%16, dinoY + 12, SH110X_BLACK);

  // Vẽ cây sương rồng tam giác / Draw cactus triangle
  int triHeight = 16;
  int triBase = 16;
  int x0 = obstacleX;
  int y0 = GROUND_Y + 16;
  display.fillTriangle(x0, y0, x0 + triBase, y0,
                       x0 + triBase/2, y0 - triHeight, SH110X_WHITE);

  // Hiển thị điểm / Show score
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(100,0);
  display.print(score);
  
  display.display();               // Cập nhật màn hình / Update display
}

void displayGameOver(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10,20);
  display.println("GAME OVER");    // Thông báo thua / Game over
  display.setTextSize(1);
  display.setCursor(10,50);
  display.println("Press BTN to restart"); // Nhấn nút để chơi lại
  display.display();
}

void resetGame(){
  dinoY = GROUND_Y;                // Reset vị trí dino / Reset dino
  dinoVY = 0;
  obstacleX = SCREEN_WIDTH;        // Reset chướng ngại vật / Reset obstacle
  score = 0;
  gameOver = false;
  jumping = false;
  runFrame = 0;
}

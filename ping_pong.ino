#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define VRX_PIN A0
#define VRY_PIN A1
#define SW_PIN 2

int paddleWidth = 20;
int paddleHeight = 5;
int paddleX = (SCREEN_WIDTH - paddleWidth) / 2;
int paddleY = SCREEN_HEIGHT - paddleHeight - 1;
int ballX = SCREEN_WIDTH / 2;
int ballY = SCREEN_HEIGHT / 2;
int ballSize = 3;
int ballSpeedX = 2;
int ballSpeedY = 2;

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); 
  }

  pinMode(SW_PIN, INPUT_PULLUP);
  display.clearDisplay();
}

void loop() {
  int xValue = analogRead(VRX_PIN);
  paddleX = map(xValue, 0, 1023, 0, SCREEN_WIDTH - paddleWidth);

  ballX += ballSpeedX;
  ballY += ballSpeedY;

  if (ballX <= 0 || ballX >= SCREEN_WIDTH - ballSize) {
    ballSpeedX = -ballSpeedX;
  }
  if (ballY <= 0) {
    ballSpeedY = -ballSpeedY;
  }

  if (ballY >= paddleY - ballSize && ballX >= paddleX && ballX <= paddleX + paddleWidth) {
    ballSpeedY = -ballSpeedY;
  }

  if (ballY > SCREEN_HEIGHT) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 20);
    display.println("Game Over");
    display.display();
    delay(2000);
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballSpeedX = 2;
    ballSpeedY = 2;
  }

  display.clearDisplay();
  display.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, SSD1306_WHITE);
  display.fillRect(ballX, ballY, ballSize, ballSize, SSD1306_WHITE);
  display.display();
  delay(30);
}



```C
// Photogrammetry

// +++++++++++ 設定藍牙
#include <BluetoothSerial.h>  // 載入藍牙程式庫
BluetoothSerial BT;
char val;  // 設定藍牙接收數據變數

#include <AccelStepper.h>  // 載入步進馬達程式庫

// +++++++++++++
int maxspeed = 500;  // 電機最大速度 1000（正數為順時針）
int maxaccel = 50;   // 電機最大加速度
int xstops = 16;     // 預設一圈停16次
int xsteps = 20;     // 預設Y軸增減幅度
int ysteps = 20;     // 預設Y軸增減幅度
int temp = 0;
const int ypos[] = { 980, 1010, 1038, 1068 };  // Y座標 x4
const int ypos_max = 4;                       // 最大Y座標數目

// +++++++++++ 定義步進電機引腳 ESP32 Lolin Lite
#define LEDPin 2     // GPIO22 - 開發板上的LED
#define PhotoPin 19  // 拍照

// +++++++++++ 步進電機：Y 軸旋轉（前後）
#define motor1Pin1 32  // 28BYJ48連接的ULN2003電機驅動板引腳 in1
#define motor1Pin2 33  // 28BYJ48連接的ULN2003電機驅動板引腳 in2
#define motor1Pin3 25  // 28BYJ48連接的ULN2003電機驅動板引腳 in3
#define motor1Pin4 26  // 28BYJ48連接的ULN2003電機驅動板引腳 in4
// +++++++++++ 步進電機：X 軸旋轉（左右）
#define motor2Pin1 27  // 28BYJ48連接的ULN2003電機驅動板引腳 in1
#define motor2Pin2 14  // 28BYJ48連接的ULN2003電機驅動板引腳 in2
#define motor2Pin3 12  // 28BYJ48連接的ULN2003電機驅動板引腳 in3
#define motor2Pin4 13  // 28BYJ48連接的ULN2003電機驅動板引腳 in4

// +++++++++++ 電機步進方式定義：全步進參數（FULLSTEP）
#define FULLSTEP 4  // 全步進參數（預設）：2048為一圈
#define HALFSTEP 8  // 半步進參數：2048為半圈；4096為一圈

// +++++++++++ 建立 stepp0erX 及 stepperX 物件
AccelStepper stepperY(FULLSTEP, motor1Pin1, motor1Pin3, motor1Pin2, motor1Pin4);  // Y 軸旋轉（前後）
AccelStepper stepperX(FULLSTEP, motor2Pin1, motor2Pin3, motor2Pin2, motor2Pin4);  // X 軸旋轉（左右）

// +++++++++++ 程式設定
void setup() {

  Serial.begin(115200);  // 設定藍牙Baud Rate

  BT.begin("ESP32 Photogrammetry");  // 設定藍牙名稱，可以是中文字符
  byte macBT[6];
  esp_read_mac(macBT, ESP_MAC_BT);
  Serial.printf("藍牙MAC位址：%02X:%02X:%02X:%02X:%02X:%02X\n",
                macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);

  // +++++++++++ 宣告 ESP32 Lolin Lite 的針腳作用
  pinMode(LEDPin, OUTPUT);    // 開發板上的LED
  pinMode(PhotoPin, OUTPUT);  // 設定拍照為輸出

  pinMode(motor1Pin1, OUTPUT);  // 28BYJ48連接的ULN2003電機驅動板引腳 in1
  pinMode(motor1Pin2, OUTPUT);  // 28BYJ48連接的ULN2003電機驅動板引腳 in2
  pinMode(motor1Pin3, OUTPUT);  // 28BYJ48連接的ULN2003電機驅動板引腳 in3
  pinMode(motor1Pin4, OUTPUT);  // 28BYJ48連接的ULN2003電機驅動板引腳 in4

  pinMode(motor2Pin1, OUTPUT);  // 28BYJ48連接的ULN2003電機驅動板引腳 in1
  pinMode(motor2Pin2, OUTPUT);  // 28BYJ48連接的ULN2003電機驅動板引腳 in2
  pinMode(motor2Pin3, OUTPUT);  // 28BYJ48連接的ULN2003電機驅動板引腳 in3
  pinMode(motor2Pin4, OUTPUT);  // 28BYJ48連接的ULN2003電機驅動板引腳 in4

  stepperX.setMaxSpeed(maxspeed);      // 電機最大速度
  stepperY.setMaxSpeed(maxspeed);      // 電機最大速度
  stepperX.setAcceleration(maxaccel);  // 電機最大加速度
  stepperY.setAcceleration(maxaccel);  // 電機最大加速度
  stepperX.setCurrentPosition(0);      // 複位步進電機初始位置
  stepperY.setCurrentPosition(1024);   // 複位步進電機初始位置

  digitalWrite(LEDPin, LOW);  // LED ON：程式啟動
  digitalWrite(PhotoPin, LOW);
}
// +++++++++++ 主程式
void loop() {
  // +++++++++++ 讀取藍牙字元輸入
  if (BT.available()) {  // 若接收到藍牙序列資料
    val = BT.read();
    switch (val) {
      // ++++++++++ test max position X
      case 'a':
        testx();
        break;
      // ++++++++++ test max position Y
      case 'b':
        testy();
        break;
      // ++++++++++ xstops=8
      case 'c':
        xstops = 8;
        break;
      // ++++++++++ xstops=16
      case 'd':
        xstops = 16;
        break;
      // ++++++++++ xstops=32
      case 'e':
        xstops = 32;
        break;
      // ++++++++++ X rotate a circle
      case 'f':
        Anglestop_Clk_X();
        break;
      // ++++++++++ Preset procedures
      case 'g':
        Allprocedure();
        break;
      // ++++++++++ Y rotate increase
      case 'h':
        temp = stepperY.currentPosition();  // 讀取步進電機位置
        stepperY.moveTo(temp + ysteps);
        stepperY.runToPosition();
        break;
      // ++++++++++ Y rotate decrease
      case 'i':
        temp = stepperY.currentPosition();  // 讀取步進電機位置
        stepperY.moveTo(temp - ysteps);
        stepperY.runToPosition();
        break;
      // ++++++++++ take a photo
      case 'j':
        digitalWrite(LEDPin, HIGH);    // 拍照
        digitalWrite(PhotoPin, HIGH);  // 拍照
        delay(100);                    // 稍停 0.1秒
        digitalWrite(LEDPin, LOW);     // 停止拍照
        digitalWrite(PhotoPin, LOW);   // 停止拍照
        delay(100);                    // 稍停 0.1秒
        break;
      // ++++++++++ restore Y
      case 'k':
        restoreYpos();
        break;
      // ++++++++++ set Y
      case 'l':
        stepperY.setCurrentPosition(1024);  // 複位步進電機初始位置
        break;
      // ++++++++++ X rotate increase
      case 'm':
        stepperX.moveTo(xsteps);
        stepperX.runToPosition();
        stepperX.setCurrentPosition(0);
        break;
      // ++++++++++ X rotate dencrease
      case 'n':
        stepperX.moveTo(-xsteps);
        stepperX.runToPosition();
        stepperX.setCurrentPosition(0);
        break;
      default:
        //
        break;
    }
  }
}
void testx() {
  // +++++++++++ 啟動測試 X
  stepperX.moveTo(2048);  // 順時針轉一圈
  stepperX.runToPosition();
  stepperX.setCurrentPosition(0);  // 複位步進電機初始位置
}

void testy() {
  // +++++++++++ 啟動測試 Y
  for (int y = 0; y <= ypos_max - 1; y++) {
    stepperY.moveTo(ypos[y]);  // 順時針轉
    stepperY.runToPosition();
  }
  stepperY.moveTo(1024);  // 複位步進電機初始位置
  stepperY.runToPosition();
}
void Allprocedure() {
  // Y 軸共停4次：位置ypos[array]
  for (int y = 0; y <= ypos_max - 1; y++) {
    stepperY.moveTo(ypos[y]);
    stepperY.runToPosition();
    Anglestop_Clk_X();
  }
  restoreYpos();  // 復位
  // ++++++++++++++++++ 復位
  stepperY.moveTo(1024);
  stepperY.runToPosition();
  delay(500);
}
void restoreXpos() {
  // ++++++++++++++++++ X 復位
  stepperX.moveTo(0);
  stepperX.runToPosition();
}
void restoreYpos() {
  // ++++++++++++++++++ Y 復位
  stepperY.moveTo(1024);
  stepperY.runToPosition();
}
void Anglestop_Clk_X() {
  // 轉動一圈停頓次數換算步數
  for (int x = 1; x <= xstops; x++) {
    digitalWrite(LEDPin, HIGH);    // 拍照
    digitalWrite(PhotoPin, HIGH);  // 拍照
    delay(100);                    // 稍停 0.1秒
    digitalWrite(LEDPin, LOW);     // 停止拍照
    digitalWrite(PhotoPin, LOW);   // 停止拍照
    stepperX.moveTo(2048 / xstops * x);
    stepperX.runToPosition();
    delay(100);
  }
  stepperX.setCurrentPosition(0);  // 複位步進電機初始位置
}
```

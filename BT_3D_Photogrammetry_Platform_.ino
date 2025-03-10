// 小型物件拍攝台 + 應用方案
// 開發板：WEMOS LOLIN32（ESP32）
// Kok Alexander
//
// email：kokalexb@gmail.com

// +++++++++++ 設定藍牙
#include <BluetoothSerial.h>  // 載入藍牙程式庫
BluetoothSerial BT;           // 建立藍牙物件
char val;                     // 設定藍牙接收數據變數
#include <AccelStepper.h>     // 載入步進馬達程式庫

// +++++++++++++
int maxspeed = 500;	// 電機最大速度 1000（正數為順時針）
int maxaccel = 50;		// 電機最大加速度
int xstops = 16;			// 預設一圈停16次
int xsteps = 20;			// 預設X軸增減幅度
int ysteps = 20;			// 預設Y軸增減幅度
int temp = 0;				// 步進電機位置變數
const int ypos_max = 4;                        // 預設Y軸停頓4次
const int ypos[] = { 970, 1000, 1048, 1078 };  // 預設的Y軸4次停頓位置
// +++++++++++ 定義步進電機引腳 ESP32 Lolin Lite
#define LEDPin 2     // GPIO22 - 開發板上的LED
#define PhotoPin 19  // 連接Android藍牙拍攝器
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
// +++++++++++ 建立 stepperX 及 stepperY 物件
AccelStepper stepperY(FULLSTEP, motor1Pin1, motor1Pin3, motor1Pin2, motor1Pin4);  // Y 軸旋轉（前後）
AccelStepper stepperX(FULLSTEP, motor2Pin1, motor2Pin3, motor2Pin2, motor2Pin4);  // X 軸旋轉（左右）

// +++++++++++ 程式設定
void setup() {
  Serial.begin(115200);                // 設定藍牙Baud Rate
  BT.begin("ESP32 - Photogrammetry");  // 設定藍牙Device時的名稱，可以是中文字符
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

  stepperX.setMaxSpeed(maxspeed);      // AccelStepper的參數：電機最大速度
  stepperY.setMaxSpeed(maxspeed);      // AccelStepper的參數：電機最大速度
  stepperX.setAcceleration(maxaccel);  // AccelStepper的參數：電機最大加速度
  stepperY.setAcceleration(maxaccel);  // AccelStepper的參數：電機最大加速度
  stepperX.setCurrentPosition(0);      // AccelStepper的參數：X軸電機的初始位置
  stepperY.setCurrentPosition(1024);   // AccelStepper的參數：Y軸電機的初始位置

  digitalWrite(LEDPin, LOW);  // LED ON：程式啟動
  digitalWrite(PhotoPin, LOW);
}

// +++++++++++ 主程式
void loop() {
  // +++++++++++ 讀取藍牙字元輸入
  if (BT.available()) {  // 若接收到藍牙序列資料
    val = BT.read();     // 將接收到的字元放入val變數中
    switch (val) {

      case 'a':
        // +++++++++++ 測試 X 軸活動範圍
        stepperX.moveTo(2048);     // 順時針轉一圈
        stepperX.runToPosition();  // 執行電機轉動指令
        delay(1000);               // 給電機的工作時間：1秒停頓
        break;

      case 'b':
        // +++++++++++ 測試 Y 軸活動範圍
        for (int y = 0; y <= ypos_max - 1; y++) {
          stepperY.moveTo(ypos[y]);  // 順時針轉
          stepperY.runToPosition();  // 執行電機轉動指令
          delay(1000);               // 給電機的工作時間：1秒停頓
        }
        // 測試活動範圍後， Y 軸回到「中間」位置：1024
        stepperY.moveTo(1024);     // Y 軸回到「中間」位置
        stepperY.runToPosition();  // 執行電機轉動指令
        delay(1000);               // 給電機的工作時間：1秒停頓
        break;

      case 'c':
        // ++++++++++ 設定X軸轉動一圈停頓：8 次
        xstops = 8;
        break;

      case 'd':
        // ++++++++++ 設定X軸轉動一圈停頓：16 次
        xstops = 16;
        break;

      case 'e':
        // ++++++++++ 設定X軸轉動一圈停頓：32 次
        xstops = 32;
        break;

      case 'f':
        // ++++++++++ X軸轉動一圈並拍照
        // X軸一圈需要停頓的次數為xstops
        stepperX.setCurrentPosition(0);  // 重設現在位置為0
        for (int x = 1; x <= xstops; x++) {
          // 先拍一張照
          digitalWrite(LEDPin, HIGH);    // 拍照
          digitalWrite(PhotoPin, HIGH);  // 拍照
          delay(100);                    // 稍停 0.1秒
          digitalWrite(LEDPin, LOW);     // 停止拍照
          digitalWrite(PhotoPin, LOW);   // 停止拍照
                                         // 計算下一個X軸停頓位置
          stepperX.moveTo(2048 / xstops * x);
          stepperX.runToPosition();  // 執行電機轉動指令
          delay(1000);               // 給電機的工作時間：1秒停頓
        }
        break;

      case 'g':
        // ++++++++++ 自動操作
        stepperX.setCurrentPosition(0);  // 重設現在位置為0
        // ++++++++++ 自動拍攝程序：Y軸移至停頓位置
        for (int y = 0; y <= ypos_max - 1; y++) {          
          // Y軸電機的4次停頓位置（ ypos[0]至 ypos[3]）
          stepperY.moveTo(ypos[y]);
          stepperY.runToPosition();  // 執行電機轉動指令
          delay(1000);               // 給電機的工作時間：1秒停頓
                                     // ++++++++++ X軸開始轉動一圈並拍照
          // X軸一圈需要停頓的次數為xstops
          for (int x = 1; x <= xstops; x++) {
            // 先拍一張照
            digitalWrite(LEDPin, HIGH);    // 拍照
            digitalWrite(PhotoPin, HIGH);  // 拍照
            delay(100);                    // 稍停 0.1秒
            digitalWrite(LEDPin, LOW);     // 停止拍照
            digitalWrite(PhotoPin, LOW);   // 停止拍照
                                           // 計算下一個X軸停頓位置
            stepperX.moveTo(2048 / xstops * x);
            stepperX.runToPosition();  // 執行電機轉動指令
            delay(1000);               // 給電機的工作時間：1秒停頓
          }
          stepperX.setCurrentPosition(0);  // 重設現在位置為0
        }
        // ++++++++++ 拍照完成：Y軸步進電機回到「中間」位置：1024
        stepperY.moveTo(1024);
        stepperY.runToPosition();  // 執行電機轉動指令
        delay(1000);               // 給電機的工作時間：1秒停頓
        break;

      case 'h':
        // ++++++++++ Y軸微調（增加）
        temp = stepperY.currentPosition();  // 讀取步進電機位置
        stepperY.moveTo(temp + ysteps);     // 增加Y軸電機幅度
        stepperY.runToPosition();           // 執行電機轉動指令
        break;

      case 'i':
        // ++++++++++ Y軸微調（減少）
        temp = stepperY.currentPosition();  // 讀取步進電機位置
        stepperY.moveTo(temp - ysteps);     // 減少Y軸電機幅度
        stepperY.runToPosition();           // 執行電機轉動指令
        break;

      case 'j':
        // ++++++++++ 即時拍攝一張照片
        digitalWrite(LEDPin, HIGH);    // 拍照
        digitalWrite(PhotoPin, HIGH);  // 拍照
        delay(100);                    // 稍停 0.1秒
        digitalWrite(LEDPin, LOW);     // 停止拍照
        digitalWrite(PhotoPin, LOW);   // 停止拍照
        delay(100);                    // 稍停 0.1秒
        break;

      case 'k':
        // ++++++++++ Y軸步進電機回到初始位置，即「中間」位置：1024
        stepperY.moveTo(1024);     // Y軸步進電機回到「中間」位置：1024
        stepperY.runToPosition();  // 執行電機轉動指令
        delay(1000);               // 給電機的工作時間：1秒停頓
        break;

      case 'l':
        // ++++++++++ 把現在Y軸步進電機位置設定為「中間」位置：1024
        stepperY.setCurrentPosition(1024);
        break;

      case 'm':
        // ++++++++++ X軸微調（增加）
        stepperX.setCurrentPosition(0);  // 重設現在位置為0
        stepperX.moveTo(xsteps);         // 增加X軸電機位置
        stepperX.runToPosition();        // 執行電機轉動指令
        stepperX.setCurrentPosition(0);  // 重設現在位置為0
        delay(1000);                     // 給電機的工作時間：1秒停頓
        break;

      case 'n':
        // ++++++++++ X軸微調（減少）
        stepperX.setCurrentPosition(0);  // 重設現在位置為0
        stepperX.moveTo(-xsteps);        // 減少X軸電機位置
        stepperX.runToPosition();        // 執行電機轉動指令
        stepperX.setCurrentPosition(0);  // 重設現在位置為0
        delay(1000);                     // 給電機的工作時間：1秒停頓
        break;
      default:
        //
        break;
    }
  }
}
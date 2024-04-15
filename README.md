# Turntable

// +++++++++++ 定義步進電機引腳 ESP32 Lolin Lite
#define LEDPin 2 // GPIO22 - 開發板上的LED
#define PhotoPin 19 // 拍照

// +++++++++++ 步進電機：Y 軸旋轉（前後）
#define motor1Pin1 32 // 28BYJ48連接的ULN2003電機驅動板引腳 in1
#define motor1Pin2 33 // 28BYJ48連接的ULN2003電機驅動板引腳 in2
#define motor1Pin3 25 // 28BYJ48連接的ULN2003電機驅動板引腳 in3
#define motor1Pin4 26 // 28BYJ48連接的ULN2003電機驅動板引腳 in4
// +++++++++++ 步進電機：X 軸旋轉（左右）

#define motor2Pin1 27 // 28BYJ48連接的ULN2003電機驅動板引腳 in1
#define motor2Pin2 14 // 28BYJ48連接的ULN2003電機驅動板引腳 in2
#define motor2Pin3 12 // 28BYJ48連接的ULN2003電機驅動板引腳 in3
#define motor2Pin4 13 // 28BYJ48連接的ULN2003電機驅動板引腳 in4
// +++++++++++ 電機步進方式定義：全步進參數（FULLSTEP）

#define FULLSTEP 4 // 全步進參數（預設）：2048為一圈
#define HALFSTEP 8 // 半步進參數：2048為半圈；4096為一圈

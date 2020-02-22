#define ESP32
#include <M5StickC.h>
#include <M5Display.h>
#include <string>


using namespace std;
/*
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;
*/
bool hai = false;

int c = 0;

void setup() {
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  Serial.begin(9600);
}

void loop() {
  M5.update();
  float accX, accY, accZ;

  // データ取得
  M5.IMU.getAccelData(&accX, &accY, &accZ);

  // 10ループごとに画面の表示を更新
  if (c == 10){
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("x: %f\ny: %f\nz: %f", accX, accY, accZ);
    c = 0;
  }

  Serial.printf("%d\n", getAttitude(accX, accY, accZ));

  c += 1;

  delay(100);
}


/*
静止状態での上になっている辺や面の番号を返します
*/
int getAttitude(float x, float y, float z){
  // 上面(Z軸+方向の面)
  if (isFlat(z, x, y)) return 3;

  // X軸+方向の面
  if (isFlat(x, y, z)) return 7;

  // Y軸+方向の面
  if (isFlat(y, x, z)) return 9;

  // Y軸-方向の面
  if (isFlat(-y, x, z)) return 10;

  // X軸-方向の面
  if (isFlat(-x, y, z)) return 12;

  // 底面(Z軸-方向の面)
  if (isFlat(-z, x, y)) return 16;


  if(littleDiff(z, x) && notLean(y)){
      if (isPositive(z, x))return 1;
      return 18;
    }
  // 
  if(littleDiff(z, y) && notLean(x)) {
    if(isPositive(z, y)) return 2;
    return 17;
  }
  
  if(littleDiff(z, -y) && notLean(x)){
    if (isPositive(z, -y))return 4;
    return 15;
  }
  if(littleDiff(z, -x) && notLean(y)){
    if (isPositive(z, -x))return 5;
    return 14;
  }
  if(littleDiff(x, y) && notLean(z)){
    if (isPositive(x, y)) return 6;
    return 13;
  }
  if(littleDiff(x, -y) && notLean(z)){
    if (isPositive(x, -y)) return 8;
    return 11; 
  }
  // 上記の条件に当てはまらなかった場合角とする
  if (x > 0){
    if (y > 0){
      if (z > 0){
        return 19;
      }
      return 23;
    }
    if (z > 0){
      return 20;
    }
    return 24;
  }
  if (y > 0){
    if (z > 0){
      return 21;
    }
    return 25;
  }
  if (z > 0){
    return 22;
  }
  return 26;
}

/*
ある1軸に対して傾いていないかを返します
*/
bool notLean(float a){
  // ここの0.15fは変える可能性あり
  return abs(a) < 0.2f;
}

/*
aを垂直方向+とした平面上かを判定します
*/
bool isFlat(float a, float b, float c){
  return notLean(a - 1.0f) && notLean(b) && notLean(c);
}

/*
加速度に小さい差があるか判定
*/
bool littleDiff(float a, float b){
  return 0.8f > abs(a - b);
}

/*
引数が両方とも正か判定します
*/
bool isPositive(float a, float b){
  return a > 0 && b > 0;
}
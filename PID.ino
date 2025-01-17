#include <IRremote.h> // IR 리모컨 라이브러리
#define PIN_IR_RECEIVE 2
#define PIN_MOTOR_LEFT_FORWARD 10 
#define PIN_MOTOR_LEFT_BACKWARD 9 
#define PIN_MOTOR_RIGHT_FORWARD 6 
#define PIN_MOTOR_RIGHT_BACKWARD 5
#define PIN_LINE_DETECT_RIGHT A0
#define PIN_LINE_DETECT_LEFT A1
#define REF_VALUE_LINE_DETECT 400

int motorSpeedLeft = 0; // -255~255 
int motorSpeedRight = 0; // -255~255
int isStartLineTracing = 0;
int detectValueLeft = 0;
int detectValueRight = 0;
int calibrationRight = 30;


int threshold = 20; // 오차 무시 임계값

// PID 제어 변수
float Kp = 0.3; // 비례 계수
float Ki = 0.0; // 적분 계수
float Kd = 0.3; // 미분 계수

float error = 0;          // 현재 오차
float lastError = 0;      // 이전 오차
float integral = 0;       // 적분 값
float derivative = 0;     // 미분 값

const int baseSpeed = 100; // 기본 속도

void processMotor(void);

void setup() {
  IrReceiver.begin(PIN_IR_RECEIVE, ENABLE_LED_FEEDBACK);
  pinMode(PIN_MOTOR_LEFT_FORWARD, OUTPUT); 
  pinMode(PIN_MOTOR_LEFT_BACKWARD, OUTPUT); 
  pinMode(PIN_MOTOR_RIGHT_FORWARD, OUTPUT); 
  pinMode(PIN_MOTOR_RIGHT_BACKWARD, OUTPUT);
  analogWrite(PIN_MOTOR_LEFT_FORWARD, 0); 
  analogWrite(PIN_MOTOR_LEFT_BACKWARD, 0); 
  analogWrite(PIN_MOTOR_RIGHT_FORWARD, 0); 
  analogWrite(PIN_MOTOR_RIGHT_BACKWARD, 0);
}

void processMotor(void) {
  if (motorSpeedLeft >= 0){
    analogWrite(PIN_MOTOR_LEFT_FORWARD, motorSpeedLeft); 
    analogWrite(PIN_MOTOR_LEFT_BACKWARD, 0);
  }
  else {
    analogWrite(PIN_MOTOR_LEFT_FORWARD, 0); 
    analogWrite(PIN_MOTOR_LEFT_BACKWARD, -motorSpeedLeft);
  }
  if (motorSpeedRight >= 0){ 
    analogWrite(PIN_MOTOR_RIGHT_FORWARD, motorSpeedRight); 
    analogWrite(PIN_MOTOR_RIGHT_BACKWARD, 0);
  }
  else {
    analogWrite(PIN_MOTOR_RIGHT_FORWARD, 0); 
    analogWrite(PIN_MOTOR_RIGHT_BACKWARD, -motorSpeedRight); 
  }
}

void loop() {
  if (IrReceiver.decode() == true) {
    if (IrReceiver.decodedIRData.protocol == NEC) {
      if (IrReceiver.decodedIRData.command == 0x16) { // * 
        isStartLineTracing = 1;
      }
      else if (IrReceiver.decodedIRData.command == 0x0D) { // # 
        isStartLineTracing = 0;
      }
    }
    IrReceiver.resume();
  }
  
  if (isStartLineTracing == 0) {
    motorSpeedLeft = 0;
    motorSpeedRight = 0;
  }
  else {
    detectValueLeft = analogRead(PIN_LINE_DETECT_LEFT);
    detectValueRight = analogRead(PIN_LINE_DETECT_RIGHT);

    // PID 제어 계산
    error = detectValueLeft - detectValueRight;
    
    // 오차 무시 범위 적용
    if (abs(error) < threshold) {
        error = 0;
        integral = 0; // 적분 값 초기화
    } else {
        integral += error; // 적분 값 누적
    }
    
    derivative = error - lastError;
    lastError = error;

    float pidOutput = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // 모터 속도 계산
    motorSpeedLeft = baseSpeed - pidOutput;
    motorSpeedRight = baseSpeed + pidOutput + calibrationRight;

    // 속도 제한
    motorSpeedLeft = constrain(motorSpeedLeft, -255, 255);
    motorSpeedRight = constrain(motorSpeedRight, -255, 255);
  }

  processMotor();
}

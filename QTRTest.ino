#include <QTRSensors.h>

#define LeftMotorMin 255  
#define RightMotorMin 255  
#define LeftMotorMax 255 
#define RightMotorMax 255 

QTRSensors qtr;

  int PWMA = 9;
  int PWMB = 3;
  float KP = 0.05; //0.1
  float KD = 1.1; //0.5 (8:88), 0.7(8:76), 0.9(8:76), 1.1(8:68),

void setup()
{
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, 8);

  pinMode(LED_BUILTIN, OUTPUT); //LED на Ардуино
  pinMode(6, OUTPUT); //STBY
  pinMode(7, OUTPUT); //AIN1
  pinMode(8, OUTPUT); //AIN2
  pinMode(9, OUTPUT); //PWMA
  pinMode(5, OUTPUT); //BIN1
  pinMode(4, OUTPUT); //BIN2
  pinMode(3, OUTPUT); //PWMB

  digitalWrite(6, HIGH); //STBY
  digitalWrite(7, HIGH); //AIN1
  digitalWrite(8, LOW); //AIN2
  digitalWrite(5, LOW); //BIN1
  digitalWrite(4, HIGH); //BIN2

  delay(1500);
  Calibration();
  Speed(0,0);
}

  int lastError = 0;
  unsigned int sensors[8];
  
void loop()
{
  
  int position = qtr.readLineBlack(sensors); 
  int error = position - 3500;

  int motorSpeed = KP * error + KD * (error - lastError);
  lastError = error;

  int leftMotorSpeed = LeftMotorMin + motorSpeed;
  int rightMotorSpeed = RightMotorMin - motorSpeed;

  Speed(leftMotorSpeed, rightMotorSpeed);
}

void Speed(int leftmotorspeed, int rightmotorspeed)
{
  if (leftmotorspeed > LeftMotorMax ) leftmotorspeed = LeftMotorMax;
  if (rightmotorspeed > RightMotorMax ) rightmotorspeed = RightMotorMax;
  if (leftmotorspeed < 0) leftmotorspeed = 0; 
  if (rightmotorspeed < 0) rightmotorspeed = 0;

  analogWrite(PWMA, leftmotorspeed);
  analogWrite(PWMB, rightmotorspeed);
}


void Calibration() {

  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < 250; i++)
  {
    qtr.calibrate();
    delay(20);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

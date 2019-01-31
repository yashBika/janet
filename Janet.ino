#include <Servo.h>
#include <PID_v1.h>

Servo NovaServo_1; //neck
Servo NovaServo_3; //head rotation
Servo NovaServo_4; //body rotation
Servo NovaServo_5; //head movement

// Joystick
int joystick1_x = A9;
int joystick1_y = A11;
int joystick2_x = A13;
int joystick2_y = A15;

int joystick1_sw = 43;
int joystick2_sw = 45;

int value1_x;
int value1_y;
int value2_x;
int value2_y;

int value1_xF = 90;
int value1_yF = 90;
int value2_xF = 57;
int value2_yF = 130;

int value1_sw;
int value2_sw;

int degree = 90;

// joystick code above

double Setpoint_1 = 90;
double Input_1;
double Output_1;

double Setpoint_2 = 90;
double Input_2;
double Output_2;

double Kp_1 = 0.016;
double Ki_1 = 0.012;
double Kd_1 = 0;

double Kp_2 = 0.028;
double Ki_2 = 0.026;
double Kd_2 = 0;

PID PID1(&Input_1, &Output_1, &Setpoint_1, Kp_1, Ki_1, Kd_1, DIRECT);
PID PID2(&Input_2, &Output_2, &Setpoint_2, Kp_2, Ki_2, Kd_2, DIRECT);

int serialCount = 0;
int serialInArray[2];

int posX = 90;
int posY = 90;

int errorX;
int errorY;


void setup() {
  Serial.begin(9600);

  NovaServo_1.attach(44);
  //NovaServo_2.attach();
  NovaServo_3.attach(36);
  NovaServo_4.attach(38);
  NovaServo_5.attach(42);

  NovaServo_3.write(90);
  NovaServo_4.write(90);
  
  pinMode(joystick1_sw, INPUT_PULLUP);
  pinMode(joystick2_sw, INPUT_PULLUP);
  
  PID1.SetMode(AUTOMATIC);
  PID1.SetSampleTime(1);
  PID1.SetOutputLimits(-35,35);


  PID2.SetMode(AUTOMATIC);
  PID2.SetSampleTime(1);
  PID2.SetOutputLimits(-35,35);


}

void loop() {
  // code for joysticks
  value1_x = analogRead(joystick1_x);
  value1_y = analogRead(joystick1_y);
  value2_x = analogRead(joystick2_x);
  value2_y = analogRead(joystick2_y);
  
  
  value1_sw = digitalRead(joystick1_sw);
  value2_sw = digitalRead(joystick2_sw);
  
  value1_x = map(value1_x, 0, 1024, 0, 100);//min and max angles of head rotation
  value1_y = map(value1_y, 0, 1024, 0, 100);
  value2_x = map(value2_x, 0, 1024, 0, 100);
  value2_y = map(value2_y, 0, 1024, 0, 100);

  /*
  value1_xF = value1_xF * (1 - filter1_x) + value1_x * filter1_x;
  value1_yF = value1_yF * (1 - filter1_y) + value1_y * filter1_y;
  value2_xF = value2_xF * (1 - filter2_x) + value2_x * filter2_x;
  value2_yF = value2_yF * (1 - filter2_y) + value2_y * filter2_y;
  */
  //deadzones
  if(abs(value1_x-50)<20){
    value1_x=0;
  }else{
    value1_x=value1_x-50;
  }
  
  if(abs(value2_x-50)<20){
    value2_x=0;
  }else{
    value2_x=value2_x-50;
  }

  if(abs(value1_y-50)<20){
    value1_y=0;
  }else{
    value1_y=value1_y-50;
  }
  
  if(abs(value2_y-50)<20){
    value2_y=0;
  }else{
    value2_y=value2_y-50;
  }


  value1_xF= value1_xF+(value1_x/10);
  value1_yF= value1_yF+(value1_y/10);  
  value2_xF= value2_xF+(value2_x/10);
  value2_yF= value2_yF+(value2_y/10);

  if(value1_xF>120){
    value1_xF=120;
  }
  
  if (value1_sw == ACTIVATED) {
      if(degree < 141) degree = degree + 2;
  }
  
  if (value2_sw == ACTIVATED) {
      if(degree > 89) degree = degree - 2;
  } 

  /*
  value1_xF = 90;
  value1_yF = 90;
  value2_xF = 57;
  value2_yF = 130;
  */
  if(value1_yF > 80)      NovaServo_1.write(value1_yF);
                          NovaServo_2.write(value2_xF);
  if(value2_yF > 65)      NovaServo_3.write(value2_yF);
                          NovaServo_4.write(value1_xF);
                          NovaServo_5.write(degree);

  delay(30);
  // joystick code above
  
  while(Serial.available() == 0);
  serialInArray[serialCount] = Serial.read();
  serialCount++;

  if (serialCount > 1){

  Input_1 = serialInArray[1];
  Input_2 = serialInArray[0];

  PID1.Compute();
  PID2.Compute();

  posX = posX + Output_2;
  posY = posY + Output_1;

  NovaServo_4.write(posX);

  if(posY > 75)NovaServo_3.write(posY);
  serialCount = 0;
  }

}

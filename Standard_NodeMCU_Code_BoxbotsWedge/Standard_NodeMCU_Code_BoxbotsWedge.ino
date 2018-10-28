/*
    Oct 27 2018
    https://github.com/180Studios/BoxbotsNodeMCU_ESC
    BoxBots robot motor control for NodeMCU & ESP12E Motor Shield 
    AKA: Electronic Speed Control (ESC)
    Converts 2 RC Servo 20ms Pulse width signals to high speed PWM signal for 2 channel bidirectional motor speed control
    Adapted from AMT Boxbots control for arduino

    Hardware: NodeMCU ESP12E mounted on...
              ESP12E Motor Shield with L293D driver. Note this does not support braking

    Radio channel connection:
        CH1 - Steering, connect to D5 on shield board
        CH2 - Throttle, connect to D6 on shield board
        CH3 - Weapon, not used. Suggest connect to D7

    Motor Wiring: 
        looking from front of robot:
          * Left Motor wired to motor terminals A, Red to MINUS (A-) 
          * Right Motor wired to motor terminals B, Red to PLUS (B+)
        looking at front of the axel:
          * Red wires to motor lug that makes motor turn clockwise with positive voltage

    * Battery connected to VM terminals
    * VIN not used
    * Install header shunt on VIN & VM pins
*/


// NodeMCU board signal name -to- Arduino Pin number
#define   D0      16   // USER, WAKE
#define   D1      5    // ~           (~ = PWM)
#define   D2      4    // ~
#define   D3      0    // ~ FLASH
#define   D4      2    // ~
#define   D5      14   // ~
#define   D6      12   // ~
#define   D7      13   // ~RXD2
#define   D8      15   // ~TXD2
#define   RX      3    // ~RXD0
#define   TX      1    // ~TXD0


// STANDARD BOXBOTS WIRING CONFIGURATION
#define   lpwm    D1     // pulse width modulation for left motor(A) 
#define   rpwm    D2     // pulse width modulation for right motor(B) 
#define   ldir    D3     // left direction control 
#define   rdir    D4     // right direction control 
#define   in_ch1  D5    // input channel one, STEERING
#define   in_ch2  D6    // input channel two, THOTTLE


// states
#define   forward 0
#define   reverse 1
#define   coast   2 
#define   rightMotor  0
#define   leftMotor 1
//Note: The EXP12E Motor shield does not support braking

int ch1; // Steering - Joystick x-axis
int ch2; // Thottle - Joystick y-axis
int ch3; // Weapon Switch
int throttle = 0;
int spin = 0;
int rightMotorSpeed = 0;
int leftMotorSpeed = 0;
byte  oldDirection = 0; //for troubleshooting stuttering left motor problem
byte  newDirection = 0;

void motorFunction(byte function, byte motor) {
  switch (motor) {
    case leftMotor:
      switch (function) {
        case forward:
          digitalWrite(ldir, HIGH);
          break;
        case reverse:
          digitalWrite(ldir, LOW);
          break;
      }
      break;
    case rightMotor:
      switch (function) {
        case forward:
          digitalWrite(rdir, HIGH);
          break;

        case reverse:
          digitalWrite(rdir, LOW);
          break;
      }
      break;
    default:
      break;
  }
}

void setup() {
  //Serial.begin(115200);
  delay(10);

  pinMode(lpwm, OUTPUT);
  pinMode(ldir, OUTPUT);
  pinMode(rpwm, OUTPUT);
  pinMode(rdir, OUTPUT);
  
  pinMode(in_ch1, INPUT);       // channel one of RC receiver, x-axis steering
  pinMode(in_ch2, INPUT);       // channel two of RC receiver, y-axis throttle

  digitalWrite(ldir, LOW);
  digitalWrite(rdir, LOW);

  //Serial.println("Initialized");

  }

void loop() {
  // pulsein returning value of 1000 to 2000 (1500 default neutral position)
  // All Numbers are with transmitter channels in Normal position
  ch1 = pulseIn(in_ch1, HIGH, 25000); // Steering : 1000 Left, 2000 Right
  ch2 = pulseIn(in_ch2, HIGH, 25000); // Throttle : 1000 Reverse, 2000 Forward
  //Serial.print(ch1);
  //Serial.print("\t");
  //Serial.print(ch2);

  // handle the case in which the signals time out
  if (ch1 < 800) {
    ch1 = 1500;
  }
  if (ch2 < 800) {
    ch2 = 1500;
  }

  ch1 = map(ch1, 1000, 2000, -1023, 1023); 
  ch2 = map(ch2, 1000, 2000, -1023, 1023); 

  if (abs(ch1) < 10) {
    ch1 = 0;
  }
  if (abs(ch2) < 6) {
    ch2 = 0;
  }
  spin = -0.7 * ch1;
  throttle = -1 * ch2;

  rightMotorSpeed = constrain( throttle + spin, -1023, 1023);
  leftMotorSpeed = constrain( throttle - spin, -1023, 1023 );

  if (rightMotorSpeed < 0) {  // outside deadband, in reverse
    //Serial.print(" Right Back ");
    motorFunction(reverse, rightMotor);
  }
  else {
    //Serial.print(" Right Fwd ");
    motorFunction(forward, rightMotor);
  }
  if (leftMotorSpeed < 0) {
    //Serial.print(" Left Back ");
    motorFunction(reverse, leftMotor);
    newDirection = reverse;
  }
  else {
    newDirection = forward;
    //Serial.print(" Left Fwd ");
    motorFunction(forward, leftMotor);
    leftMotorSpeed = (int)(leftMotorSpeed * 0.85);
  }
  if (oldDirection != newDirection) {
    //Serial.print("@");
  }
  oldDirection = newDirection;

  analogWrite(lpwm, abs(leftMotorSpeed));
  analogWrite(rpwm, abs(rightMotorSpeed));

  //Serial.println("");
 
}
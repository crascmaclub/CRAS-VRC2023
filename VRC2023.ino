// Include necessary lib
#include <Adafruit_PWMServoDriver.h>
#include <PS2X_lib.h>
#include <Wire.h>

// Classes
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
PS2X ps2x;

// Define input PS2
#define PS2_DAT 12 // MISO 
#define PS2_CMD 13 // MOSI 
#define PS2_SEL 15 // SS 
#define PS2_CLK 14 // SLK
#define pressures false
#define rumble false


// Define input Motor DC/Servo (D = (+), A = (-), SV = Servo )
#define DT 8
#define AT 9
#define DP 14
#define AP 15
#define DC 10
#define AC 11
#define SVT 2
#define SVP 3
#define DBD 12
#define ABD 13
#define SVE 4
#define SVD 5

// Define Led RGB
#define RED 0
#define BLUE 2
#define GREEN 25


bool cuon = false; 
bool ban = false; 
bool quaybanh = false;  
int sp = 3071; 
bool boost = false; 
int ext = 950; 
int doc = 1610; 
int led = 0; 
bool nguoc = false; 

void setup() {
 
  Serial.begin(115200); 

  // Setup led
  ledcSetup(RED , 5000, 8);
  ledcSetup(BLUE, 5000, 8);
  pinMode(GREEN,OUTPUT);

  ledcAttachPin(RED, RED);
  ledcAttachPin(BLUE, BLUE);

  digitalWrite(GREEN, HIGH);
  ledcWrite(RED, 255);
  ledcWrite(BLUE, 255);
  
  // --- Startup ViA B ---
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Wire.setClock(400000);

  pwm.writeMicroseconds(SVE, ext); // Set servo extension 
  pwm.writeMicroseconds(SVD, doc); // Set servo steep

  // --- Check error ps2 ---
  int error = -1; 
  for (int i = 0; i < 10; i++)
  {
    delay(1000); 
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble); 
    Serial.print("."); 
    if(!error)
    break;

  }
}


void loop(){
  ps2x.read_gamepad(false, false);  


  // Movement
  if (ps2x.Button(PSB_PAD_UP)) {  // Forward
    pwm.setPWM(DT, 0, sp);
    pwm.setPWM(AT, 0, 0);
    pwm.setPWM(DP, 0, sp);
    pwm.setPWM(AP, 0, 0);
    Serial.println("Đang tiến");
    delay(50); 
  }

  else if (ps2x.Button(PSB_PAD_DOWN)) {  // Backward
    pwm.setPWM(AT, 0, sp);
    pwm.setPWM(DT, 0, 0);
    pwm.setPWM(AP, 0, sp);
    pwm.setPWM(DP, 0, 0);
    Serial.println("Đang lùi");
    delay(50);
  }

  else if (ps2x.Button(PSB_PAD_LEFT)) {  // Left
    pwm.setPWM(DT, 0, 0);
    pwm.setPWM(AT, 0, 0);
    pwm.setPWM(DP, 0, 1638);
    pwm.setPWM(AP, 0, 0);
    Serial.println("Đang rẽ trái");
    delay(50);
  }


  else if (ps2x.Button(PSB_PAD_RIGHT)) {  // Right
    pwm.setPWM(DP, 0, 0);
    pwm.setPWM(AP, 0, 0);
    pwm.setPWM(DT, 0, 1638);
    pwm.setPWM(AT, 0, 0);
    Serial.println("Đang rẽ phải");
    delay(50);
  } 


  else if (ps2x.ButtonReleased(PSB_PAD_UP) || ps2x.ButtonReleased(PSB_PAD_DOWN) || ps2x.ButtonReleased(PSB_PAD_LEFT) || ps2x.ButtonReleased(PSB_PAD_RIGHT)) {
    pwm.setPWM(DT, 0, 0);
    pwm.setPWM(AT, 0, 0);
    pwm.setPWM(DP, 0, 0);
    pwm.setPWM(AP, 0, 0);
    Serial.println("Đã dừng");
    delay(50);
  } 


  // Boost
  if (ps2x.ButtonPressed(PSB_L2)) {
    if (boost == false) {
      sp = 4095;
      boost = true;
      Serial.println("Tốc độ = 100%");
      delay(50);
    }

    else {
      sp = 3071;
      boost = false;
      Serial.println("Tốc độ = 50%");
      delay(50);
    }
  }
  // Intake
  if (ps2x.ButtonPressed(PSB_BLUE)) {
    if (cuon == false) {
      pwm.writeMicroseconds(SVD, 950);
      delay(600);
      pwm.setPWM(DC, 0, 2048);
      pwm.setPWM(AC, 0, 0);
      Serial.println("Đang cuốn");
      cuon = true;
      delay(50);
    }
    else {
      pwm.setPWM(DC, 0, 0);
      pwm.setPWM(AC, 0, 0);
      delay(400);
      pwm.writeMicroseconds(SVD, 1610);
      Serial.println("Dừng cuốn");
      cuon = false;
      delay(50);
    }
  }


  // Reversed
  if (ps2x.ButtonPressed(PSB_GREEN)) {
    if (cuon == false) {
      pwm.writeMicroseconds(SVD, 950);
      delay(600);
      pwm.setPWM(DC, 0, 0);
      pwm.setPWM(AC, 0, 2048);
      Serial.println("Đang nhả");
      cuon = true;
      delay(50);
    }
    else {
      pwm.setPWM(DC, 0, 0);
      pwm.setPWM(AC, 0, 0);
      delay(400);
      pwm.writeMicroseconds(SVD, 1610);
      Serial.println("Dừng nhả");
      cuon = false;
      delay(50);   
    }

  }


  // Shoot
  if (ps2x.ButtonPressed(PSB_R1)) {
    if (ban == false) {
      pwm.writeMicroseconds(SVP, 700);
      pwm.writeMicroseconds(SVT, 2300);
      Serial.println("Bắt đầu bắn bóng");
      ban = true;
      delay(50);
    }
    else {
      pwm.writeMicroseconds(SVT, 1400);
      pwm.writeMicroseconds(SVP, 1400);
      Serial.println("Dừng bắn bóng");
      ban = false;
      delay(50);
    }
  }

  // Emergency button
  if (ps2x.ButtonPressed(PSB_R2)) {
    if (ban == false) {
      pwm.writeMicroseconds(SVT, 700);
      pwm.writeMicroseconds(SVP, 2300);
      Serial.println("Bắt đầu trả bóng");
      ban = true;
      delay(50);
    }
    else {
      pwm.writeMicroseconds(SVT, 1400);
      pwm.writeMicroseconds(SVP, 1400);
      Serial.println("Dừng trả bóng");
      ban = false;
      delay(50);
    }
  }

  // Starting to roll
  if (ps2x.ButtonPressed(PSB_L1)) {
    if (quaybanh == false) {
      pwm.setPWM(DBD, 0, 3276);
      pwm.setPWM(ABD, 0, 0);
      Serial.println("Bắt đầu quay bánh đà");
      quaybanh = true;
      delay(50);
    }
  
    else if (quaybanh == true) {
      pwm.setPWM(DBD, 0, 0);
      pwm.setPWM(ABD, 0, 0);
      Serial.println("Dừng quay bánh đà");
      quaybanh = false;
      delay(50);
    }
  }

  // Reversed
    if (ps2x.ButtonPressed(PSB_L3)) {
    if (nguoc == false) {
      pwm.setPWM(ABD, 0, 3276);
      pwm.setPWM(DBD, 0, 0);
      Serial.println("Bắt đầu quay bánh đà");
      nguoc = true;
      delay(50);
    }
  
    else {
      pwm.setPWM(DBD, 0, 0);
      pwm.setPWM(ABD, 0, 0);
      Serial.println("Dừng quay bánh đà");
      nguoc = false;
      delay(50);
    }
  }

  // Extension
  if (ps2x.Button(PSB_PINK) && ext < 2100) {
    ext += 50;
    pwm.writeMicroseconds(SVE, ext);
    Serial.print("Góc hiện tại theo us là: ");
    Serial.println(ext);
    delay(10);
  }

  if (ps2x.Button(PSB_RED) && ext > 950) {
    ext -= 50;
    pwm.writeMicroseconds(SVE, ext);
    Serial.print("Góc hiện tại theo us là: ");
    Serial.println(ext);
    delay(10);
  }

  // Led Boost
  if (led == 0) {
    if (boost == false) {
      digitalWrite(GREEN, HIGH);
      ledcWrite(RED , 0);
      ledcWrite(BLUE, 0);
    }

    else {
      digitalWrite(25,LOW);
      ledcWrite(RED , 255);
      ledcWrite(BLUE, 0);
    }
  }

  // Off Led
  if (ps2x.ButtonPressed(PSB_SELECT)) {
    if (led == -1) {
      led = 0;
      Serial.println("Bật led");
      delay(50);
    }
    else {
      led = -1;
      digitalWrite(GREEN, LOW);
      ledcWrite(RED, 0);
      ledcWrite(BLUE, 0);
      Serial.println("Tắt led");
      delay(50);
    }
  }

  // Blink led
  if (ps2x.ButtonPressed(PSB_START)) {
    if (led == 0) {
      led = 1;
      Serial.println("Bắt đầu blink");
      delay(50);
    }
    else if (led != 0) {
      led = 0;
      Serial.println("Chuyển led về dạng thường");
      delay(50);
    }
  }


  if (led == 1) {
    digitalWrite(GREEN, HIGH);
    ledcWrite(RED, 255);
    ledcWrite(BLUE, 255);
    led += 1;
    delay(15);
  }

  if (led == 2) {
    digitalWrite(GREEN, LOW);
    ledcWrite(RED, 255);
    ledcWrite(BLUE, 0);
    led += 1;
    delay(15);
  }

  if (led == 3) {
    digitalWrite(GREEN, LOW);
    ledcWrite(RED, 128);
    ledcWrite(BLUE, 0);
    led += 1;
    delay(15);
  }

  if (led == 4) {
    digitalWrite(GREEN, LOW);
    ledcWrite(RED, 255);
    ledcWrite(BLUE, 128);
    led += 1;
    delay(15);
  }

  if (led == 5) {
    digitalWrite(GREEN, LOW);
    ledcWrite(RED, 255);
    ledcWrite(BLUE, 255);
    led += 1;
    delay(15);
  }

  if (led == 6) {
    digitalWrite(GREEN, LOW);
    ledcWrite(RED, 180);
    ledcWrite(BLUE, 255);
    led += 1;
    delay(15);
  }

  if (led == 7) {
    digitalWrite(GREEN, LOW);
    ledcWrite(RED, 110);
    ledcWrite(BLUE, 255);
    led += 1;
    delay(15);
  }

  if (led == 8) {
    digitalWrite(GREEN, LOW);
    ledcWrite(RED, 0);
    ledcWrite(BLUE, 255);
    led += 1;
    delay(15);
  }

  if (led == 9) {
    digitalWrite(GREEN, HIGH);
    ledcWrite(RED, 0);
    ledcWrite(BLUE, 255);
    led += 1;
    delay(15);
  }

  if (led == 10) {
    digitalWrite(GREEN, HIGH);
    ledcWrite(RED, 0);
    ledcWrite(BLUE, 188);
    led += 1;
    delay(15);
  }

  if (led == 11) {
    digitalWrite(GREEN, HIGH);
    ledcWrite(RED, 0);
    ledcWrite(BLUE, 128);
    led += 1;
    delay(15);
  }

  if (led == 12) {
    digitalWrite(GREEN, HIGH);
    ledcWrite(RED, 0);
    ledcWrite(BLUE, 0);
    led += 1;
    delay(15);
  }

  if (led == 13) {
    digitalWrite(GREEN, HIGH);
    ledcWrite(RED, 110);
    ledcWrite(BLUE, 0);
    led += 1;
    delay(15);
  }

  if (led == 14) {
    digitalWrite(GREEN, HIGH);
    ledcWrite(RED, 188);
    ledcWrite(BLUE, 0);
    led += 1;
    delay(15);
  }

  if (led == 15) {
    digitalWrite(GREEN, HIGH);
    ledcWrite(RED, 255);
    ledcWrite(BLUE, 0);
    led += 1;
    delay(15);
  }

  if (led == 16) {
    digitalWrite(GREEN, LOW);
    ledcWrite(RED, 0);
    ledcWrite(BLUE, 0);
    led = 1;
    delay(15);
  }
}


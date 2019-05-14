#include <M5Stack.h>
#include <Wire.h>

#define GOPLUS_ADDR 0x5d

#define SERVO_ADDR 0x10
#define SERVO_NUM0 0x00
#define SERVO_NUM1 0x01
#define SERVO_NUM2 0x02
#define SERVO_NUM3 0x03

#define MOTOR_ADDR 0x30
#define MOTOR_NUM0 0x00
#define MOTOR_NUM1 0x01

#define HUB_ADDR 0x40
#define HUB_NUM0 0x00
#define HUB_NUM1 0x01
#define HUB_NUM2 0x02
#define HUB_NUM3 0x03

#define HUB_READ 0x00
#define HUB_WIRE 0x80

#define HUB_DIGITAL 0x00
#define HUB_ANALOG  0x01


void Servo_write_angle(uint8_t number, uint8_t angle) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(0x10 | number);
    Wire.write(angle);
    Wire.endTransmission();
}

void Motor_write_speed(uint8_t number, uint8_t trun,uint8_t motor_speed) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(0x30 | number);
    Wire.write(trun);
    Wire.write(motor_speed);
    Wire.endTransmission();
}

uint16_t hub1_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x40 | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){
    RegValue_L = Wire.read();
  }
  
  return RegValue_L;   
} 


uint16_t hub1_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x40 | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_L = Wire.read();
    RegValue_H = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 

uint16_t hub2_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x50 | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_L = Wire.read();
    RegValue_H = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 

uint16_t hub3_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x60 | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_L = Wire.read();
    RegValue_H = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 

uint16_t hub2_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x50 | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){
    RegValue_L = Wire.read();
  }
  
  return RegValue_L;   
} 

uint16_t hub3_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x60 | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){
    RegValue_L = Wire.read();
  }
  
  return RegValue_L;   
} 


void hub1_wire_value(uint8_t reg,uint16_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x40 | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 

void hub2_wire_value(uint8_t reg,uint16_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x50 | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 

void hub3_wire_value(uint8_t reg,uint16_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x60 | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 

void setup() {
  // put your setup code here, to run once:
   M5.begin();
   Wire.begin();

   pinMode(13,OUTPUT);
   Motor_write_speed((uint8_t)2, (uint8_t)0x80, 250);
   Motor_write_speed((uint8_t)0, (uint8_t)0x80, 250);
   delay(5000);
   Motor_write_speed((uint8_t)0, (uint8_t)0x80, 0);
   Motor_write_speed((uint8_t)2, (uint8_t)0x00, 0);
   delay(500);


   Motor_write_speed((uint8_t)2, (uint8_t)0x00, 250);
   Motor_write_speed((uint8_t)0, (uint8_t)0x00, 250);
   delay(5000);
   Motor_write_speed((uint8_t)0, (uint8_t)0x80, 0);
   Motor_write_speed((uint8_t)2, (uint8_t)0x00, 0);
   delay(500);

}
uint8_t a = 20;
uint16_t adInValue[6] = {0};
int led = 0;
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, led);
  Servo_write_angle(0,  a); delay(100);
  Servo_write_angle(1 , a); delay(100);
  Servo_write_angle(2 , a); delay(100);
  Servo_write_angle(3 , a); delay(100);

  if (a > 180) {
    a = 10;
  }
  a = a + 170;
  
  //delay(1000);
  
    M5.Lcd.fillScreen(BLACK);
    adInValue[0] = hub1_d_read_value((uint8_t)6);
    delay(50);
    //M5.Lcd.Fill(0, 30 * i, 1);
    M5.Lcd.setCursor(0, 30 * 0, 1);
    M5.Lcd.print(adInValue[0]); 

    adInValue[1] = hub1_d_read_value((uint8_t)7);
    delay(50);
    //M5.Lcd.Fill(0, 30 * i, 1);
    M5.Lcd.setCursor(0, 30 * 1, 1);
    M5.Lcd.print(adInValue[1]); 


    adInValue[2] = hub2_d_read_value((uint8_t)6);
    delay(50);
    //M5.Lcd.Fill(0, 30 * i, 1);
    M5.Lcd.setCursor(0, 30 * 2, 1);
    M5.Lcd.print(adInValue[2]); 
  


    adInValue[3] = hub2_d_read_value((uint8_t)7);
    delay(50);
    //M5.Lcd.Fill(0, 30 * i, 1);
    M5.Lcd.setCursor(0, 30 * 3, 1);
    M5.Lcd.print(adInValue[3]); 

    adInValue[4] = hub3_d_read_value((uint8_t)6);
    delay(50);
    //M5.Lcd.Fill(0, 30 * i, 1);
    M5.Lcd.setCursor(0, 30 * 4, 1);
    M5.Lcd.print(adInValue[4]); 


    adInValue[5] = hub3_d_read_value((uint8_t)7);
    delay(50);
    //M5.Lcd.Fill(0, 30 * i, 1);
    M5.Lcd.setCursor(0, 30 * 5, 1);
    M5.Lcd.print(adInValue[5]); 
    led = !led;
    delay(1000);   
}

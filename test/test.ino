#include <M5Stack.h>
#include <Wire.h>

#define GOPLUS_ADDR 0x2b

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

void Motor_write_speed(uint8_t number, uint8_t trun) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(0x30 | number);
    Wire.write(trun);
    Wire.endTransmission();
}

uint16_t hub_read_value(uint8_t port_num, uint8_t ad_flag){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x40 | port_num);
  Wire.write(0x00 | ad_flag);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_L = Wire.read();
    RegValue_H = Wire.read();
  }
  
  return (RegValue_H << 8)|RegValue_L;   
} 


void hub_wire_value(uint8_t port_num, uint8_t ad_flag,uint16_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(0x40 | port_num);
  Wire.write(0x80 | ad_flag);
  Wire.write(0x00 | ad_flag);
  Wire.write(value & 0xff);
  Wire.write(value >> 8);
  Wire.endTransmission(); 
} 

void setup() {
  // put your setup code here, to run once:
   M5.begin();
   Wire.begin();

}
uint8_t a = 20;
uint16_t adInValue[6] = {0};
void loop() {
  // put your main code here, to run repeatedly:
  
  Servo_write_angle(0,  a); delay(100);
  Servo_write_angle(1 , a); delay(100);
  Servo_write_angle(2 , a); delay(100);
  Servo_write_angle(3 , a); delay(100);

  if (a > 90) {
    a = 20;
  }
  a = a + 20;
  M5.Lcd.fillScreen(BLACK);
  for(int i = 0; i < 6; i++){
    adInValue[i] = hub_read_value((uint8_t)i, 0);
    delay(100);
    //M5.Lcd.Fill(0, 30 * i, 1);
    M5.Lcd.setCursor(0, 30 * i, 1);
    M5.Lcd.print(adInValue[i]); 
  }


   Motor_write_speed((uint8_t)0, (uint8_t)1);delay(100);
   Motor_write_speed((uint8_t)1, (uint8_t)1);delay(100);
}

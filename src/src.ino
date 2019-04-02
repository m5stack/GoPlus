#include <Wire.h>
#include <Servo.h>

//!servo
Servo myservo_0,myservo_1,myservo_2,myservo_3;
const int SERVO_0 = 8;
const int SERVO_1 = 9;
const int SERVO_2 = 10;
const int SERVO_3 = 11; 

void Servo_read_angle(uint8_t number, uint8_t angle) {
    
    switch(number){
      case 0: myservo_0.write(angle); break;
      case 1: myservo_1.write(angle); break;
      case 2: myservo_2.write(angle); break;
      case 3: myservo_3.write(angle); break;
      default:break;
    }
}

//! HUB
//! 10-bit ADC
uint16_t adInValue[6]  = {0};
const int HubPort[6] = {A0,5,A1,6,A2,7}; 
uint8_t port_num = 0;

void hub_wire_value(bool ad_flag, uint8_t value){

  if(ad_flag){
    digitalWrite(HubPort[port_num], value);
  }else
  {
    /* code */
    analogWrite(HubPort[port_num], value);
  }
} 

void hub_read_value(bool ad_flag){

  if(ad_flag){
    adInValue[port_num] = digitalRead(HubPort[port_num]);
  }else
  {
    /* code */
    adInValue[port_num] = analogRead(HubPort[port_num]);
  }

}


//！motor
const int IN_0 = 17;
const int IN_1 = 2;
const int IN_2 = 3;
const int IN_3 = 4; 
void motor_wire_value(uint8_t motor_num,uint8_t trun){

  if(trun == 0){
    if(motor_num){
      digitalWrite(IN_0, 0);
      digitalWrite(IN_1, 0);
    }else
    {
      digitalWrite(IN_2, 0);
      digitalWrite(IN_3, 0);
    } 
  }
  else if(trun == 1)
  {
    /* code */

     if(motor_num){
      digitalWrite(IN_0, 1);
      digitalWrite(IN_1, 0);
    }else
    {
      digitalWrite(IN_2, 1);
      digitalWrite(IN_3, 0);
    } 

  }
  else if(trun == 2){

    if(motor_num){
      digitalWrite(IN_0, 0);
      digitalWrite(IN_1, 1);
    }else
    {
      digitalWrite(IN_2, 0);
      digitalWrite(IN_3, 1);
    } 

  }
}


uint8_t reg;
//uint8_t number = 0;
void receiveEvent(int howMany) {
	uint8_t number = 0; 
  if ((howMany != 2) && (howMany != 3)){
    while (Wire.available() > 0)
      Wire.read();
    return ;
  }
  //! function
  reg = Wire.read();
 
  if(reg & 0xf0) {
    number = reg & 0xf0;
  } else {
    while (Wire.available() > 0)
      Wire.read();
    return;
  }
  

  if(number == 0x30){
     uint8_t motor_num = 0;
     uint8_t turn_flag = 0;
     motor_num = reg & 0x0f;
     turn_flag = Wire.read();
     motor_wire_value(motor_num,turn_flag);  
  } else if(number == 0x40){
     
    port_num = reg & (0x0f);
    uint8_t temp = Wire.read();
    uint8_t rw_flag = temp & 0X80;
    uint8_t ad_flag = temp & 0X01;

    if (rw_flag) {
      uint8_t value = Wire.read();
      hub_wire_value(ad_flag, value);
    }else{
       hub_read_value(ad_flag); 
    }

  }else if(number == 0x10){
     uint8_t angle = 0;
     uint8_t num = 0;
     num = reg & 0x0f;
     angle = Wire.read();
     Servo_read_angle(num, angle);
  }
#if 0
  switch(number)
  { 
    case 0x10:
      uint8_t angle = 0;
      uint8_t num = 0;
      num = reg & 0x0f;
      angle = Wire.read();
      Servo_read_angle(num, angle);
      break;
    case 0x20:
      port_num = reg & (0x0f);
      uint8_t temp = Wire.read();
      bool rw_flag = temp & 0X80;
      bool ad_flag = temp & 0X01;
      
      if (rw_flag) {
        /* code */
        uint8_t value = Wire.read();
        hub_wire_value(ad_flag, value);

      }else
      {
        /* code */
         hub_read_value(ad_flag);
         
         //Servo_read_angle(0, a);
         //a = a + 20;
         //if(a > 180) a = 1;
      }
      break;

    case 0x30:
         motor_wire_value(0,0);
        motor_wire_value(1,0);
        uint8_t motor_num = 0;
        uint8_t turn_flag = 0;
        motor_num = reg & 0x0f;
        turn_flag = Wire.read();
       // uint8_t speed = Wire.read();;
       // motor_wire_value(motor_num,turn_flag);
      break;
    default:
      break;
  }
  #endif
}

void requestEvent() {
  switch(reg & 0xf0){
    case 0x40:
      Wire.write((uint8_t)(adInValue[port_num]&0xff));
      Wire.write((uint8_t)(adInValue[port_num]>>8)&0x03);
    break;

    default:break;
  }
  /*
  switch(reg & 0xf0)
  {
    case 0x00:
      Wire.write((uint8_t)(level));
      break;    
    case 0x01:  
      Wire.write((uint8_t)(sensorValue_A>>8)&0x03);
      Wire.write((uint8_t)(sensorValue_A&0xff));
      break;
    case 0x02:  
      Wire.write((uint8_t)(sensorValue_B>>8)&0x03);
      Wire.write((uint8_t)(sensorValue_B&0xff));
      break;
    case 0x03:  
      Wire.write((uint8_t)(sensorValue_C>>8)&0x03);
      Wire.write((uint8_t)(sensorValue_C&0xff));
      break;
    case 0x04:  
      Wire.write((uint8_t)(sensorValue_D>>8)&0x03);
      Wire.write((uint8_t)(sensorValue_D&0xff));
      break;

    case 0x05:  
      Wire.write((uint8_t)(ThresholdValue_A>>8)&0x03);
      Wire.write((uint8_t)(ThresholdValue_A&0xff));
      break;

    case 0x06:  
      Wire.write((uint8_t)(ThresholdValue_B>>8)&0x03);
      Wire.write((uint8_t)(ThresholdValue_B&0xff));
      break;

    case 0x07:  
      Wire.write((uint8_t)(ThresholdValue_C>>8)&0x03);
      Wire.write((uint8_t)(ThresholdValue_C&0xff));
      break;

    case 0x08:  
      Wire.write((uint8_t)(ThresholdValue_D>>8)&0x03);
      Wire.write((uint8_t)(ThresholdValue_D&0xff));
      break;
    default:
	  break;
  }
  */
}
void setup() {
  // put your setup code here, to run once:
  Wire.begin(0x2b);
  Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);


  myservo_0.attach(SERVO_0); 
  myservo_1.attach(SERVO_1); 
  myservo_2.attach(SERVO_2); 
  myservo_3.attach(SERVO_3); 


  pinMode(IN_0, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);

}

void loop() {

}

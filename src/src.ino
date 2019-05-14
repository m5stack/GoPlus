#include <Wire.h>
#include <Servo.h>
//#include <MsTimer2.h> 

//!servo
Servo myservo_0,myservo_1,myservo_2,myservo_3;
const int SERVO_0 = 8;
const int SERVO_1 = 17;
const int SERVO_2 = 2;
const int SERVO_3 = 4; 
uint8_t  SERVO_flag[4] = {0,0,0,0};

void Servo_read_angle(uint8_t number, uint8_t angle) {
    
    switch(number){
      case 0: if(!SERVO_flag[0]) myservo_0.attach(SERVO_0);SERVO_flag[0] = 1; myservo_0.write(angle); break;
      case 1: if(!SERVO_flag[1]) myservo_1.attach(SERVO_1);SERVO_flag[1] = 1; myservo_1.write(angle); break;
      case 2: if(!SERVO_flag[2]) myservo_2.attach(SERVO_2);SERVO_flag[2] = 1; myservo_2.write(angle); break;
      case 3: if(!SERVO_flag[3]) myservo_3.attach(SERVO_3);SERVO_flag[3] = 1; myservo_3.write(angle); break;
      default:break;
    }
}

//! HUB
//! 10-bit ADC
uint16_t adInValue[6]  = {0};
const int HubPort[6] = {A0,5,A1,6,A2,7}; 
//uint8_t port_num = 0;
/*
void hub_wire_value(bool ad_flag, uint8_t value){

  if(ad_flag){
    pinMode(HubPort[port_num],OUTPUT);
    digitalWrite(HubPort[port_num], value);
  }else
  {
    analogWrite(HubPort[port_num], value);
  }
} 

void hub_read_value(bool ad_flag){

  if(ad_flag){
    pinMode(HubPort[port_num],INPUT);
    adInValue[port_num] = digitalRead(HubPort[port_num]);
  }else
  {
    adInValue[port_num] = analogRead(HubPort[port_num]);
  }

}
*/




//！motor
const int IN_0 = 9;
const int IN_1 = 10;
const int IN_2 = 11;
const int IN_3 = 3; 

int duty0  = 0, duty1 = 0;
uint8_t duty_trun0 = 0,duty_trun1 = 0;
uint8_t duty_speed0 = 0,duty_speed1 = 0;
uint8_t duty_motor_0_flag = 0,duty_motor_1_flag = 0;
uint8_t time_flag = 0;

void motor_wire_value(uint8_t motor_num, uint8_t trun, uint8_t motor_speed){
  
  //duty_motor_num = motor_num;
  if(motor_speed == 0){
    if(motor_num == 0){
      
      digitalWrite(IN_0, 0);
      digitalWrite(IN_1, 0);
      duty_trun0 = trun;
      duty_speed0 =  motor_speed;
      //MsTimer2::stop(); 
      //time_flag = 0;
      duty0 = 0;
      duty_motor_0_flag = 0;

      //analogWrite(IN_0, 0);
      //analogWrite(IN_1, 0);
    }else if(motor_num == 2)
    {
     // digitalWrite(IN_2, 0);
      //digitalWrite(IN_3, 0);

      digitalWrite(IN_2, 0);
      digitalWrite(IN_3, 0);
      duty_trun1 = trun;
      duty_speed1 =  motor_speed;
      //MsTimer2::stop(); 
      //time_flag = 0;
      duty1 = 0;
      duty_motor_1_flag = 0;

      // analogWrite(IN_2, 0);
      // analogWrite(IN_3, 0);
    } 
  }
  else if(trun == 0x00)
  {
     if(motor_num == 0){
     // digitalWrite(IN_0, 1);
     // digitalWrite(IN_1, 0);
     duty_motor_0_flag = 1;
      //analogWrite(IN_0, motor_speed);
      //analogWrite(IN_1, 0);
     duty_trun0 = trun;
     duty_speed0 =  motor_speed;
     //MsTimer2::start();
     time_flag = 1;

    }else if(motor_num == 2)
    {
      //analogWrite(IN_2, motor_speed);
      //analogWrite(IN_3, 0);
      duty_motor_1_flag = 1;
      duty_trun1 = trun;
      duty_speed1 =  motor_speed;
      //MsTimer2::start();
      time_flag = 1;
    } 

  }
  else if(trun == 0x80){

    if(motor_num == 0){
     // digitalWrite(IN_0, 0);
      //digitalWrite(IN_1, 1);

      //analogWrite(IN_0, 0);
     /// analogWrite(IN_1, motor_speed);
      duty_motor_0_flag = 1;
      duty_trun0 = trun;
      duty_speed0 =  motor_speed;
      //MsTimer2::start();
      time_flag = 1;
    }else if(motor_num == 2)
    {
      //digitalWrite(IN_2, 0);
      //digitalWrite(IN_3, 1);

      //analogWrite(IN_2, 0);
      //analogWrite(IN_3, motor_speed);
      duty_motor_1_flag = 1;
      duty_trun1 = trun;
      duty_speed1 =  motor_speed;
     // MsTimer2::start();
      time_flag = 1;
    } 

  }
}


uint8_t reg;
//uint8_t number = 0;
void receiveEvent(int howMany) {
	uint8_t number = 0; 
  if ((howMany != 1) &&(howMany != 2) && (howMany != 3) && (howMany != 4)){
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
     uint8_t motor_speed = 0;
     motor_num = reg & 0x0f;
     turn_flag = Wire.read();
     motor_speed = Wire.read();
     motor_wire_value(motor_num,turn_flag,motor_speed);  
  } else if(number == 0x40){
     
    uint8_t port_num = reg & (0x0f);
    uint8_t value = 0;
    switch(port_num){
      case 0:  value = Wire.read();
               pinMode(HubPort[0],OUTPUT);
               digitalWrite(HubPort[0], value);
               break;
      case 1:  value = Wire.read();
               pinMode(HubPort[1],OUTPUT);
               digitalWrite(HubPort[1], value);
               break;
      case 2:  value = Wire.read();
               analogWrite(HubPort[0], value);
               break;
      case 3:  value = Wire.read();
               analogWrite(HubPort[1], value);
               break;
      case 4:  pinMode(HubPort[0],INPUT_PULLUP);
               adInValue[0] = digitalRead(HubPort[0]);
               break;
      case 5:  pinMode(HubPort[1],INPUT_PULLUP);
               adInValue[1] = digitalRead(HubPort[1]);
               break;
      case 6:  
              pinMode(HubPort[0],INPUT_PULLUP);
              adInValue[0] = analogRead(HubPort[0]);
               break;
      case 7:   pinMode(HubPort[1],INPUT_PULLUP);
               adInValue[1] = analogRead(HubPort[1]);
               break;
      default:break;
    }

  }else if(number == 0x50){
     uint8_t port_num = reg & (0x0f);
     uint8_t value = 0;
     switch(port_num){
       case 0:  value = Wire.read();
               pinMode(HubPort[2],OUTPUT);
               digitalWrite(HubPort[2], value);
               break;
      case 1:  value = Wire.read();
               pinMode(HubPort[3],OUTPUT);
               digitalWrite(HubPort[3], value);
               break;
      case 2:  value = Wire.read();
               analogWrite(HubPort[2], value);
               break;
      case 3:  value = Wire.read();
               analogWrite(HubPort[3], value);
               break;
      case 4:  pinMode(HubPort[2],INPUT_PULLUP);
               adInValue[2] = digitalRead(HubPort[2]);
               break;
      case 5:  pinMode(HubPort[3],INPUT_PULLUP);
               adInValue[3] = digitalRead(HubPort[3]);
               break;
      case 6: pinMode(HubPort[2],INPUT_PULLUP); 
              adInValue[2] = analogRead(HubPort[2]);
               break;
      case 7:  pinMode(HubPort[3],INPUT_PULLUP); 
               adInValue[3] = analogRead(HubPort[3]);
              break;
      default:break;
      }

  }else if(number == 0x60){
      uint8_t port_num = reg & (0x0f);
      uint8_t value = 0;
      switch(port_num){
        case 0:  value = Wire.read();
                 pinMode(HubPort[4],OUTPUT);
               digitalWrite(HubPort[4], value);
               break;
        case 1:  value = Wire.read();
               pinMode(HubPort[5],OUTPUT);
               digitalWrite(HubPort[5], value);
               break;
        case 2:  value = Wire.read();
               analogWrite(HubPort[4], value);
               break;
        case 3:  value = Wire.read();
               analogWrite(HubPort[5], value);
               break;
        case 4:  pinMode(HubPort[4],INPUT_PULLUP);
               adInValue[4] = digitalRead(HubPort[4]);
               break;
        case 5:  pinMode(HubPort[5],INPUT_PULLUP);
               adInValue[5] = digitalRead(HubPort[5]);
               break;
        case 6:pinMode(HubPort[4],INPUT_PULLUP);   
               adInValue[4] = analogRead(HubPort[4]);
               break;
        case 7:pinMode(HubPort[5],INPUT_PULLUP);  
              adInValue[5] = analogRead(HubPort[5]);
              break;
        default:break;
        }

  }
  else if(number == 0x10){
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
      break;
    default:
      break;
  }
  #endif
}

void requestEvent() {
  uint8_t port_num = reg & (0x0f);
  switch(reg & 0xf0){
    case 0x40:
      switch(port_num){
      case 4:
        Wire.write((uint8_t)(adInValue[0]&0xff));
        break;
      case 5:
        Wire.write((uint8_t)(adInValue[1]&0xff));
        break;
      case 6:
        Wire.write((uint8_t)(adInValue[0]&0xff));
        Wire.write((uint8_t)(adInValue[0]>>8)&0x03);
        break;

      case 7:
        Wire.write((uint8_t)(adInValue[1]&0xff));
        Wire.write((uint8_t)(adInValue[1]>>8)&0x03);
        break;
      }
    break;
    case 0x50:
       switch(port_num){
      case 4:
        Wire.write((uint8_t)(adInValue[2]&0xff));
        break;
      case 5:
        Wire.write((uint8_t)(adInValue[3]&0xff));
       
        break;
      case 6:
        Wire.write((uint8_t)(adInValue[2]&0xff));
        Wire.write((uint8_t)(adInValue[2]>>8)&0x03);
        break;

      case 7:
        Wire.write((uint8_t)(adInValue[3]&0xff));
        Wire.write((uint8_t)(adInValue[3]>>8)&0x03);
        break;
      }
    break;
    case 0x60:
       switch(port_num){
      case 4:
        Wire.write((uint8_t)(adInValue[4]&0xff));
        break;
      case 5:
        Wire.write((uint8_t)(adInValue[5]&0xff));
        break;
      case 6:
        Wire.write((uint8_t)(adInValue[4]&0xff));
        Wire.write((uint8_t)(adInValue[4]>>8)&0x03);
        break;

      case 7:
        Wire.write((uint8_t)(adInValue[5]&0xff));
        Wire.write((uint8_t)(adInValue[5]>>8)&0x03);
        break;
      }
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

void Timer(void){  
  if(duty_motor_0_flag){    
  duty0++;
  if(duty0 >= 128)
     duty0 = 0;
  }
 if(duty_motor_1_flag){  
 duty1++;
  if(duty1 >= 128)
     duty1 = 0;
 }

}
void setup() {
  // put your setup code here, to run once:
 /*
  pinMode(SERVO_0,OUTPUT);
  pinMode(SERVO_1,OUTPUT);
  pinMode(SERVO_2,OUTPUT);
  pinMode(SERVO_3,OUTPUT);

  digitalWrite(SERVO_0,HIGH);
  digitalWrite(SERVO_1,HIGH);
  digitalWrite(SERVO_2,HIGH);
  digitalWrite(SERVO_3,HIGH);

  */
  Wire.begin(0x5d);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);


  //myservo_0.attach(SERVO_0); 
  //myservo_1.attach(SERVO_1); 
  //myservo_2.attach(SERVO_2); 
  //myservo_3.attach(SERVO_3); 


  //pinMode(IN_0, OUTPUT);
  //pinMode(IN_1, OUTPUT);
 


  ///MsTimer2::set(1, Timer); 
  pinMode(IN_0, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  motor_wire_value(2,0x00,0);
  motor_wire_value(0,0x00,0);
}

uint8_t pwmflag0 = 0;
uint8_t pwmflag1 = 0;
uint8_t pwmflag2 = 0;
uint8_t pwmflag3 = 0;

void loop() {

#if 0
if(duty_motor_0_flag){

  if(duty_trun0 == 0x80){

      if(duty0 < duty_speed0){
        digitalWrite(IN_0, 1);
        //digitalWrite(IN_1, 0);
      } else {
        digitalWrite(IN_0, 0);
        //digitalWrite(IN_1, 0);
      }
      digitalWrite(IN_1, 0);
  }else if(duty_trun0 == 0x00){
     if(duty0 < duty_speed0){
       //digitalWrite(IN_0, 0);
       digitalWrite(IN_1, 1);
      } else {
       //digitalWrite(IN_0, 0);
       digitalWrite(IN_1, 0);
      }
      digitalWrite(IN_0, 0);
  }
 // else {
    // digitalWrite(IN_0, 0);
    // digitalWrite(IN_1, 0);
  //}
}

if(duty_motor_1_flag){
  
    if(duty_trun1 == 0x80){
      if(duty1 < duty_speed1){
        digitalWrite(IN_2, 1);
      } else {
        digitalWrite(IN_2, 0);
      }
      digitalWrite(IN_3, 0);

  }else if(duty_trun1 == 0x00){
     if(duty1 < duty_speed1){
        //digitalWrite(IN_2, 0);
        digitalWrite(IN_3, 1);
      } else {
        //digitalWrite(IN_2, 0);
        digitalWrite(IN_3, 0);
      }
      digitalWrite(IN_2, 0);
  }
  //else {
   // digitalWrite(IN_2, 0);
   // digitalWrite(IN_3, 0);
  //}
}
#else
  if(duty_motor_0_flag){
    switch(duty_trun0){
      case 0x00:
        if(duty0 < duty_speed0/2){
          if(!pwmflag0){
            digitalWrite(IN_1, LOW);
            digitalWrite(IN_0, HIGH);
            pwmflag0 = 1;
          }
        } else {
          if(pwmflag0){
            digitalWrite(IN_0, LOW);
            digitalWrite(IN_1, LOW);
            pwmflag0 = 0;
          }
        }
        break;
      case 0x80:
        if(duty0 < duty_speed0/2){
           if(!pwmflag1){
             digitalWrite(IN_0, LOW);
             digitalWrite(IN_1, HIGH);
             pwmflag1 = 1;
           }
        } else {
          if(pwmflag1){
            digitalWrite(IN_0, LOW);
            digitalWrite(IN_1, LOW);
            pwmflag1 = 0;
          }
        }
        break;
      default:break;
    }
  }

  if(duty_motor_1_flag){
    switch(duty_trun1){
      case 0x80:
        if(duty1 < duty_speed1/2){
          if(!pwmflag2){
            digitalWrite(IN_3, LOW);
            digitalWrite(IN_2, HIGH);
            pwmflag2 = 1;
          }
        } else {
          if(pwmflag2){
            digitalWrite(IN_2, LOW);
            digitalWrite(IN_3, LOW);
            pwmflag2 = 0;
          }
        }
        break;
      case 0x00: 
         if(duty1 < duty_speed1/2){
           if(!pwmflag3){
             digitalWrite(IN_2, LOW);
             digitalWrite(IN_3, HIGH);
             pwmflag3 = 1;
           }
        } else {
          if(pwmflag3){
            digitalWrite(IN_2, LOW);
            digitalWrite(IN_3, LOW);
            pwmflag3 = 0;
          }
        }  
        break;
      default:break;
    }

  }

#endif


 if(time_flag){
   Timer();
 }
 
}

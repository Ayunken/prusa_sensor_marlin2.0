
/**
  Firmware para Attiny85 (Digispark 16Mhz)
  Lee la posicion Y del sensor PAT9125 y si hay diferencia con el anterior valor
  desencadena un pulso en el pin out y enciende el builtin LED
  pin I2S: 0 SDA, 2 CLK
  pint out: 3
  establecer trgtime con el valor en ms de la duración del pulso 
**/

#include "pat9125.h"

#define MCU_AT85  //ATTINY 85
//#define MCU_AUNO //MICRO/UNO

// ATTINY85
#ifdef MCU_AT85
  #define SWI2C_SDA         0 //SDA on P0
  #define SWI2C_SCL         2 //SCL on P2
  #define LEDPIN            1 // led and output to printer board as encoder

#endif

#ifdef MCU_AUNO 
//ARDUINO UNO
  #define SWI2C_SDA         2 //SDA 
  #define SWI2C_SCL         3 //SCL 
  #define LEDPIN            13 //led and output to printer board as encoder
#endif

#define THRESHOLD 12  // THRESHOLD for pin trigering
#define TRGTIME 100 // Pin trigrring duration in ms

int16_t Delta_X=0; // Current delta y

cPAT9125 pat9125(SWI2C_SDA, SWI2C_SCL, 240, 240);

void trigger_pin();


void setup()
{ //Marlin_main.cpp
    #ifdef MCU_AUNO
        Serial.begin(115200);  
    #endif    
    //fsensor_autoload_set(true);
    
    pinMode(LEDPIN,OUTPUT);
    digitalWrite(LEDPIN,LOW);
  
    
    for (int i=0;i<2;i++)
    {
      digitalWrite(LEDPIN,HIGH);
      delay(100);  
      digitalWrite(LEDPIN,LOW);
      delay(100);  
    }

    #ifdef MCU_AUNO
      if (pat9125.IsInit())
      {
        Serial.println("PAT9125 initialization complite");
        
      }
      else
      {
          Serial.println("PAT9125 Not init");
      }
    #endif
    //Serial.println(pat9125);
    
}

void loop()
{
     
     Delta_X+=pat9125.Get_delta_x(); // Use Get_delta_y if you plase sensor with 90 degrees
     
     if (abs(Delta_X)>THRESHOLD) 
      {
        #ifdef MCU_AUNO
        Serial.println("Delta X: " + String( Delta_X));
        #endif
        Delta_X=0;
        trigger_pin();
      }
     //if (pat9125_y==yy and state==1)
     //     swap_pin();      
     
     //delay(100);
}

void trigger_pin()
{
    #ifdef MCU_AUNO
      Serial.println("TRIGGER");
    #endif  
    digitalWrite(LEDPIN,HIGH);
    delay(TRGTIME);
    digitalWrite(LEDPIN,LOW);
    delay(100);
    
}
    

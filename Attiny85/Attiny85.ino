
/**
  Firmware para Attiny85 (Digispark 16Mhz)
  Lee la posicion Y del sensor PAT9125 y si hay diferencia con el anterior valor
  desencadena un pulso en el pin out y enciende el builtin LED
  pin I2S: 0 SDA, 2 CLK
  pint out: 3
  establecer trgtime con el valor en ms de la duración del pulso 
**/


#include "pat9125.h"
/*
void printSerial(const char *fmt, ...)
{
  char tmp[100];
  va_list args;
// Obtenemos la lista de argumentos
  va_start (args, fmt );
  // Escribimos en tmp, con tamaÃƒÂ±o MAX_CADENA, la cadena de formato serÃƒÂ¡ fmt y los
  // argumentos args
  vsnprintf(tmp, 100, fmt, args);
  va_end (args);
  Serial.print(tmp);
 
}*/

#define MCU_AT85  //ATTINY 85
//#define MCU_AUNO //MICRO/UNO

#ifdef MCU_AT85
 int led=1; // builtin led
 int out=3; // output to printer board as encoder
#endif
#ifdef MCU_AUNO
  // UNO
  int led=13; // builtin led
  int out=4; // output to printer board as encoder
#endif*/

long yy=0; // last y value
int ydelta=1; // relativo change in y to trigger
int trgtime=100; // ms duration pulse 
//boolean state; // save last output state

void trigger_pin(){
    #ifdef MCU_AUNO
      Serial.println("TRIGGER");
    #endif  
    digitalWrite(led,HIGH);
    digitalWrite(out,HIGH);
    delay(trgtime);
    digitalWrite(led,LOW);
    digitalWrite(out,LOW);
    delay(100);
    
}

/*
/void swap_pin(){
    state=!state;
    digitalWrite(led,state?HIGH:LOW);
    digitalWrite(out,state?HIGH:LOW);
}
*/
void setup(){ //Marlin_main.cpp
    #ifdef MCU_AUNO
        Serial.begin(9600);  
    #endif    
    //fsensor_autoload_set(true);
    
    pinMode(led,OUTPUT);
    pinMode(out,OUTPUT);
    digitalWrite(led,HIGH);
    
    uint8_t pat9125 = pat9125_init();
    
    for (int i=0;i<2;i++){
    digitalWrite(led,HIGH);
    delay(100);  
    digitalWrite(led,LOW);
    delay(100);  
    }
     
    //Serial.print("PAT9125_init:");
    //Serial.println(pat9125);
    
}

void loop(){
     
     pat9125_update(); //update sensor
     
     if (abs(pat9125_y-yy)>ydelta) 
       {
        #if MCU_AUNO
        Serial.println("x"+String( yy)+" y:" + String( pat9125_y));
        #endif
        yy=pat9125_y;
        trigger_pin();}
     //if (pat9125_y==yy and state==1)
     //     swap_pin();      
     
     //delay(100);
}
    
/*
// MarLlin_main.cpp functions
void manage_inactivity(bool ignore_stepper_queue){
        if (fsensor_check_autoload())
        {
          fsensor_autoload_check_stop();
          Serial.println("No detecto")
      }
      else
      {
        fsensor_autoload_check_stop();
        fsensor_update();
      }
  
}

void process_commands(){ //Marlin_main.cpp
  #ifdef PAT9125
  else if (code_seen("FSENSOR_RECOVER")) { //! FSENSOR_RECOVER
      fsensor_restore_print_and_continue();
    }
  #endif //PAT9125
}

void gcode_M701(){ 
  //Medficion de calidad
   fsensor_oq_meassure_start(40);
  
  /* se carga filamento ....
  
  
        fsensor_oq_meassure_stop();

        if (!fsensor_oq_result())
        {
            Serial.println("Fil. sensor response is poor, disable it?")
            //fsensor_disable(); // para deshabilitar el sensor
        }
  
}
  
	void dcode_9125()
	{
    //debug
  		Serial.println("x=%d y=%d b=%d s=%d\n", pat9125_x, pat9125_y, pat9125_b, pat9125_s);
		
		
	}
 */

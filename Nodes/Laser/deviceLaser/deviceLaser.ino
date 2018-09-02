/*
 * NodeMCU para receptor de barrera láser
 * MOSI AGIL
 * GISAI 2017
 */
 
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>     

//Definimos el pin de salida del láser
int PinLS = 2;       

Adafruit_MMA8451 mma = Adafruit_MMA8451();
double max_error = 1.5;


/* 
 *  CONFIGURACIÓN
 */
void setup() {
  Serial.begin(115200);
 
  pinMode(PinLS, OUTPUT);                //Inicializamos
  digitalWrite(PinLED, LOW);              //Dejamos inicialmente el láser apagado  

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    //while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); 
  Serial.print(2 << mma.getRange());  
  Serial.println("G");
}

 
/* 
 *  BUCLE PRINCIPAL
 */
void loop() 
{

  /* EVENTO DE LECTURA */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  while (abs(event.acceleration.x) > max_error || abs(event.acceleration.z) > max_error) {
    //se ha producido una mala colocación   
  } 

  digitalWrite(PinLS, HIGH);
}

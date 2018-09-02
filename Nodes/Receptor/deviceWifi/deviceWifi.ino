/*
 * NodeMCU para receptor de barrera láser
 * MOSI AGIL
 * GISAI 2017
 */
 
#include <ESP8266WiFi.h>                  //Incluye la librería ESP8266WiFi
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

#define POWER 0  //Potencia en dBm

// Pin del fotorresistor
#define fr1 A0; //entrada 0 analógica

const char ssid[] = "gateway";   
const char password[] = "1234567890";

const char* host = "192.168.1.1";                 

//Definimos el pin de salida - GPIO2 / D4
int PinLED = 2;       

Adafruit_MMA8451 mma = Adafruit_MMA8451();
double max_error = 1.5;



/* 
 *  CONFIGURACIÓN
 */
void setup() {
  Serial.begin(115200);
 
  pinMode(PinLED, OUTPUT);                //Inicializamos el GPIO2 como salida
  digitalWrite(PinLED, HIGH);              //Dejamos inicialmente el GPIO2 apagado
 
  WiFi.mode(WIFI_STA);

  WiFi.setOutputPower(POWER);
  //WiFi.softAP(ssid, password,3,1);      //Red con clave, en el canal 3 y visible 
  //WiFi.softAP(ssid);                    //Red abierta

  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //mientras que no estes conectado, no sale de aqui
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado al wifi");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  
  Serial.println();
 
  Serial.print("Direccion IP Access Point - por defecto: ");      //Imprime la dirección IP
  Serial.println(WiFi.softAPIP()); 
  Serial.print("Direccion MAC Access Point: ");                   //Imprime la dirección MAC
  Serial.println(WiFi.softAPmacAddress()); 
 
  //IPAddress local_ip(192, 168, 1, 1);                           //Modifica la dirección IP 
  //IPAddress gateway(192, 168, 1, 1);   
  //IPAddress subnet(255, 255, 255, 0);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  //Serial.println();
  //Serial.print("Access Point - Nueva direccion IP: ");
  //Serial.println(WiFi.softAPIP());

  Serial.println("Adafruit MMA8451 test!");
  

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
  // Comprueba si el cliente ha conectado
  WiFiClient client;      //creo el cliente
  const int httpPort = 3000; 
   if (!client.connect(host, httpPort)) { //si no me conecto
    Serial.println("conexion fallida");
    return;     //empiezo el loop de nuevo
  }

  /* EVENTO DE LECTURA */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  while (abs(event.acceleration.x) > max_error || abs(event.acceleration.z) > max_error) {
    //se ha producido una mala colocación   
  } 

  ultimoValor= analogRead(fr1);
  Serial.println(ultimoValor);
  if(ultimoValor < 700){ //Por encima de 700 es el valor normal devuelto al incidir el laser.
    Serial.println("Han traspasado la barrera!");
    delay(1000);
  }

  //Envío avanzado de datos si procede
  String body="{position:OK,detectedPeople:1}";
  String url = "/";  

  Serial.println(body);
  client.print(String("PUT ") + url + " HTTP/1.1\r\n"
  "Host: " + host + "\r\n" +
  "Content-Type: application/x-www-form-urlencoded\r\n"+
  "Content-Length:");
  client.println(body.length());
  client.print("Connection: close \r\n\r\n"+
  body ); // terminar la petecion 
}

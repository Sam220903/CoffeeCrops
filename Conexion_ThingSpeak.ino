/* 
Title : Environment monitor for variables in coffee crops
Description: Project for participation in LACCEI Hackaton 2024

------------------------ Team members --------------------------
              Mentor (from Engineering department)
                 Jorge Rafael Aguilar Cisneros 

           Students (from Software Engineering career)
                      Pablo Serrano Limón
                   Luis Fernando Calva Torres
                     Samuel Pérez Guarneros
*/








#include <Wire.h> 
#include <Adafruit_Sensor.h> // SENSORES I2C
//#define BLYNK_PRINT Serial // Comente esto para deshabilitar las impresiones y ahorrar espacio
#include <SPI.h>
//******libreria ESP8266WiFi**********///
#include <ESP8266WiFi.h> //ESTE
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#include <Servo.h>
#include <DHT.h>           //Cargamos la librería DHT
#include <DHT_U.h>
////////////***********DHT*************//////***********************************
#define DHTPIN    D5  
DHT dht(DHTPIN, DHT11);// STAR AND OBJECT DHT
//*******************LUX****************
#include <BH1750.h>//luz uv
BH1750 lightMeter; //sensor de luz

//***********************/
//char auth[] = BLYNK_AUTH_TOKEN;
char ssid1[] = ""; // ingrese nombre de red wifi
char pass1[] = "";// ingrese contaseña de la red wifi
char ssid2[] = ""; // ingrese nombre de red wifi
char pass2[] = ""; // ingrese contaseña de la red wifi
// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t TiempoEsperaWifi = 5000;
//SimpleTimer timer;


//--------conexion a google-----------Host & httpsPort
const char* host = "api.thingspeak.com";
const int httpsPort = 443;
//const int httpPort = 80;
//-conexion a cliente --------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.
//String GAS_ID = "AKfycbzYowYUc0SzufjQKiBOV5aohftRyOz4aqS2xw2y8f9vCNyeBHic1TQzs8RtHpZ7wqUixg"; //--> spreadsheet script ID
String API = "";   // Write API KEY (Personalizada para cada canal de ThingSpeak)

void setup() {
 // Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("\n ESP8266 Multi WiFi iniciando..");
//*** Register multi WiFi networks*******************************
  wifiMulti.addAP(ssid1,pass1);
  wifiMulti.addAP(ssid2,pass2);
  //  WiFi ESTADO
  WiFi.mode(WIFI_STA);
  Serial.print("Conetando a WiFi..");
  while (wifiMulti.run(TiempoEsperaWifi) != WL_CONNECTED){
    Serial.print(".");
  }
  Serial.println("conectado...");
  Serial.print("SSID:");
  Serial.print(WiFi.SSID());
  Serial.print(" ID: ");
  Serial.println(WiFi.localIP());

//************dht11*********************************************///
  dht.begin(); 
///////////////rayos Uv////////
  Wire.begin();//sensor BH1750  // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
  lightMeter.begin(); //sensor BH1750

   //-inicializamos cliente google---------------------------------------
  client.setInsecure();
}

void loop() 
{
  
ActualizarWifi();
// Maintain WiFi conexion**************************************
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi con codigo:");   
  delay(5000);    
    }
    else {
   Serial.println("codigo sin  wifi... ");
  }
//******todo este codigo de loop apartir de arriba funciona muy bien 

  //////////////////////****HT11******////////////////
    float h = dht.readHumidity(); //Se lee la humedad y se asigna el valor a "h"
    float t = dht.readTemperature(); //Se lee la temperatura y se asigna el valor a "t"
 float l = lightMeter.readLightLevel();
// se realiza un caracter de datos para excel
  String Temp = "Temperature : " + String(t) + " °C";
  String Humi = "Humidity : " + String(h) + " %";
  String Lux = "Luxes : " + String(l) + " %";
 Serial.println(Temp);
  Serial.println(Humi);
   Serial.println(Lux);
 sendData(t, h, l); //--> Calls the sendData Subroutine*/



} //CIERRA LOOP  
void sendData(int tem, int hum, int lux) {

  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //---------------------------------------- Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------Proceso  de datos
  String string_temperature =  String(tem);
  // String string_temperature =  
  String(tem, DEC); 
  String string_humidity =  String(hum, DEC);
 // luces  
  String string_luxes =  String(lux, DEC);
  
  // URL a enviar para canal en ThingSpeak
  String url = " /update?api_key="+ API +"&field1="+ string_temperature +"&field2="+ string_humidity + "&field3=" string_luxes;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------
  //----------------------------------------en caso de falla de conexion
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
//   if (line.startsWith("{\"state\":\"success\""))
  if (line.startsWith("\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
//delay(5000);
} 

void ActualizarWifi() 
{
 if(wifiMulti.run(TiempoEsperaWifi) != WL_CONNECTED){
    Serial.println("sin conexion wifi "); 
      Serial.println(".. reintentando conectar a excel ");   
 }
 }
  


#include <ESP8266WiFi.h>
#include <Servo.h>
#include <PubSubClient.h>


Servo motor;



#define MAX_INTENTOS 50
#define WIFI_SSID "robotica"
#define WIFI_PASS "pass"
#define MQTT_HOST IPAddress(52, 13, 116, 147)
#define MQTT_PORT  1883
//servo
#define SERVOPIN 0
Servo servo;
#define MQTT_PUB_SERVO "wemos/robotica/servo"
#define MQTT_NOME_CLIENTE "Cliente servo"

  WiFiClient espClient;
  PubSubClient mqttClient(espClient);
 #define LED 14

 int tiempo =  500;
 bool conectado = false;

 
void setup(){
Serial.begin(115200);
pinMode(LED,OUTPUT);
servo.attach(SERVOPIN);
conectarWifi();
conectado=conectarWifi();
mqttClient.setServer(MQTT_HOST, MQTT_PORT);
mqttClient.setCallback(callback);
}
void loop (){
  if(conectado)escintila(tiempo);
  else escintila(tiempo/10);
}
void escintila (int espera){
  digitalWrite(LED,HIGH);
  delay(espera);
  digitalWrite(LED,LOW);
  delay(espera);
}

bool conectarWifi(){
WiFi.mode(WIFI_STA);
WiFi.disconnect();
WiFi.begin(WIFI_SSID, WIFI_PASS);
Serial.print("\n\nAgardando polaWifi");
int contador = 0;
while (WiFi.status()!=WL_CONNECTED and contador < MAX_INTENTOS){
contador ++;
delay(500);
Serial.print(".");
}
Serial.println("");
if (contador<MAX_INTENTOS){
Serial.print("Conectado a wifi coa IP: "); Serial.println(WiFi.localIP());
conectado = true;
  
}

else {
Serial.println("Non se puido conectar a wifi");
conectado = false;
}
return(conectado);
}
void callback (String topic, byte* message, unsigned int len){
Serial.print("Nova mensaxe no topic:"); Serial.print (topic);
Serial.print (".Mensaxe: ");
String mensaxeTmp = "";
for (int i=0; i<len; i++){
  Serial.print ((char)message[i]);
  mensaxeTmp += (char)message[i];
}
Serial.println();
accionarServo(mensaxeTmp);
}
void reconnect() {
  while(!espClient.connected()) {
  Serial.print("Tentando conectar ao servidor MQTT... ");  

  if (mqttClient.connect(MQTT_NOME_CLIENTE )) {
    Serial.println(" Conectado");
    mqttClient.subscribe(MQTT_PUB_SERVO); 
  }
  else {
    Serial.print("Fallo ao conectar ao servidor mqtt, rc=");
    Serial.print (mqttClient.state());
    Serial.println("nova tentativa en 5 s");
    delay (5000);
  }
}

}
void accionarServo(String orde){
  int posicion;
  if (Serial.available()) {
orde= Serial.readStringUntil('\n');
orde.toLowerCase();
if (orde.equals("esquerda")) posicion = 180;
else if(orde.equals("dereita")) posicion = 0;
else if(orde.equals("centro")) posicion = 90;
else {
int tmp = orde.toInt();
if(tmp >= 0 && tmp <= 180) posicion = tmp;
else posicion = 0;
}
  }
motor.write(posicion);
delay(tiempo);
  
}


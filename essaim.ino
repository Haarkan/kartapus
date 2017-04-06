#define BLYNK_PRINT Serial
#include <Wire.h> // Déclaration de la bibliothèque moteurs arduino
#include <Servo.h>
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <BlynkSimpleWiFiShield101.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//---------------------PARTIE ESSAIM 
//-----DECLARATIONS
int status = WL_IDLE_STATUS;
char ssidUDP[] = "iPhone de Quentin";
char passUDP[] = "quinou84";
int keyIndex = 0; // Index de la clé réseau

unsigned int localPort = 2390; // On écoute sur ce port


int pret = 0;

char packetBuffer[255];
char ReplyBuffer[] = "89";

IPAddress ip(255,255,255,255); 
IPAddress ipLeader;

WiFiUDP Udp;




//---------------------PARTIE PILOTAGE
char auth[] = "56ae31f68d6041f5a45bd363f37ef9ab";
char ssid[] = "iPhone de Quentin"; //  your network SSID (name)
char pass[] = "quinou84";   // your network password (use for WPA, or use as key for WEP)

int valDistanceAvant;
int valDistanceGauche;
int valDistanceArriere;
int valDistanceDroit;
const int servo1Pin = 4; //droite
const int servo2Pin = 6;
int sensorPin = A0;
Servo servo1;
Servo servo2;
int stop = 0;
int sensorVal = 0;
int avant = 0;
int droite = 0;
int gauche = 0;
int arriere = 0;
bool isLeader = true;
int sensorPinDevant = A0;
int sensorPinGauche = A1;
int sensorPinDroite = A2;
int sensorPinArriere = A3;

Adafruit_BNO055 bno = Adafruit_BNO055(55, BNO055_ADDRESS_B);

void arret()
{
  servo1.write(92);
  servo2.write(90);
}

void avancer()
{
  servo1.write(0);
  servo2.write(180);
}

void reculer()
{
  servo1.write(180);
  servo2.write(0);
}

void tourner_gauche()
{
  servo1.write(52);
  servo2.write(50);
}

void tourner_droite()
{
  servo1.write(132);
  servo2.write(130);
}

void tourner_droite_lent()
{
  servo1.write(90);
  servo2.write(90);
}

void print_orientation()
{
  imu::Vector<3> test = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  Serial.println(atan2(test[0], test[1]));
}

void send_pos_leader() {
      
      imu::Vector<3> test = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
      if (atan2(test[0], test[1]) != 0) {
        int valueatan = atan2(test[0], test[1])*100;
        Serial.println(valueatan);
        Udp.beginPacket("255.255.255.255", 2390);
        String loc = String(valueatan);
        Serial.println(loc);
        char charBuf[50];
        loc.toCharArray(charBuf, 50);
        Udp.write(charBuf);
        Serial.println(charBuf);
        Udp.endPacket();
      }
      
}

void alignement_leader(double DegreRotation) {

  imu::Vector<3> test = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  if(atan2(test[0], test[1]) < DegreRotation-0.1 || atan2(test[0], test[1]) > DegreRotation+0.07){  
    tourner_droite_lent();
  } else{
    arret();
  }
}

void checkDistance (int pin) {
    sensorVal = analogRead(pin);
    if (sensorVal >= 800)
    {
      arret();
      Serial.println("arret");
    }
}

void checkLeader(String data){   
    if (data == "leader") {
      isLeader = false;
    }

    if (isLeader) {
      digitalWrite(LED_BUILTIN, LOW);
    } 
    else {
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
    }
  
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

//Bouton d'arret d'urgence
BLYNK_WRITE(V2) {
  stop = param.asInt();
  if (stop == 1)
  {
    arret();
  }

}

//avancer
BLYNK_WRITE(V3) {
  avant = param.asInt();
  if (avant == 1)
  {
    avancer();
  }
  else
  {
    arret();
  }
}
//tourner a gauche
BLYNK_WRITE(V4) {
  gauche = param.asInt();
  if (gauche == 1)
  {
    tourner_gauche(); 
  }
  else
  {
    arret();
  }
}
//reculer
BLYNK_WRITE(V5) {
  arriere = param.asInt();
  if (arriere == 1)
  {
    reculer(); 
  }
  else
  {
    arret();
  }
}
//tourner a droite
BLYNK_WRITE(V6) {
  droite = param.asInt();
  if (droite == 1)
  {
    tourner_droite(); 
  }
  else
  {
    arret(); 
  }
}

void setup()
{
  Serial.begin(9600); // initialiser le moniteur série à 115200 bps

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  Blynk.begin(auth, ssid, pass);

  //Initialisation du BNO
  Serial.println("Orientation Sensor Test"); Serial.println("");
  if(!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  char statut[] = "leader";
  Udp.beginPacket("255.255.255.255", 2390);
  Udp.write(statut);
  Udp.endPacket();
  pinMode(PIN_LED_13, OUTPUT);
}

void loop()
{

  Blynk.run();
    checkDistance(sensorPinDevant);
    checkDistance(sensorPinArriere);
    checkDistance(sensorPinGauche);
    checkDistance(sensorPinDroite);
  if(stop == 0)
  {
    int packetSize = Udp.parsePacket();
        if (packetSize)
        {
          int len = Udp.read(packetBuffer, 255);
          Serial.println(packetBuffer);
          if (len > 0) packetBuffer[len] = 0;
          Serial.print(", data: ");
          Serial.println(packetBuffer);
          String data = packetBuffer;
          checkLeader(data);
        }

    if (isLeader) 
      {
        send_pos_leader();
      }
      else 
      {
        int pos = atoi(packetBuffer);
        float realPos = pos;
        realPos /= 100;
        Serial.println(pos);
        Serial.println(realPos);
        alignement_leader(realPos);
      }
      
  
    
    
    delay(100);

  }
}

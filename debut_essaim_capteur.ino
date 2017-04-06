#define BLYNK_PRINT Serial
#include <Wire.h> // Déclaration de la bibliothèque moteurs arduino
#include <Servo.h>
#include <SPI.h>
#include <WiFi101.h>
#include <BlynkSimpleWiFiShield101.h>

char auth[] = "86c5e183f8b747d28d4a6dd69a449339";
char ssid[] = "OnePlus3";
char pass[] = "";

int valDistanceAvant;
int valDistanceGauche;
int valDistanceArriere;
int valDistanceDroit;
const int servo1Pin = 4; //droite
const int servo2Pin = 6;
int sensorPin = A0;
Servo servo1;
Servo servo2;
int demarre = 0;
int stop = 0;
int sensorVal = 0;
int avant = 0;
int droite = 0;
int gauche = 0;
int arriere = 0;

bool allume = 1;

//Bouton d'arret d'urgence
BLYNK_WRITE(V2) {
  stop = param.asInt();
  if (stop == 1)
  {
    servo2.write(95);
    servo1.write(95); 
  }
}

BLYNK_WRITE(V1) {
  demarre = param.asInt();
  if (demarre == 1)
  {
    allume = true;
  }
}


//avancer
BLYNK_WRITE(V3) {
  avant = param.asInt();
  if (avant == 1 && allume == true)
  {
    servo2.write(180);
    servo1.write(0); 
  }
  else
  {
    servo2.write(95);
    servo1.write(95); 
    Serial.print('stop');
  }
}
//tourner a gauche
BLYNK_WRITE(V4) {
  gauche = param.asInt();
  if (gauche == 1 && allume == 1)
  {
    servo2.write(130);
    servo1.write(132); 
  }
  else
  {
    servo2.write(95);
    servo1.write(95); 
  }
}
//reculer
BLYNK_WRITE(V5) {
  arriere = param.asInt();
  if (arriere == 1 && allume == 1)
  {
    servo2.write(0);
    servo1.write(180); 
  }
  else
  {
    servo2.write(95);
    servo1.write(95); 
  }
}
//tourner a droite
BLYNK_WRITE(V6) {
  droite = param.asInt();
  if (droite == 1 && allume == 1)
  {
    servo2.write(50);
    servo1.write(52); 
  }
  else
  {
    servo2.write(95);
    servo1.write(95); 
  }
}

void verifierDistance(){
  //200 car notre capteur arriere est different (plus petit)
    if (valDistanceAvant > 600 || valDistanceArriere < 200 || valDistanceGauche > 600 || valDistanceDroit > 600) {  //distance danger
      digitalWrite(PIN_LED_13, LOW);   // turn the LED on (HIGH is the voltage level)
      servo2.write(95);
      servo1.write(95);
      allume = false;
    }
  else{
    digitalWrite(PIN_LED_13, HIGH);    // turn the LED off by making the voltage LOW
  }
}

void setup()
{
  Serial.begin(9600); // initialiser le moniteur série à 115200 bps
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  valDistanceAvant = analogRead(A1);
  valDistanceDroit = analogRead(A2);
  valDistanceGauche = analogRead(A0);
  valDistanceArriere = analogRead(A3);
  verifierDistance();
  
  Serial.print("arriere ");
  Serial.println(valDistanceArriere); 
  Serial.print("allume ");
  Serial.println(allume); 
  
  delay(100);
}

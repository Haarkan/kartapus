#include <Servo.h>

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <WiFi101.h>
#include <BlynkSimpleWiFiShield101.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "fc592aad069f4a3eabc2bb6919dee533";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OnePlus3";
char pass[] = "";

Servo monServoDroit;  // create servo object to control a servo
Servo monServoGauche; 
// twelve servo objects can be created on most boards

int valDistance;

bool ligne = false;

int valLum =0;

int vitMax = 0;
int vitMaxDroit = 180;
int vitArret = 90;
int vitInter = 63;
int vitInterDroit = 135;

bool allume = true;

BLYNK_WRITE(V1){
  //probleme
  if (param.asInt() == 1) {
     monServoGauche.write(vitArret);
     monServoDroit.write(vitArret);
     Sdelay(10000);
  } else {   
    monServoGauche.write(vitInter);
    monServoDroit.write(vitInterDroit); 
  } 
}
void verifierDistance(){
    if (valDistance > 600) {  //distance danger
      digitalWrite(PIN_LED_13, LOW);   // turn the LED on (HIGH is the voltage level)
      monServoGauche.write(vitArret);
      monServoDroit.write(vitArret);
      allume = false;
  }
  else{
    digitalWrite(PIN_LED_13, HIGH);    // turn the LED off by making the voltage LOW
  }
}
void setup() {
  monServoDroit.attach(4,1,5);  // attaches the servo on pin 4 to the servo object
  monServoGauche.attach(6,15,15);  // attaches the servo on pin 6 to the servo object
  pinMode(LED_BUILTIN, OUTPUT);
  monServoGauche.write(vitInter);
  monServoDroit.write(vitInterDroit);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  digitalWrite(LED_BUILTIN, HIGH);
  
}

void loop() {
  Blynk.run();
  valDistance = analogRead(A1);
  verifierDistance();

  valLum = analogRead(A0);
  Serial.println(valLum    );

  int distance = 10;
  //se decaler a doite de distance - x

  verifierDistance(); 
  //robot detecte ligne 
  if (valLum >= 350) {
    ligne = false;
  }
  //tant que le robot ne detecte pas la ligne
  while(!ligne) {
    verifierDistance();
    if(allume){
      valLum = analogRead(A0);
      for (int i =0; i < distance; ++i) {
        //decaler d'un cran a gauche
        monServoGauche.write(vitArret);
        delay(distance);
        monServoGauche.write(vitInter);
        
        valLum = analogRead(A0);
        if (valLum < 350){
          //decaler un peu a gauche
            monServoGauche.write(vitArret);
            delay(10);
            monServoGauche.write(vitInter);
          ligne = true;
          break;
        }
      }
      if (ligne) break;
      //augmenter distance
      distance = distance + 10;
      for (int i =0; i < distance; ++i) {
        //decaler d'un cran a droite
        monServoDroit.write(vitArret);
        delay(distance);
        monServoDroit.write(vitInterDroit);
        
        valLum = analogRead(A0);
        if (valLum < 350){
          //decaler un peu a droite
          monServoDroit.write(vitArret);
          delay(10);
          monServoDroit.write(vitInterDroit);
          ligne = true;
          break;
        }
      }
      distance = distance + 10;
    }
  }
  
}

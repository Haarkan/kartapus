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

int vitMax = 90;
int vitInter = 45;
int vitMaxDroit = vitMax + 90;
int vitInterDroit = vitInter + 90;

BLYNK_WRITE(V1){
  //probleme
  int i = param.asInt();
  if (i==1) {
     digitalWrite(LED_BUILTIN, HIGH);
     vitInter = 0;
     vitInterDroit = 0;
     monServoGauche.write(vitInter);
     monServoDroit.write(vitInterDroit);
  } else {    
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void setup() {
  monServoDroit.attach(6,1,5);  // attaches the servo on pin 4 to the servo object
  monServoGauche.attach(4,15,15);  // attaches the servo on pin 6 to the servo object
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
    Blynk.run();
    monServoGauche.write(vitInter);
    monServoDroit.write(vitInterDroit);
}


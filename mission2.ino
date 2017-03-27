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

int vitMax = 0;
int vitMaxDroit = 180;
int vitArret = 90;
int vitInter = 63;
int vitInterDroit = 135;

BLYNK_WRITE(V1){
  //probleme
  if (param.asInt() == 1) {
     monServoGauche.write(vitArret);
     monServoDroit.write(vitArret);
  } else {    
    monServoGauche.write(vitInter);
    monServoDroit.write(vitInterDroit);
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
  digitalWrite(LED_BUILTIN, LOW);
  
}

void loop() {
  Blynk.run();
  valDistance = analogRead(A1);
  Serial.println(valDistance);
  if (valDistance > 600) {  //distance danger
    digitalWrite(PIN_LED_13, HIGH);   // turn the LED on (HIGH is the voltage level)
    /* arret total
    monServoGauche.write(vitArret);
    monServoDroit.write(vitArret);*/

    monServoGauche.write(vitArret);
    delay(1000);
    monServoGauche.write(vitInter);
  }
  else{
    digitalWrite(PIN_LED_13, LOW);    // turn the LED off by making the voltage LOW
  }
}





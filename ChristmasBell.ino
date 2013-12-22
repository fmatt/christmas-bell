/**
 *    IR Remote controlled Christmas Bell
 *
 *    (c) 22.12.2013 Matthias Fölser <matthias.foelser@gmail.com>
 */

#include <IRremote.h>
#include <Servo.h>

int irPin = 11;
int bellPin = 9;
int ledPin = 13; // for status information

 // IR Code to listen for
int irCode = 0x2A2CA019;

long ringTimeout = 100; // Timeout between Button presses

int minPos = 10;  // initial servo position (degree)
int maxPos = 25; // maximum servo throw (degree)
int stepDelay = 5; // ms between servo steps
int numRings = 4; // number of rings per button press


Servo bell;
IRrecv irRecv(irPin);
decode_results results;
long lastRing = 0;
int pos = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin, LOW);

  irRecv.enableIRIn();

  bell.attach(bellPin);
  bell.write(minPos);

  delay(50);
}

void ring(){
  
  Serial.print("ring ring...");
  digitalWrite(ledPin, HIGH);

  for(int ring =0;ring < numRings;ring++){
    
    for(pos = minPos; pos < maxPos; pos += 1){                                  
      bell.write(pos);            
      delay(stepDelay);
    } 
    for(pos = maxPos; pos > minPos; pos-=1){
      bell.write(pos);
      delay(stepDelay);
    } 
    
  }
    
  digitalWrite(ledPin, LOW);
  Serial.println("!");
  
}


void loop() {
  if (irRecv.decode(&results)) {
    
    if(results.value == irCode){
      if((lastRing+ringTimeout) < millis()){
        ring();
        lastRing = millis();
      }
    }
      
    irRecv.resume();
  }
  delay(10);
}

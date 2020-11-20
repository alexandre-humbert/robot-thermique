#include <Wire.h>
#include <TimerOne.h>
extern "C"
{
#include <motor.h>
#include <coder.h>
}
#include <control.h>
#include <interface.h>

//#define DEBUG 0  // commenter pour une machine de prod
#define DEFAULT_SPEED_PARAM 50  // in RPM
#define DEFAULT_RADIUS 44  // in cm (2 * inter-wheel )
#define LED_PIN 13  // Teensy 3.0 has the LED on pin 13

int dObstacle = 10;

// initialisation arduino
void setup()
{
  setup_coder_pin() ;
  setup_motor_pin();
  setup_control() ;
  setup_interface(DEFAULT_SPEED_PARAM,DEFAULT_RADIUS) ;
  Serial1.begin(9600); 
  delay( 100);
#ifdef DEBUG  // debug avec la console
  Serial1.println( "init...");
#endif
  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN, LOW);    // set the LED off
}

void loop()
{ // wait for serial line
  if ( Serial1.available() > 0)
  {
  String sRet;
  digitalWrite(LED_PIN, HIGH);    // set the LED on
  Serial1.setTimeout( 10000);  // pour terminer readString
    sRet = Serial1.readStringUntil(0x0a);
#ifdef DEBUG
  Serial1.print("string =");   Serial1.println( sRet);
    // atest la bonne reception
#endif
    if (sRet.indexOf( "al")>=0) digitalWrite(LED_PIN, HIGH);    // set the LED on
    else if (sRet.indexOf( "et")>=0)  digitalWrite(LED_PIN, LOW);    // set the LED off
    else 
    inter_cmde(sRet) ;
  digitalWrite(LED_PIN, LOW);    // set the LED off
  }
}

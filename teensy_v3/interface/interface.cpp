#include <interface.h>
#include <control.h>

#define INTER_WHEEL 22.

float SpeedParam ;	// in RPM
float RadiusParam ; // in cm
float AngleParam ;  // in degree
float DistanceParam ; // in cm

void setup_interface(float speed, float radius)
{
  SpeedParam = speed ;
  RadiusParam = radius ;
}

void inter_cmde(String str)
{
  int i, j ;
  int param1, param2 ;
  int nbparam ;
  String cmde ;
  
  float speed_int ;
  float speed_ext ;
 
  speed_int = RadiusParam / (RadiusParam + INTER_WHEEL /2) ;
  speed_ext = (RadiusParam + INTER_WHEEL) / (RadiusParam + INTER_WHEEL /2) ;
 
  cmde = str.substring(0,2) ;  // speed up string search and avoid syntax error
	   
   param1 = 0 ; param2 = 0 ; nbparam = 0 ;
   i = str.indexOf(" ") ; j = str.indexOf(" ",i+1);
   if (i >= 2) nbparam++ ;
   if ((j > i+1)&&((int)str.length()>j)) nbparam++ ; 
   
   if (nbparam==1) param1 = str.substring(i+1,str.length()).toFloat() ;
   if (nbparam==2) 
   {
	   param1 = str.substring(i+1,j).toFloat() ;
	   param2 = str.substring(j+1,str.length()).toFloat() ;
   }
#define DEBUG
 #ifdef DEBUG
  Serial.print("command = ") ; Serial.print(cmde) ; Serial.print(i) ;
  Serial.print("  param1 = ") ; Serial.print(param1);
  Serial.print("  param2 = ") ; Serial.println(param2);
  Serial.print("  nbparam = ") ; Serial.println(nbparam);
#endif
// Commands for compatibility with old version  
  if (nbparam==0) {
	if (cmde.equals("av")) consigne_v1_v2(SpeedParam,SpeedParam) ;
	else if (cmde.equals("ar")) consigne_v1_v2(-SpeedParam,-SpeedParam) ;
	else if (cmde.equals("st")) consigne_v1_v2(0,0) ;
	else if (cmde.equals("dr")) consigne_v1_v2(SpeedParam,SpeedParam/2) ;
	else if (cmde.equals("ga")) consigne_v1_v2(SpeedParam/2,SpeedParam) ;
	else if (cmde.equals("ad")) consigne_v1_v2(-SpeedParam,-SpeedParam/2) ;
	else if (cmde.equals("ag")) consigne_v1_v2(-SpeedParam/2,-SpeedParam) ;
	else if (cmde.equals("tg")) consigne_v1_v2(-SpeedParam/2,0) ; //TEST : tourne sur lui même vers la gauche (moteur inversé)
	else if (cmde.equals("td")) consigne_v1_v2(0,-SpeedParam/2) ; //TEST : tourne sur lui même vers la droite (moteur inversé)
	  
  }

 // New commands
  if (nbparam==0) {
	if (cmde.equals("fw")) consigne_v1_v2(SpeedParam,SpeedParam) ;
	else if (cmde.equals("bw")) consigne_v1_v2(-SpeedParam,-SpeedParam) ;
	else if (cmde.equals("fr")) consigne_v1_v2(speed_ext*SpeedParam,speed_int*SpeedParam) ;
	else if (cmde.equals("fl")) consigne_v1_v2(speed_int*SpeedParam,speed_ext*SpeedParam) ;
	else if (cmde.equals("br")) consigne_v1_v2(-speed_ext*SpeedParam,-speed_int*SpeedParam) ;
	else if (cmde.equals("bl")) consigne_v1_v2(-speed_int*SpeedParam,-speed_ext*SpeedParam) ;
  }
  
  
// Definition of current parameters
  if (nbparam==1) {
	if (cmde.equals( "sp")) SpeedParam = param1 ;
	else if (cmde.equals("ra")) RadiusParam = param1 ;
	else if (cmde.equals("an")) AngleParam = param1 ;
	else if (cmde.equals("di")) DistanceParam = param1 ;
  }  
  
// Commands with one parameter
  if (nbparam==1) {
	if (cmde.equals("fw")) consigne_v1_v2(param1,param1) ;
	else if (cmde.equals("bw")) consigne_v1_v2(-param1,-param1) ;
	else if (cmde.equals("fr")) consigne_v1_v2(speed_ext*param1,speed_int*param1) ;
	else if (cmde.equals("fl")) consigne_v1_v2(speed_int*param1,speed_ext*param1) ;
 	else if (cmde.equals("br")) consigne_v1_v2(-speed_ext*param1,-speed_int*param1) ;
	else if (cmde.equals("bl")) consigne_v1_v2(-speed_int*param1,-speed_ext*param1) ;
  }  

  //CurrentSpeedParam = param1 ;
  
}


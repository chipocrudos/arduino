#include "pitches.h"
// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };
// Pin buzzer
const int BZZ = 4;
//Los pines 0 y 1 Digital se conectan directos al bluetooth
//const int BPW = 3; //PWM reinicio de bluetooth
//Motor A - Avanzar y retroceder
const int ENA = 11; //PWM
const int IN1 = 12;
const int IN2 = 13;
//Motor B - Derecha e Izquierda 
const int ENB = 6; //PWM
const int IN3 = 7;
const int IN4 = 8;
//Luces
const int LTR = 9; //Luz trasera PWM
const int LDR = 10; //Luz delantera PWM Der
const int LDL = 5; //Luz delantera PWM Izq

//movimientos
const char Forward = 'F';
const char Back = 'B';
const char Left = 'L';
const char Right = 'R';
const char ForwardLeft = 'G';
const char ForwardRight = 'I';
const char BackLeft = 'H';
const char BackRight = 'J';
const char Stop = 'S';
const char FrontLightsOn = 'W';
const char FrontLightsOff = 'w';
const char BackLightsOn = 'U';
const char BacktLightsOff = 'u';
const char HornOn = 'V';
const char HornOff = 'v';
const char ExtraOn = 'X';
const char ExtraOff = 'x';
const char StopAll = 'D';


int PWA = 0; //Velocidad Motor A
int PWB = 255; //Velocidad Motor A
int minLuces = 0;
boolean cuartos = false;
boolean altas = false;
boolean extra = false;
boolean centro = true;
boolean der = false;
boolean izq = false;
boolean claxon = false;

boolean adelan = true;
char comando = 'z';

void setup()
{
  Serial.begin(9600);

  pinMode(BPW, OUTPUT);
  //Motor A
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  //Motar B
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);
  //Luces
  pinMode(LDR, OUTPUT); //Luz delantera
  pinMode(LDL, OUTPUT); //Luz delantera
  pinMode(LTR, OUTPUT); //Luz trasera
  
  //Inicializando pines
  //Bluetooth
  delay(5000);
  digitalWrite(BPW, HIGH);  
  //Motor A
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  //Motar B
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  //Luces
  digitalWrite (LDR, LOW); //Luz delantera
  digitalWrite (LDL, LOW); //Luz delantera
  digitalWrite(LTR, LOW); //Luz trasera
  //anunciar inicio
  toneMelody();
}

void loop()
{
      lecturaSerial(); //Recibir dato bluetooth    
      if (comando == Forward)
      {
        centrar();
        avanzar();
      }
      else if (comando == Back)
      {
        centrar();
        retroceder();
      }
      else if (comando == Right)
      {
        derecha();
      }
      else if (comando == Left)
      {
        izquierda();
      }
      else if (comando == ForwardLeft)
      {
        avanzar();
        izquierda();
      }
      else if (comando == ForwardRight)
      {
        avanzar();
        derecha();
      }
      else if (comando == BackLeft)
      {
        retroceder();
        izquierda();
      }
      else if (comando == BackRight)
      {
        retroceder();
        derecha();
      }
      else if (comando == StopAll || comando == Stop)
      {
        altoTotal();
      }else if (comando == '0') {
        PWA = 0;
      }
      else if (comando == '1') {
        PWA = 60;
      }
      else if (comando == '2') {
        PWA = 80;
      }
      else if (comando == '3') {
        PWA = 100;
      }
      else if (comando == '4') {
        PWA = 120;
      }
      else if (comando == '5') {
        PWA = 130;
      }
      else if (comando == '6') {
        PWA = 155;
      }
      else if (comando == '7') {
        PWA = 175;
      }
      else if (comando == '8') {
        PWA = 200;
      }
      else if (comando == '9') {
        PWA = 225;
      }
      else if (comando == 'q') {
        PWA = 255;
      }
      else if (comando == FrontLightsOn) {
        altas = true;
        estadoAltas();
      }
      else if (comando == FrontLightsOff) {
        altas = false;
        estadoAltas();
      }      
      else if (comando == BackLightsOn) {
        cuartos = true;
        estadoCuartos();
      }
      else if (comando == BacktLightsOff) {
        cuartos = false;
        estadoCuartos();
      }
      else if (comando == HornOn) {
         toneMelody();
         claxon = true;
      }
      else if (comando == HornOff) {
         claxon = false;
      }
      else if (comando == ExtraOn) {
         extra = true;
      }
      else if (comando == ExtraOff) {
         extra = false;
      }
      luzEstrobo();

}

void lecturaSerial()
{
  comando = 'z';
  while(Serial.available())
  {//while there is data available on the serial monitor
    comando= char(Serial.read());//store string from serial command
  }
  
}


void altoTotal() // Detiene y centra la direccion
{
  //Detener Motor A
  traccion();
  analogWrite(ENA, 0);
  //Detener Motor b
  centrar();
  analogWrite(LTR, 255);
}

void centrar() //Centra direccion
{
  if (!centro) {
    //Detener motor B
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, LOW);
    analogWrite(ENB, 0);
    delay(100);
  }
  centro = true;
  der = false;
  izq = false;

}

void derecha() // Giro a la derecha
{
  if (!centro && !der)
  {
    centrar();
  }
  if (!der)
  {
    //Activar Motor B
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    analogWrite(ENB, PWB);
    der = true;
    centro = false;
  }
}

void izquierda() // Giro a la izq
{
  if (!centro && !izq)
  {
    centrar();
  }
  if (!izq) {
    //Activar Motor B
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    analogWrite(ENB, PWB);
    izq = true;
    centro = false;
  }

}

void traccion() //Centra detiene Motor A
{
  //Detener Motor A
  digitalWrite (IN1, LOW);
  digitalWrite (IN2, LOW);
  analogWrite (ENA, 0);
  delay(100);
  if (claxon)
  {
    toneMelody();
  }
}


void avanzar()
{
    if (!adelan) {
       traccion(); //Detener Motor A    
    }
    estadoCuartos();
    //Activar motor A
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    analogWrite (ENA, PWA);
    adelan = true;

}

void retroceder(){
    if (adelan) {
       traccion(); //Detener Motor A    
    }
    estadoCuartos();
    //Activar Motor A
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    analogWrite (ENA, 70);
    adelan = false;
    
}


void estadoCuartos()
{
  if (cuartos)
  {
    minLuces = 100;
  }
  else
  {
    minLuces = 0;
  }
  analogWrite(LTR, minLuces);
  estadoAltas();
  
}

void estadoAltas()
{
  if (altas)
  {
    analogWrite(LDR, 255);
    analogWrite(LDL, 255);
  }
  else
  {
    analogWrite(LDR, minLuces);
    analogWrite(LDL, minLuces);
  }
}

void luzEstrobo(){
   int tiempo = 30;
  if (extra){
    digitalWrite(LDR, HIGH);
    delay(tiempo);
    digitalWrite(LDR, LOW);
    delay(tiempo);
    digitalWrite(LDR, HIGH);
    delay(tiempo);
    digitalWrite(LDR, LOW);
    delay(80);

    digitalWrite(LDL, HIGH);
    delay(tiempo);
    digitalWrite(LDL, LOW);
    delay(tiempo);
    digitalWrite(LDL, HIGH);
    delay(tiempo);
    digitalWrite(LDL, LOW);
    delay(80);

    estadoAltas();
  }
  else
  {
    delay(100);
  }
}


void toneMelody()
{
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(BZZ, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BZZ);
  }
}

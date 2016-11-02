#include "Timer.h"
#include <SoftwareSerial.h>
Timer t;

int FSR_Pin = A0; ////Entrada asignada del ADC para el FSR
int VOLTAPin = A1; ////Entrada asignada del ADC para el voltaje
int SECONDS_BOOT_DELAY = 10; ////delay de inicio para esperar que el módulo GSM se conecte a la RED

float lectura = 0; //100 para voltios, 0.1 para milivoltios
float escala = 100; //100 para voltios, 0.1 para milivoltios
unsigned long IMEI = 86071902;
unsigned long IMEI2 = 2136042;
//char IMEI[255] = 860719022136042;
volatile int lecturaFSR=0; // Variable de la lectura ADC del FSR
volatile float peso=0; // Variable de conversion a peso
volatile float pesomin=0; // Variable de salida de ciclo de peso
volatile float voltaje=0; // Variable de conversion voltaje real
int LEDpin = 11;      // LED to pin 11 (PWM pin)
int LEDbrillo;
char  bateria[255]="\0"; ;
// Eliminador de rebote por software
volatile long lastInt = 0;

SoftwareSerial sim800l(7,8); //RX - TX  
bool res_at(char *buf,char *patern){ 
  char *ptr;

   ptr = strstr(buf, patern);
   if (ptr !=0){
   return true;
   }else{
   return false;
   }
}

char *debug()  // devuelve el ``contenido de un objeto apuntado por un apuntador''. 
{
int i=0;
char cad[255]="\0";
char c='\0';
        
        strcpy(cad,"");
        while(sim800l.available()>0)
        {
        c=sim800l.read();
        cad[i]=c;
        bateria[i]=c;
        i++;
        }
      
return cad;
}


char *debug_bat()  // devuelve el ``contenido de un objeto apuntado por un apuntador''. 
{
int i=0;
char cad[255]="\0";
//char bateria[255]="\0";
char c='\0';
        
        strcpy(bateria,"");
        while(sim800l.available()>0)
        {
        c=sim800l.read();
//        cad[i]=c;
        bateria[i]=c;
        i++;
        }
      
return bateria;
}


void setup() {
 pinMode(LEDpin, OUTPUT);
Serial.begin(19200);  //Configuracion de puerto serial por hardware en bps(baudios por segundo)
sim800l.begin(19200); //Configuracion de puerto serial del modulo (baudios por segundo)
int tickEvent = t.every(1000, sendData);
Serial.print("ID envio=");
Serial.println(tickEvent);
Serial.println("Iniciando configuracion del modulo GSM");
delay(SECONDS_BOOT_DELAY * 1000);
sim800l.println(F("AT"));
delay(500);
Serial.println(debug());
sim800l.println(F("AT+CBC"));  //Retorna el estado de la bateria del dispositivo, el % y milivol
delay(500);
//*bateria[255]=debug_bat();
Serial.println(debug());

sim800l.println(F("AT+CSQ")); // Retorna la calidad de la señal que depende de la antena y la localizacion
delay(500);
Serial.println(debug());
// Inicializamos la ultima interrupcion para evitar que se activen al encender
lastInt = millis();
// Asignamos las interrupciones 0 y 1
pinMode(3,INPUT_PULLUP); // Maneja el apagado del modulo GSM
attachInterrupt(0, leerdatos, FALLING);
// pinMode(2,INPUT_PULLUP); // Maneja la conexion al GPRS y envio de datos
// attachInterrupt(1, pin2OnFalling, FALLING);
configuracionGPRS();
Serial.print("Configuracion finalizada.\r\n");
//enviardatos();

}

void leerdatos() {
  if((millis()-lastInt)>500) {
 peso= averageAnalogRead(FSR_Pin);
// f(x) = p1*x^3 + p2*x^2 + p3*x + p4
//Coefficients (with 95% confidence bounds):
//       p1 =   1.149e-05  (9.354e-06, 1.362e-05)
//       p2 =   -0.007885  (-0.01041, -0.005358)
//       p3 =       2.233  (1.409, 3.057)
//       p4 =      -11.64  (-74.04, 50.77)
 voltaje=averageAnalogRead(VOLTAPin);
 lectura = map(voltaje, 0, 1023, 0, 500);
 voltaje = lectura / escala;
 
  //Actualiza los datos del peso al iniciar la interupcion 
 //peso=sqrt(lecturaFSR)*5-37;
 pesomin=peso;
//sim800l.println(F("AT+CBC"));
//char *bateria=debug_bat();

    lastInt = millis();
    
  }
}

void apagarModulo() {
  
sim800l.println(F("AT+CPOWD=1")); // Apaga el modulo GSM , es necesario esperar cuando se vuelva a prender
Serial.println(debug());
delay(500);
 
  
}

void enviardatos() {  //Se acitva si el peso se encuentra en un limite definido
//sim800l.println(F("AT+ CIICR")); // Levantar conexión wireless(GPRS o CSD)
//Serial.println(debug());
//delay(500);
  
sim800l.println(F("AT+CIFSR")); // Obtiene una dirección IP
Serial.println(debug());
delay(500); 
sim800l.println(F("AT+CIPSTART=\"UDP\",\"107.170.52.239\",8080")); //Inicia conexión UDP o TCP
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPSEND\r\n")); // Envia datos al servidor remoto, ctlr+z o 0x1A,
//verifica que los datos salieron del puerto serial pero no indica si llegaron al servidor UDP
Serial.println(debug());
delay(500);
sim800l.println("ID:"+String(IMEI)+String(IMEI2)+","+"peso:"+String(peso)+","+"voltaje:"+String(voltaje));
//sim800l.println(peso); // Se envia por un peticion GET los valores obtenidos
//sim800l.println(voltaje);

//sim800l.println(F("AT+CBC"));
//delay(500);
//debug_bat();
//Serial.println(debug());

pushSlow("\r\n",100,100); //Envia un salto de linea
pushSlow("\x1A",100,100);//ctlr+z para finalizar el envio o 0x1A
Serial.println(debug());
delay(500);
//sim800l.println(F("AT+CIPSHUT")); //Resetea las direcciones IP
//Serial.println(debug());

}

void configuracionGPRS() {
sim800l.println(F("AT+CREG=1")); // Verifica si la simcard a sido o no registrada
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPSHUT")); // Resetea las direcciones IP
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CGATT=1")); // Verifica si el gprs esta activo o no
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPSTATUS")); //Verifica si la pila o stack IP es inicializada
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPMUX=0")); //Esta la conexión en modo simple(udp/tcp cliente o tcp server)
Serial.println(debug());
delay(500);
// Configurar tarea y configura el APN
sim800l.println(F("AT+CSTT=\"internet.comcel.com.co\",\"COMCELWEB\",\"COMCELWEB\"")); // CAMBIAR APN
Serial.println(debug());
delay(500);

sim800l.println(F("AT+ CIICR")); // Levantar conexión wireless(GPRS o CSD)
Serial.println(debug());
delay(500);



}

void loop() {
//Monitoreo constantemente el peso y se actualiza al oprimir pulsador
//Serial.println(peso);
//LEDbrillo = map(peso, 0, 1023, 0, 255);//Ajusta el brillo segun lectura del sensor
//analogWrite(LEDpin, LEDbrillo);
t.update(); 
 
 //Serial.println(bateria);
//delay(2000);
// if (pesomin >=40 ) {      
//         enviardatos();
//         // apagarModulo();
//      } 
}

   // Envia datos por el SoftSerial
// lentamente
void pushSlow(char* command,int charaterDelay,int endLineDelay) {
  for(int i=0; i<strlen(command); i++) {
    sim800l.write(command[i]);
    if(command[i]=='\n') {
      delay(endLineDelay);
    } else {
      delay(charaterDelay);
    }
  }
}   
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 10;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}
//Funcion que se repite cada hora
void sendData()
{ 
  peso= averageAnalogRead(FSR_Pin);
  voltaje=averageAnalogRead(VOLTAPin);
 lectura = map(voltaje, 0, 1023, 0, 500);
 voltaje = lectura / escala;
  enviardatos();
}



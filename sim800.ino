
#include <SoftwareSerial.h>

int FSR_Pin = A0; ////Entrada asignada del ADC para el FSR
volatile int lecturaFSR=0; // Variable de la lectura ADC del FSR
volatile float peso=0; // Variable de conversion a peso
volatile float pesomin=0; // Variable de salida de ciclo de peso
volatile float  bateria = 0;
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
        i++;
        }
      
return cad;
}

void setup() {

Serial.begin(19200);  //Configuracion de puerto serial por hardware en bps(baudios por segundo)
sim800l.begin(19200); //Configuracion de puerto serial del modulo (baudios por segundo)

Serial.println("Iniciando configuracion del modulo GSM");

sim800l.println(F("AT"));
delay(500);
Serial.println(debug());
sim800l.println(F("AT+CBC"));  //Retorna el estado de la bateria del dispositivo, el % y milivol
delay(500);
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

}

void leerdatos() {
  if((millis()-lastInt)>500) {
 lecturaFSR = analogRead(FSR_Pin); //Actualiza los datos del peso al iniciar la interupcion 
 peso=sqrt(lecturaFSR)*5-37;
 pesomin=peso;
// sim800l.println(F("AT+CBC"));
//bateria=Serial.println(debug()).toInt();

    lastInt = millis();
    
  }
}

void apagarModulo() {
  
sim800l.println(F("AT+CPOWD=1")); // Apaga el modulo GSM , es necesario esperar cuando se vuelva a prender
Serial.println(debug());
delay(500);
 
  
}

void enviardatos() {  //Se acitva si el peso se encuentra en un limite definido
 
sim800l.println(F("AT+CIPSTART=\"TCP\",\"107.170.52.239\",80")); //Inicia conexión UDP o TCP
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPSEND\r\n")); // Envia datos al servidor remoto, ctlr+z o 0x1A,
//verifica que los datos salieron del puerto serial pero no indica si llegaron al servidor UDP
Serial.println(debug());
delay(500);
sim800l.println(F("GET /variables.php?volt=30&caffe=30")); // Se envia por un peticion GET los valores obtenidos
Serial.println(debug());
delay(500);
sim800l.println(F("\r\n")); //Envia un salto de linea
sim800l.println(F("\x1A"));//ctlr+z para finalizar el envio o 0x1A
Serial.println(debug());
delay(500);
sim800l.println(F("AT+CIPSHUT")); //Resetea las direcciones IP
Serial.println(debug());

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
sim800l.println(F("AT+CSTT=\"internet.movistar.com.co\",\"MOVISTAR\",\"MOVISTAR\""));
Serial.println(debug());
delay(500);

sim800l.println(F("AT+ CIICR")); // Levantar conexión wireless(GPRS o CSD)
Serial.println(debug());
delay(500);

sim800l.println(F("AT+CIFSR")); // Obtiene una dirección IP
Serial.println(debug());
delay(500);

}

void loop() {
//Monitoreo constantemente el peso y se actualiza al oprimir pulsador
 Serial.println(peso);
 Serial.println("gr");
 //Serial.println(bateria);
delay(2000);
 if (pesomin <=40 ) {      
         enviardatos();
         // apagarModulo();
      } 
}




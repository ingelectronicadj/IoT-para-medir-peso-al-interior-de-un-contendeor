
//FSR Indicador RGB
	const int ledRed = 4; // Indicador Nivel Alto
    const int ledBlue = 2; // Indicador Nivel Medio
    const int ledGreen = 7; // Indicador Nivel Minimo
    int fsrPin = 0; // Nodo del divisor entre FSR y R:10K (pulldown are connected to A0)
    int fsrReading; 
    
    void setup(void) {
       Serial.begin(9600);
       pinMode(ledRed, OUTPUT);
       pinMode(ledBlue, OUTPUT);
       pinMode(ledGreen, OUTPUT);
    }
    void loop(void) {
       fsrReading = analogRead(fsrPin); 
       
       if (fsrReading <=100) {
          digitalWrite(ledRed, LOW);
          digitalWrite(ledGreen, LOW);
          digitalWrite(ledBlue, LOW);
       }
       if (fsrReading > 100 && fsrReading <= 500) {
          digitalWrite(ledRed, LOW);
          digitalWrite(ledGreen, HIGH);
          digitalWrite(ledBlue, LOW);
       }
       if (fsrReading > 500 && fsrReading <= 900) {
          digitalWrite(ledRed, LOW);
          digitalWrite(ledGreen, LOW);
          digitalWrite(ledBlue, HIGH);
       }
       if (fsrReading > 900) {
          digitalWrite(ledRed, HIGH);
          digitalWrite(ledGreen, LOW);
          digitalWrite(ledBlue, LOW);
       }
       Serial.print("Analog reading = ");
       Serial.println(fsrReading); 
       delay(250); 
    }
    

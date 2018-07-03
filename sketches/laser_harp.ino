/*
Liceo Scientifico A. Roiti Ferrara 2018

Laser Harp (Blues Scale

Added comments in English per google tranlate
*/
int    sensorValue;								// intensit luminosa dei laser rilevata dai fotoresistori
												          // 	luminous intensity of the lasers detected by the photoresistors
											
int    sensorHigh[7];							// intensit massima rilevata dai fotoresistori
												          // 	maximum intensity detected by photoresistors

int    pinBuz[4];								  // numero di buzzer, ma solo uno.
												          // 	buzzer number, but only one

int    note[7];									  // pin analogici collegati a dei fotoresistori
												          // 	analog pins connected to photoresistors

bool   libero[4];								  // indicatore che comunica se il buzzer funziona (acceso o spento)
												          // 	indicator that tells if the buzzer works (on or off)

int    ass[4];
int    freq[]={262,311,349,370,392,466,523};	// frequenze note
												          // 	frequencies known

int    bt=7;									    // interruttore
int    c=1;										    // 	switch

double t;										      // cronometro
												          // 	stopwatch

void setup() {									  // assegnazione di input e output
												          // 	input and output assignment
												
	for(int i=1,j=0; i<8,j<7; i++,j++){			// pin analogici come input
												                  // 	analog pins as input
		note[j]=i;
		pinMode(i,INPUT);
	}
	
	for(int i=0; i<4; i++){
		libero[i]=true;
		pinBuz[i]=i+3;                       
		pinMode(i+3,OUTPUT);
	}

	Serial.begin(9600);
	for(int i=0; i<7; i++){
		sensorHigh[i]=0;
	}
  
	for(int i=0; i<7; i++){						    // calibrazione dei fotoresistori
		t=millis();       						      // 	photoresistors calibration
		do{
			sensorValue=analogRead(note[i]);	// sV assume il valore del voltaggio che riceve ogini pin dei fotoresistori
												                // 	sV takes the value of the voltage that receives each pin of the photoresistors
			if(sensorValue>sensorHigh[i]){
				sensorHigh[i]=sensorValue; 
			}
		Serial.println(sensorValue);			  // attivazione del monitor seriale dove vedi i valori registrati dai fotoresistori
												                // 	activation of the serial monitor where you see the values ​​recorded by the photoresistors
        }
  
		while(millis()-t<=1000);				    // fai il "do" per 10 secondi
												                // 	do the "do" for 10 seconds
			Serial.println(sensorHigh[i]);  
	}
	
	delay(1000);
}

void loop() {
	for(int i=0; i<7; i++){
		int button;								  // bottone attaccato a pinDig7 e ground
												        // 	button attached to pinDig7 and ground
		button=digitalRead(bt);             
		if(button==1){							// se bottone acceso
												        // 	if button is on
												
			c=2;								      // frequenza alta
												        // 	high frequency
		}
		else{
			c=1;								      // frequenza bassa
												        // 	low frequency
		}

		sensorValue=analogRead(note[i]);
		if(sensorValue>sensorHigh[i]+50){		// se il laser vuiene interrotto e	
			for(int j=0; j<4; j++){				    // la frequenza luminosa rilevata maggiore di quella default								
				if(libero[j]==true || 
					ass[j]==note[i]){			        // il buzzer suona
												                // 	if the laser is interrupted and
												                // 	the detected light frequency greater than the default one
												                // 	the buzzer sounds
		
					libero[j]=false;
					ass[j]=note[i];
					tone(pinBuz[j], c*freq[i],
							100);				              // per quanto pu suonare al minimo la nota
											                  // 	as far as the note can play at a minimum

					delay(10);					          // pausa tra una nota e l'altra
												                // 	pause between one note and another
				}
			}
		}
		else {
			for(int j=0;j<4;j++){				      // altrimenti tace.
												                // otherwise it is silent
				if(ass[j]==note[i]){
					noTone(pinBuz[j]);
					libero[j]=true;
				}
			}
		}
	}

	delay(10);
}

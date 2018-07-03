int    sensorValue;                          	// intensit luminosa dei laser rilevata dai fotoresistori
int    sensorHigh[7];                        	// intensit massima rilevata dai fotoresistori
int    pinBuz[4];                            	// numero di buzzer, ma � solo uno.
int    note[7];                             	// pin analogici collegati a dei fotoresistori
bool   libero[4];                          	  // indicatore che comunica se il buzzer funziona (acceso o spento)
int    ass[4];                                //
int    freq[]={262,311,349,370,392,466,523};  //frequenze note
int    bt=7;
int    c=1;                                	  //interruttore
double t;                                  	  //cronometro

void setup() {                              //assegnazione di input e output
  for(int i=1,j=0; i<8,j<7; i++,j++){       //pin analogici come input
    note[j]=i;                       		    //
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
  
  for(int i=0; i<7; i++){                 	//calibrazione dei fotoresistori
    t=millis();       
      do{
        sensorValue=analogRead(note[i]);  			    //sV assume il valore del voltaggio che riceve ogini pin dei fotoresistori
        if(sensorValue>sensorHigh[i]){
          sensorHigh[i]=sensorValue; 
        }
      Serial.println(sensorValue);    			      //attivazione del monitor seriale dove vedi i valori registrati dai fotoresistori
        }
  while(millis()-t<=1000);  			      //fai il "do" per 10 secondi
    Serial.println(sensorHigh[i]);  
  }
  delay(1000);
}

void loop() {
  for(int i=0; i<7; i++){
    //bottone attaccato a pinDig7 e ground
    int button;
    button=digitalRead(bt);             
    if(button==1){        		  //se bottone acceso
      c=2;                  //frequenza alta
    }
    else{
      c=1;              	  //frequenza bassa
    }

    sensorValue=analogRead(note[i]);
    if(sensorValue>sensorHigh[i]+50){           //se il laser vuiene interrotto e 
      for(int j=0; j<4; j++){               //la frequenza luminosa rilevata � maggiore di quella default
        if(libero[j]==true || ass[j]==note[i]){    	//il buzzer suona
          libero[j]=false;
          ass[j]=note[i];
          tone(pinBuz[j], c*freq[i],100);        		//per quanto pu� suonare al minimo la nota
          delay(10);    				                    //pausa tra una nota e l'altra
          }
      }
    }
    else {
      for(int j=0;j<4;j++){               // altrimenti tace.
        if(ass[j]==note[i]){
          noTone(pinBuz[j]);
          libero[j]=true;
        }
      }
    }
  }

  delay(10);
}

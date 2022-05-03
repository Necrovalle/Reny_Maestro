String ENT;     //Lectura del comando por PC (debug)
char   Sal[2];  //Separacion de ID y accion
char   RES;     //Resopuesta del modulo escalvo


//******************************************* CONFIGURACION INICIAL
void setup() {
  pinMode(48, OUTPUT);    //Data eneble
  pinMode(46, OUTPUT);    //Recive enable
  Serial.begin(9600);     //Comunicacion para DBG
  Serial1.begin(9600);    //Comunicacion con la rede de modulos
  //Serial2.begin(9600);  //Comunicacion con la pantalla
  digitalWrite(48, HIGH); //Modo escritura
  digitalWrite(46, HIGH);  
}


//************************************************** LOOP PRINCIPAL
void loop() {
  if (Serial.available() > 0){
    delay(10);
    ENT = Serial.readString();
    int NB;
    NB = ENT.length();
    Serial.print("Comando: ");
    Serial.println(ENT);
    if (NB == 2){
      ENT.toCharArray(Sal, 3);
      envioID(Sal[0]);
    }
  }
}


//********************************************** FUNCIONES PROPIAS
void envioID(char id_){
  int C_=0;
  Serial1.print(id_);
  delay(20);
  digitalWrite(48, LOW); //Modo escucha
  digitalWrite(46, LOW);
  while (C_ < 15){
    delay(200);
    if (Serial1.available()>0){
      RES = Serial1.read();
      if (RES == id_){
        Serial.print("Respondio el modulo ");
        Serial.println(id_);
        digitalWrite(48, HIGH); //Modo escritura
        digitalWrite(46, HIGH);
        delay(50);
        C_ = 16;
        envioCMD(Sal[1]);
      }
    }
    C_ ++;
    if (C_ == 15){
      Serial.print("Modulo ");
      Serial.print(id_);
      Serial.println(" sin respuesta");
      digitalWrite(48, HIGH); //Modo escritura
      digitalWrite(46, HIGH);
    }
  }
}


void envioCMD(char CMD_){
  int C_=0;
  Serial1.print(CMD_);
  delay(20);
  digitalWrite(48, LOW); //Modo escucha
  digitalWrite(46, LOW);
  while (C_ < 15){
    delay(200);
    if (Serial1.available()>0){
      RES = Serial1.read();
      if (RES == CMD_){
        Serial.print("Accion ejecutada: ");
        Serial.println(RES);
        C_ = 16;
      }
    }
    C_ ++;
    if (C_ == 15){
      Serial.print("Modulo ");
      Serial.print(Sal[0]);
      Serial.println(" sin respuesta");
    }
  }
  digitalWrite(48, HIGH); //Modo escritura
  digitalWrite(46, HIGH);
}

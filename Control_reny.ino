String ENT;     //Lectura del comando por PC (debug)
char   Sal[2];  //Separacion de ID y accion
char   RES;     //Resopuesta del modulo escalvo


//******************************************* CONFIGURACION INICIAL
void setup() {
  Serial.begin(9600);     //Comunicacion para DBG
  Serial1.begin(9600);    //Comunicacion con la rede de modulos
  Serial2.begin(9600);    //Comunicacion con la pantalla
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
  while (C_ < 10){
    delay(200);
    if (Serial1.available()>0){
      RES = Serial1.read();
      if (RES == id_){
        Serial.print("Respondio el modulo ");
        Serial.println(id_);
        C_ = 11;
        envioCMD(Sal[1]);
      }
    }
    C_ ++;
    if (C_ == 10){
      Serial.print("Modulo ");
      Serial.print(id_);
      Serial.println(" sin respuesta");
    }
  }
}


void envioCMD(char CMD_){
  int C_=0;
  Serial1.print(CMD_);
  while (C_ < 10){
    delay(200);
    if (Serial1.available()>0){
      RES = Serial1.read();
      if (RES == CMD_){
        Serial.print("Accion ejecutada en ");
        Serial.println(Sal[0]);
        C_ = 11;
      }
    }
    C_ ++;
    if (C_ == 10){
      Serial.print("Modulo ");
      Serial.print(Sal[0]);
      Serial.println(" sin respuesta");
    }
  }
}

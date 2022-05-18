//Firmware del modulo maestro Reny

#include "Nextion.h"

//*********************************************************************** Variables globales:
boolean Auto=false, Manual=true, Semi=false; //Modos de operación
unsigned int MINS = 0;
unsigned long mSECS = 0;
int minOP=3, minLV=2, minEN=2;
int T_OP[]={0,0,0,0};
int T_LV[]={0,0,0,0};
int T_EN[]={0,0,0,0};

//direcciones de salida de operacion automatica
int clarificado_Auto[] = {1, 2, 3, 4, 5, 6, 7, 8};

int retrolavado_Auto[] = {9, 10, 11, 12, 13, 14, 15, 16};

int enjuague_Auto[] = {17, 18, 19, 20, 21, 22, 23, 24};

//Direcciones del monitor del sistema
//V1 = Agua cruda
//V5 = Desecho retrolavado
//V3 = Desecho enjuague
//V4 = Solucion retrolavado
//V2 = agua clarificada
int V_sistema[8][6]={  //V1, V5, V3, V4, V2, Enable
  {0, 4, 8, 9, 16, 0},      //M1
  {20, 24, 28, 29, 36, 4},  //M2
  {1, 5, 10, 11, 17, 1},     //M3
  {21, 25, 30, 31, 37, 5},  //M4
  {2, 6, 12, 13, 18, 2},    //M5
  {22, 26, 32, 33, 38, 6},  //M6
  {3, 7, 14, 15, 19, 3},    //M7
  {23, 27, 34, 35, 39, 7}   //M8
};

int estado_V[8][6]= {  //V1, V5, V3, V4, V2, Enable
  {1, 1, 1, 1, 1, 1},      //M1
  {1, 1, 1, 1, 1, 1}, //M2
  {1, 1, 1, 1, 1, 1}, //M3
  {1, 1, 1, 1, 1, 1}, //M4
  {1, 1, 1, 1, 1, 1}, //M5
  {1, 1, 1, 1, 1, 1}, //M6
  {1, 1, 1, 1, 1, 1}, //M7
  {1, 1, 1, 1, 1, 1}  //M8
};

int Ent_sistema = 40,
    estado_ENT = 1, 
    SAL_sistema = 8, 
    estado_SAL = 1;

//***************************************************** Declracion de objetos de la pantalla
NexDSButton bt0 = NexDSButton(2, 2, "bt0");
NexButton monitor_b = NexButton(0, 3, "b0");

//Listado de eventos
NexTouch *nex_listen_list[]=
{
  &bt0,
  &monitor_b,
  NULL
};

//******************************************************* Funciones de accion de la pantalla
void operarAutomatico(void *ptr){
  uint32_t estado;
  bt0.getValue(&estado);
  if (estado == 1){
    Auto=true;
    Manual=false; 
    Semi=false;
    //Asignar tiempos iniciales de activacion de etapa
    for (int i=0; i<4; i++){
      T_OP[i]=MINS + i*(minOP);
      T_LV[i]=T_OP[i] + minOP;
      T_EN[i]=T_LV[i] + minLV;
      Serial.print(i);
      Serial.print(", ");
      Serial.print(T_OP[i]);
      Serial.print(", ");
      Serial.print(T_LV[i]);
      Serial.print(", ");
      Serial.println(T_EN[i]);
    }
    dbSerialPrintln("Encendido...");
  } else {
    Auto=false;
    Manual=true; 
    Semi=false;
    apagarAuto();
    dbSerialPrintln("Apagado...");
  }
}

//************************************************************* Funcion de estado del sistema
void mostrarMonitor(void *ptr){
  String Sal;
  for (int i=0; i<8; i++){
    for (int k=0; k<5; k++){
      Sal = "page1.bt" + String(V_sistema[i][k]) + ".val=" + String(estado_V[i][k]); //V
      Serial.println(Sal);
      Serial2.print(Sal);    
      ff();
      delay(9);
    }
    Sal = "page1.r" + String(V_sistema[i][5]) + ".val=" + String(estado_V[i][5]); //E
    Serial.println(Sal);
    Serial2.print(Sal);    
    ff();
    delay(9);
  }
  Sal = "page1.bt" + String(Ent_sistema) + ".val=" + String(estado_ENT);
  Serial.println(Sal);
  Serial2.print(Sal);    
  ff();
  delay(9);
  Sal = "page1.r" + String(SAL_sistema) + ".val=" + String(estado_SAL);
  Serial.println(Sal);
  Serial2.print(Sal);    
  ff();
  delay(9);
}

//************************************************ Funcion de obtencion de minutos operacion
void getMin(){
  unsigned long Actual = millis();
  if ((Actual - mSECS)/6000 == 1){
    MINS++;
    mSECS = Actual;
    Serial2.print("page0.t3.txt=\"");
    Serial2.print(MINS);
    Serial2.print("\"");
    ff();
  }
}

//********************************************************** Funciones de modos de operacion
void operarAuto(){
  //Cambiar a operacion
  for (int i=0; i<4; i++){
    if (T_OP[i] == MINS){
      //Mandar a oprracion
      operando_Auto_linea(i);
      T_OP[i] = T_OP[i] + (minOP + minLV + minEN);
    }
  }
  //Mandar a lavado
  for (int j=0; j<4; j++){
    if (T_LV[j] == MINS){
      //Mandar a anjuegue a modulos
      lavado_Auto_linea(j);
      T_LV[j] = T_LV[j] + (minOP + minLV + minEN);
    }
  }
  //Mandar a lavado
  for (int k=0; k<4; k++){
    if (T_EN[k] == MINS){
      //Mandar a operacion los modulos
      enjuague_Auto_linea(k);
      T_EN[k] = T_EN[k] + (minOP + minLV + minEN);
    }
  }
}

void operarSemi(){
  
}

void operarManual(){
  
}

void lavado_Auto_linea(int N){
  String Sal1 = "page2.bt" + String(clarificado_Auto[2*N]) + ".val=0";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  String Sal2 = "page2.bt" + String(clarificado_Auto[2*N+1]) + ".val=0";
  Serial2.print(Sal2);    
  ff();
  delay(5);
  Sal1 = "page2.bt" + String(retrolavado_Auto[2*N]) + ".val=1";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  Sal2 = "page2.bt" + String(retrolavado_Auto[2*N+1]) + ".val=1";
  Serial2.print(Sal2);    
  ff();
  delay(5);
  Sal1 = "page2.bt" + String(enjuague_Auto[2*N]) + ".val=0";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  Sal2 = "page2.bt" + String(enjuague_Auto[2*N+1]) + ".val=0";
  Serial2.print(Sal2);    
  ff();
  //mandar señales
  Serial.print("Lavando numero: ");
  Serial.println(N);
}

void enjuague_Auto_linea(int N){
  String Sal1 = "page2.bt" + String(clarificado_Auto[2*N]) + ".val=0";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  String Sal2 = "page2.bt" + String(clarificado_Auto[2*N+1]) + ".val=0";
  Serial2.print(Sal2);    
  ff();
  delay(5);
  Sal1 = "page2.bt" + String(retrolavado_Auto[2*N]) + ".val=0";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  Sal2 = "page2.bt" + String(retrolavado_Auto[2*N+1]) + ".val=0";
  Serial2.print(Sal2);    
  ff();
  delay(5);
  Sal1 = "page2.bt" + String(enjuague_Auto[2*N]) + ".val=1";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  Sal2 = "page2.bt" + String(enjuague_Auto[2*N+1]) + ".val=1";
  Serial2.print(Sal2);    
  ff();
  //mandar señales
  Serial.print("Enjuagando numero: ");
  Serial.println(N);
}

void operando_Auto_linea(int N){
  String Sal1 = "page2.bt" + String(clarificado_Auto[2*N]) + ".val=1";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  String Sal2 = "page2.bt" + String(clarificado_Auto[2*N+1]) + ".val=1";
  Serial2.print(Sal2);    
  ff();
  delay(5);
  Sal1 = "page2.bt" + String(retrolavado_Auto[2*N]) + ".val=0";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  Sal2 = "page2.bt" + String(retrolavado_Auto[2*N+1]) + ".val=0";
  Serial2.print(Sal2);    
  ff();
  delay(5);
  Sal1 = "page2.bt" + String(enjuague_Auto[2*N]) + ".val=0";
  Serial2.print(Sal1);    
  ff();
  delay(5);
  Sal2 = "page2.bt" + String(enjuague_Auto[2*N+1]) + ".val=0";
  Serial2.print(Sal2);    
  ff();
  //mandar señales
  Serial.print("Operando numero: ");
  Serial.println(N);
}

void apagarAuto(){
  for (int i=0; i<8; i++){
    String Sal1 = "page2.bt" + String(clarificado_Auto[i]) + ".val=0";
    Serial2.print(Sal1);    
    ff();
    delay(5);
    Sal1 = "page2.bt" + String(retrolavado_Auto[i]) + ".val=0";
    Serial2.print(Sal1);    
    ff();
    delay(5);
    Sal1 = "page2.bt" + String(enjuague_Auto[i]) + ".val=0";
    Serial2.print(Sal1);    
    ff();
    delay(5);
  }
  //mandar señales
  Auto = false;
}

//*************************************************** Funcion de fin de cadena a la pantalla
void ff(){
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

//************************************************************************************ SETUP
void setup() {
  nexInit();
  dbSerialPrintln("Inicializado");
  bt0.attachPop(operarAutomatico, &bt0);
  monitor_b.attachPop(mostrarMonitor, &monitor_b);
  
}

//******************************************************************************** MAIN LOOP
void loop() {
  getMin();
  if (Auto){
    operarAuto();
  } else if(Semi) {
    operarSemi();
  } else if (Manual) {
    operarManual();
  }
  nexLoop(nex_listen_list);
}

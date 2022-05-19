//Firmware del modulo maestro Reny

#include "Nextion.h"

//*********************************************************************** Variables globales:
boolean Auto=false, Manual=true, Semi=false; //Modos de operación
int N_man; //Linea de trabajo para operacion manual
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
  {1, 5, 10, 11, 17, 1},    //M3
  {21, 25, 30, 31, 37, 5},  //M4
  {2, 6, 12, 13, 18, 2},    //M5
  {22, 26, 32, 33, 38, 6},  //M6
  {3, 7, 14, 15, 19, 3},    //M7
  {23, 27, 34, 35, 39, 7}   //M8
};

int name_Semi[8][3] = {
  {0, 8, 16},
  {1, 9, 17},
  {2, 10, 18},
  {3, 11, 19},
  {4, 12, 20},
  {5, 13, 21},
  {6, 14, 22},
  {7, 15, 23}
};

int estado_V[8][6]= {  //V1, V5, V3, V4, V2, Enable
  {0, 0, 0, 0, 0, 0},      //M1
  {0, 0, 0, 0, 0, 0}, //M2
  {0, 0, 0, 0, 0, 0}, //M3
  {0, 0, 0, 0, 0, 0}, //M4
  {0, 0, 0, 0, 0, 0}, //M5
  {0, 0, 0, 0, 0, 0}, //M6
  {0, 0, 0, 0, 0, 0}, //M7
  {0, 0, 0, 0, 0, 0}  //M8
};

int Ent_sistema = 40,
    estado_ENT = 1, 
    SAL_sistema = 8, 
    estado_SAL = 1;

//***************************************************** Declracion de objetos de la pantalla
NexDSButton bt0 = NexDSButton(2, 2, "bt0");  //(page, id, name) modo automatico
NexButton monitor_b = NexButton(0, 3, "b0");  //Monitor del sistema
//Clarificacion semiautomatico
NexDSButton Clarificacion1 = NexDSButton(3, 16, "bt0");
NexDSButton Clarificacion2 = NexDSButton(3, 17, "bt1");
NexDSButton Clarificacion3 = NexDSButton(3, 18, "bt2");
NexDSButton Clarificacion4 = NexDSButton(3, 19, "bt3");
NexDSButton Clarificacion5 = NexDSButton(3, 20, "bt4");
NexDSButton Clarificacion6 = NexDSButton(3, 21, "bt5");
NexDSButton Clarificacion7 = NexDSButton(3, 22, "bt6");
NexDSButton Clarificacion8 = NexDSButton(3, 23, "bt7");
//Retrolavado semiautomatico
NexDSButton Retrolavado1 = NexDSButton(3, 24, "bt8");
NexDSButton Retrolavado2 = NexDSButton(3, 25, "bt9");
NexDSButton Retrolavado3 = NexDSButton(3, 26, "bt10");
NexDSButton Retrolavado4 = NexDSButton(3, 27, "bt11");
NexDSButton Retrolavado5 = NexDSButton(3, 28, "bt12");
NexDSButton Retrolavado6 = NexDSButton(3, 29, "bt13");
NexDSButton Retrolavado7 = NexDSButton(3, 30, "bt14");
NexDSButton Retrolavado8 = NexDSButton(3, 31, "bt15");
//Enjuague semiautomatico
NexDSButton Enjuague1 = NexDSButton(3, 32, "bt16");
NexDSButton Enjuague2 = NexDSButton(3, 33, "bt17");
NexDSButton Enjuague3 = NexDSButton(3, 34, "bt18");
NexDSButton Enjuague4 = NexDSButton(3, 35, "bt19");
NexDSButton Enjuague5 = NexDSButton(3, 36, "bt20");
NexDSButton Enjuague6 = NexDSButton(3, 37, "bt21");
NexDSButton Enjuague7 = NexDSButton(3, 38, "bt22");
NexDSButton Enjuague8 = NexDSButton(3, 39, "bt23");
//Apagado total semiautomatico
NexButton Apagado_semi = NexButton(3, 40, "b1");
//Selector de linea en operacion manual
NexButton LINEA1 = NexButton(4, 2, "b0");
NexButton LINEA2 = NexButton(4, 3, "b1");
NexButton LINEA3 = NexButton(4, 4, "b2");
NexButton LINEA4 = NexButton(4, 5, "b3");
NexButton LINEA5 = NexButton(4, 6, "b4");
NexButton LINEA6 = NexButton(4, 7, "b5");
NexButton LINEA7 = NexButton(4, 10, "b7");
NexButton LINEA8 = NexButton(4, 11, "b8");

//Listado de eventos
NexTouch *nex_listen_list[]=
{
  &bt0,
  &monitor_b,
  &Clarificacion1,
  &Clarificacion2,
  &Clarificacion3,
  &Clarificacion4,
  &Clarificacion5,
  &Clarificacion6,
  &Clarificacion7,
  &Clarificacion8,
  &Retrolavado1,
  &Retrolavado2,
  &Retrolavado3,
  &Retrolavado4,
  &Retrolavado5,
  &Retrolavado6,
  &Retrolavado7,
  &Retrolavado8,
  &Enjuague1,
  &Enjuague2,
  &Enjuague3,
  &Enjuague4,
  &Enjuague5,
  &Enjuague6,
  &Enjuague7,
  &Enjuague8,
  &Apagado_semi,
  &LINEA1,
  &LINEA2,
  &LINEA3,
  &LINEA4,
  &LINEA5,
  &LINEA6,
  &LINEA7,
  &LINEA8,
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
    delay(8);
    Sal1 = "page2.bt" + String(retrolavado_Auto[i]) + ".val=0";
    Serial2.print(Sal1);    
    ff();
    delay(8);
    Sal1 = "page2.bt" + String(enjuague_Auto[i]) + ".val=0";
    Serial2.print(Sal1);    
    ff();
    delay(8);
  }
  //mandar señales
  Auto = false;
}

//Clarificado semi automatico
void CLF_semi(int N, int EDO){
  String Sal;
  if (EDO == 1){
    Sal = "Calrificacion en " + String(N) + " ON";
    Serial.println(Sal);
    //Apagar las otras dos operaciones de la linea
    Sal = "page3.bt" + String(name_Semi[N-1][1]) + ".val=0";
    Serial2.print(Sal);
    ff();
    delay(8);
    Sal = "page3.bt" + String(name_Semi[N-1][2]) + ".val=0";
    Serial2.print(Sal);
    ff();
    delay(8);
    //mandar señales
  } else {
    Sal = "Calrificacion en " + String(N) + " OFF";
    Serial.println(Sal);
    //mandar señales
  }
}

void Clarificar1(void *ptr){
  uint32_t estado;
  Semi = true;
  Clarificacion1.getValue(&estado);
  if (estado){
    CLF_semi(1, 1);
  } else {
    CLF_semi(1,0);
  }
}

void Clarificar2(void *ptr){
  uint32_t estado;
  Semi = true;
  Clarificacion2.getValue(&estado);
  if (estado){
    CLF_semi(2, 1);
  } else {
    CLF_semi(2,0);
  }
}

void Clarificar3(void *ptr){
  uint32_t estado;
  Semi = true;
  Clarificacion3.getValue(&estado);
  if (estado){
    CLF_semi(3, 1);
  } else {
    CLF_semi(3,0);
  }
}

void Clarificar4(void *ptr){
  uint32_t estado;
  Semi = true;
  Clarificacion4.getValue(&estado);
  if (estado){
    CLF_semi(4, 1);
  } else {
    CLF_semi(4,0);
  }
}

void Clarificar5(void *ptr){
  uint32_t estado;
  Semi = true;
  Clarificacion5.getValue(&estado);
  if (estado){
    CLF_semi(5, 1);
  } else {
    CLF_semi(5,0);
  }
}

void Clarificar6(void *ptr){
  uint32_t estado;
  Semi = true;
  Clarificacion6.getValue(&estado);
  if (estado){
    CLF_semi(6, 1);
  } else {
    CLF_semi(6,0);
  }
}

void Clarificar7(void *ptr){
  uint32_t estado;
  Semi = true;
  Clarificacion7.getValue(&estado);
  if (estado){
    CLF_semi(7, 1);
  } else {
    CLF_semi(7,0);
  }
}

void Clarificar8(void *ptr){
  uint32_t estado;
  Semi = true;
  Clarificacion8.getValue(&estado);
  if (estado){
    CLF_semi(8, 1);
  } else {
    CLF_semi(8,0);
  }
}

//Retrolavado semi automatico
void RLV_semi(int N, int EDO){
  String Sal;
  if (EDO == 1){
    Sal = "Retrolavado en " + String(N) + " ON";
    Serial.println(Sal);
    //Apagar las otras dos operaciones de la linea
    Sal = "page3.bt" + String(name_Semi[N-1][0]) + ".val=0";
    Serial2.print(Sal);
    ff();
    delay(8);
    Sal = "page3.bt" + String(name_Semi[N-1][2]) + ".val=0";
    Serial2.print(Sal);
    ff();
    delay(8);
    //mandar señales
  } else {
    Sal = "Retrovado en " + String(N) + " OFF";
    Serial.println(Sal);
    //mandar señales
  }
}

void Retrolavar1(void *ptr){
  uint32_t estado;
  Semi = true;
  Retrolavado1.getValue(&estado);
  if (estado){
    RLV_semi(1, 1);
  } else {
    RLV_semi(1,0);
  }
}

void Retrolavar2(void *ptr){
  uint32_t estado;
  Semi = true;
  Retrolavado2.getValue(&estado);
  if (estado){
    RLV_semi(2, 1);
  } else {
    RLV_semi(2,0);
  }
}
void Retrolavar3(void *ptr){
  uint32_t estado;
  Semi = true;
  Retrolavado3.getValue(&estado);
  if (estado){
    RLV_semi(3, 1);
  } else {
    RLV_semi(3,0);
  }
}

void Retrolavar4(void *ptr){
  uint32_t estado;
  Semi = true;
  Retrolavado4.getValue(&estado);
  if (estado){
    RLV_semi(4, 1);
  } else {
    RLV_semi(4,0);
  }
}

void Retrolavar5(void *ptr){
  uint32_t estado;
  Semi = true;
  Retrolavado5.getValue(&estado);
  if (estado){
    RLV_semi(5, 1);
  } else {
    RLV_semi(5,0);
  }
}

void Retrolavar6(void *ptr){
  uint32_t estado;
  Semi = true;
  Retrolavado6.getValue(&estado);
  if (estado){
    RLV_semi(6, 1);
  } else {
    RLV_semi(6,0);
  }
}

void Retrolavar7(void *ptr){
  uint32_t estado;
  Semi = true;
  Retrolavado7.getValue(&estado);
  if (estado){
    RLV_semi(7, 1);
  } else {
    RLV_semi(7,0);
  }
}

void Retrolavar8(void *ptr){
  uint32_t estado;
  Semi = true;
  Retrolavado8.getValue(&estado);
  if (estado){
    RLV_semi(8, 1);
  } else {
    RLV_semi(8,0);
  }
}

//Enjuague semi automatico
void ENJ_semi(int N, int EDO){
  String Sal;
  if (EDO == 1){
    Sal = "Enjuague en " + String(N) + " ON";
    Serial.println(Sal);
    //Apagar las otras dos operaciones de la linea
    Sal = "page3.bt" + String(name_Semi[N-1][0]) + ".val=0";
    Serial2.print(Sal);
    ff();
    delay(8);
    Sal = "page3.bt" + String(name_Semi[N-1][1]) + ".val=0";
    Serial2.print(Sal);
    ff();
    delay(8);
    //mandar señales
  } else {
    Sal = "Enjuague en " + String(N) + " OFF";
    Serial.println(Sal);
    //mandar señales
  }
}

void Enjuagar1(void *ptr){
  uint32_t estado;
  Semi = true;
  Enjuague1.getValue(&estado);
  if (estado){
    ENJ_semi(1, 1);
  } else {
    ENJ_semi(1,0);
  }
}

void Enjuagar2(void *ptr){
  uint32_t estado;
  Semi = true;
  Enjuague2.getValue(&estado);
  if (estado){
    ENJ_semi(2, 1);
  } else {
    ENJ_semi(2,0);
  }
}

void Enjuagar3(void *ptr){
  uint32_t estado;
  Semi = true;
  Enjuague3.getValue(&estado);
  if (estado){
    ENJ_semi(3, 1);
  } else {
    ENJ_semi(3,0);
  }
}

void Enjuagar4(void *ptr){
  uint32_t estado;
  Semi = true;
  Enjuague4.getValue(&estado);
  if (estado){
    ENJ_semi(4, 1);
  } else {
    ENJ_semi(4,0);
  }
}

void Enjuagar5(void *ptr){
  uint32_t estado;
  Semi = true;
  Enjuague5.getValue(&estado);
  if (estado){
    ENJ_semi(5, 1);
  } else {
    ENJ_semi(5,0);
  }
}

void Enjuagar6(void *ptr){
  uint32_t estado;
  Semi = true;
  Enjuague6.getValue(&estado);
  if (estado){
    ENJ_semi(6, 1);
  } else {
    ENJ_semi(6,0);
  }
}

void Enjuagar7(void *ptr){
  uint32_t estado;
  Semi = true;
  Enjuague7.getValue(&estado);
  if (estado){
    ENJ_semi(7, 1);
  } else {
    ENJ_semi(7,0);
  }
}

void Enjuagar8(void *ptr){
  uint32_t estado;
  Semi = true;
  Enjuague8.getValue(&estado);
  if (estado){
    ENJ_semi(8, 1);
  } else {
    ENJ_semi(8,0);
  }
}

//Apagar semi automatico
void Apagar_semi(void *ptr){
  String Sal;
  for (int i=0; i<8; i++){
    for (int j=0; j<3; j++){
      Sal = "page3.bt" + String(name_Semi[i][j]) + ".val=0";
      Serial2.print(Sal);
      ff();
      Serial.println(Sal);
      delay(8);
    }
  }
  //mandar señales
}

//Operacion en manual
void operar1(void *ptr){
  N_man = 1;
  Serial.println(N_man);
}

void operar2(void *ptr){
  N_man = 2;
  Serial.println(N_man);
}

void operar3(void *ptr){
  N_man = 3;
  Serial.println(N_man);
}

void operar4(void *ptr){
  N_man = 4;
  Serial.println(N_man);
}

void operar5(void *ptr){
  N_man = 5;
  Serial.println(N_man);
}

void operar6(void *ptr){
  N_man = 6;
  Serial.println(N_man);
}

void operar7(void *ptr){
  N_man = 7;
  Serial.println(N_man);
}

void operar8(void *ptr){
  N_man = 8;
  Serial.println(N_man);
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
  Clarificacion1.attachPop(Clarificar1, &Clarificacion1);
  Clarificacion2.attachPop(Clarificar2, &Clarificacion2);
  Clarificacion3.attachPop(Clarificar3, &Clarificacion3);
  Clarificacion4.attachPop(Clarificar4, &Clarificacion4);
  Clarificacion5.attachPop(Clarificar5, &Clarificacion5);
  Clarificacion6.attachPop(Clarificar6, &Clarificacion6);
  Clarificacion7.attachPop(Clarificar7, &Clarificacion7);
  Clarificacion8.attachPop(Clarificar8, &Clarificacion8);
  Retrolavado1.attachPop(Retrolavar1, &Retrolavado1);
  Retrolavado2.attachPop(Retrolavar2, &Retrolavado2);
  Retrolavado3.attachPop(Retrolavar3, &Retrolavado3);
  Retrolavado4.attachPop(Retrolavar4, &Retrolavado4);
  Retrolavado5.attachPop(Retrolavar5, &Retrolavado5);
  Retrolavado6.attachPop(Retrolavar6, &Retrolavado6);
  Retrolavado7.attachPop(Retrolavar7, &Retrolavado7);
  Retrolavado8.attachPop(Retrolavar8, &Retrolavado8);
  Enjuague1.attachPop(Enjuagar1, &Enjuague1);
  Enjuague2.attachPop(Enjuagar2, &Enjuague2);
  Enjuague3.attachPop(Enjuagar3, &Enjuague3);
  Enjuague4.attachPop(Enjuagar4, &Enjuague4);
  Enjuague5.attachPop(Enjuagar5, &Enjuague5);
  Enjuague6.attachPop(Enjuagar6, &Enjuague6);
  Enjuague7.attachPop(Enjuagar7, &Enjuague7);
  Enjuague8.attachPop(Enjuagar8, &Enjuague8);
  Apagado_semi.attachPop(Apagar_semi, &Apagado_semi);
  LINEA1.attachPop(operar1, &LINEA1);
  LINEA2.attachPop(operar2, &LINEA2);
  LINEA3.attachPop(operar3, &LINEA3);
  LINEA4.attachPop(operar4, &LINEA4);
  LINEA5.attachPop(operar5, &LINEA5);
  LINEA6.attachPop(operar6, &LINEA6);
  LINEA7.attachPop(operar7, &LINEA7);
  LINEA8.attachPop(operar8, &LINEA8);
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

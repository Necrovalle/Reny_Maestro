//*********************************************************** DECLARACIONES GLOBALES
char ENT;
boolean id1;
unsigned long T1;
unsigned long Ta;

//******************************************************** CONFIGURACION DEL SISTEMA
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);  //LED in build
  pinMode(8, OUTPUT);   //Enable escritura RS485
  digitalWrite(13, HIGH);
  delay(300);
  digitalWrite(13, LOW);
  id1 = false;
  digitalWrite(8, LOW); //Modo escucha RS485
}

//****************************************************************** CICLO PRINCIPAL
void loop() {
  if (id1){
    Ta = millis();
    if (Ta > (T1+2000)){
      Serial.print('Z');
      id1 = false;
    }
  }
  if (Serial.available()>0){
    digitalWrite(13, HIGH);
    ENT = Serial.read();
    digitalWrite(8, HIGH); //Modo ecritura RS485
    delay(20);
    if (!id1){
      switch (ENT){
        case '1':
          Serial.print('1');
          break;
        case '2':
          Serial.print('2');
          break;
        case '3':
          Serial.print('3');
          break;
        case '4':
          Serial.print('4');
          break;
        case '5':
          Serial.print('5');
          break;
        case '6':
          Serial.print('6');
          break;
        case '7':
          Serial.print('7');
          break;
        case '8':
          Serial.print('8');
          break;
        case '9':
          Serial.print('9');
          break;
        default:
          Serial.print('0');
          break;
      }
      id1 = true;
      T1 = millis();
    } else {
      switch (ENT){
        case 'A':  //Operar
          Serial.print('A');
          break;
        case 'B':   //Retrolavado
          Serial.print('B');
          break;
        case 'C':   //Enjuague
          Serial.print('C');
          break;
        case 'H':   //V1 on
          Serial.print('H');
          break;
        case 'h':   //V1 off
          Serial.print('h');
          break;
        case 'I':   //V2 on
          Serial.print('I');
          break;
        case 'i':   //V2 off
          Serial.print('i');
          break;
        case 'J':   //V3 on
          Serial.print('J');
          break;
        case 'j':   //V3 off
          Serial.print('j');
          break;
        case 'K':   //V4 on
          Serial.print('K');
          break;
        case 'k':   //V4 off
          Serial.print('k');
          break;
        case 'L':   //V5 on
          Serial.print('L');
          break;
        case 'l':   //V5 off
          Serial.print('l');
          break;
        case 'X':   //Cerrar todo
          Serial.print('X');
          break;
        default:
          Serial.print('Z');
      }
      id1 = false;
      digitalWrite(13, LOW);
    }
    digitalWrite(8, LOW); //Modo escucha RS485
  }
}
//******************************************************************** FIN DE CODIGO

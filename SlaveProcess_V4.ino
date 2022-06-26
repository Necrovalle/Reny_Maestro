
// https://www.youtube.com/watch?v=ZR9QEWpGEDo
/*V4 Modificacion Miguel
  V6 Time Out
  V10 comunicacion unidireccional
*/
#define LED       13
#define SLAVE_EN_TX  2  //D3 Morado (DE)
#define SLAVE_EN_RX  3  //D3 Verde  (RE)
#define Relay1    4
#define Relay2    5
#define Relay3    6
#define Relay4    7
#define Relay5    8
char var, var2;
char ID = '6';
int i;
double Timer = 0;

void setup() {
  pinMode(LED , OUTPUT);                        // Declare LED pin as output
  pinMode(SLAVE_EN_RX , OUTPUT);                   // Declare Enable pin as output
  pinMode(SLAVE_EN_TX , OUTPUT);
  pinMode(Relay1 , OUTPUT);
  pinMode(Relay2 , OUTPUT);
  pinMode(Relay3 , OUTPUT);
  pinMode(Relay4 , OUTPUT);
  pinMode(Relay5 , OUTPUT);
  Serial.begin(9600);                           // set serial communication baudrate
  digitalWrite(SLAVE_EN_TX , LOW);
  digitalWrite(SLAVE_EN_RX , LOW);                 // Make Enable pin low
  digitalWrite(Relay1 , HIGH);
  digitalWrite(Relay2 , HIGH);
  digitalWrite(Relay3 , HIGH);
  digitalWrite(Relay4 , HIGH);
  digitalWrite(Relay5 , HIGH);
  // Receiving mode ON
  i = 0;
}

void loop() {

  while (Serial.available())                    // If serial data is available then enter into while loop
  {
    var = Serial.read();                  // if available data is A
    digitalWrite(LED, HIGH);

    if (var == ID) {

      Timer = millis();

      //Cambiar a escribir

      digitalWrite(SLAVE_EN_TX , HIGH); digitalWrite(SLAVE_EN_RX , HIGH);    // Make Enable pin high to send Data
      delay(10);
      // delay(50); Serial.print(";"); Serial.print(ID); Serial.flush();
      Serial.print(ID); Serial.flush();
      // set serial communication  READ
      digitalWrite(SLAVE_EN_RX , LOW); digitalWrite(SLAVE_EN_TX , LOW);                // Make Enable pin low
      delay(5);
      i = 0; //Bandera

      while (i == 0) {

        while (Serial.available())                    // If serial data is available then enter into while loop
        {
          var2 = Serial.read();                  // if available data is A
          //          digitalWrite(SLAVE_EN_RX , HIGH); digitalWrite(SLAVE_EN_TX , HIGH);
                    delay(10);
          //          Serial.print(":"); Serial.print(var2);
          switch (var2) {
            case 'A':
              Serial.print("A"); Serial.flush(); delay(300);//Operacion
              digitalWrite(Relay1 , HIGH); delay(20); //
              digitalWrite(Relay2 , HIGH); delay(20);
              digitalWrite(Relay3 , HIGH); delay(20);
              digitalWrite(Relay4 , HIGH); delay(20);
              digitalWrite(Relay5 , HIGH); delay(20);
              break;
            case 'B':
              Serial.print("B"); Serial.flush(); delay(300);//Retorolavado
              digitalWrite(Relay1 , LOW); delay(20); //
              digitalWrite(Relay2 , LOW); delay(20);
              digitalWrite(Relay3 , LOW); delay(20);
              digitalWrite(Relay4 , LOW); delay(20);
              digitalWrite(Relay5 , HIGH);  delay(20);
              break;
            case 'C':
              Serial.print("C"); Serial.flush(); delay(300);//Enjuague
              digitalWrite(Relay1 ,HIGH  ); delay(20); //
              digitalWrite(Relay2 , LOW); delay(20);
              digitalWrite(Relay3 , HIGH ); delay(20);
              digitalWrite(Relay4 , HIGH ); delay(20);
              digitalWrite(Relay5 , LOW); delay(20);
              break;

            case 'H':
              Serial.print("H"); Serial.flush(); delay(300);//Valvula 1
              digitalWrite(Relay1 , HIGH  ); delay(20); //
              break;

            case 'h':
              Serial.print("h"); Serial.flush(); delay(300);//
              digitalWrite(Relay1 , LOW ); delay(20); //
              break;

            case 'I':
              Serial.print("I"); Serial.flush(); delay(300);//Valvula 2
              digitalWrite(Relay2 , HIGH ); delay(20); //
              break;

            case 'i':
              Serial.print("i"); Serial.flush(); delay(300);//
              digitalWrite(Relay2 , LOW ); delay(20); //
              break;

            case 'J':
              Serial.print("J"); Serial.flush(); delay(300);//Valvula 3
              digitalWrite(Relay3 , LOW ); delay(20); //
              break;

            case 'j':
              Serial.print("j"); Serial.flush(); delay(300);//
              digitalWrite(Relay3 , HIGH ); delay(20); //
              break;

            case 'K':
              Serial.print("K"); Serial.flush(); delay(300);//Valvula 4
              digitalWrite(Relay4 , LOW ); delay(20); //
              break;

            case 'k':
              Serial.print("k"); Serial.flush(); delay(300);//
              digitalWrite(Relay4 , HIGH ); delay(20); //
              break;

            case 'L':
              Serial.print("L"); Serial.flush(); delay(300);//Valvula 5
              digitalWrite(Relay5 , LOW ); delay(20); //
              break;

            case 'l':
              Serial.print("l"); Serial.flush(); delay(300);//
              digitalWrite(Relay5 , HIGH ); delay(20); //
              break;

            case 'X':
              Serial.print("X"); Serial.flush(); delay(300);// Todas Cerradas
              digitalWrite(Relay1 , LOW); delay(20); //
              digitalWrite(Relay2 , LOW); delay(20);
              digitalWrite(Relay3 , HIGH); delay(20);
              digitalWrite(Relay4 , HIGH); delay(20);
              digitalWrite(Relay5 , HIGH); delay(20);
              break;

            default:
              Serial.print("Z"); Serial.flush(); delay(300);// Todas Cerradas

              break;
          }
          i = 1;
          digitalWrite(LED, LOW);
          var = "0";
          digitalWrite(SLAVE_EN_RX , LOW); digitalWrite(SLAVE_EN_TX , LOW);
        }

        if (((millis() - Timer) > 2000) && (i == 0))
        {
          i = 1;
          Serial.println('N');
        }


      }


    }
  }
}

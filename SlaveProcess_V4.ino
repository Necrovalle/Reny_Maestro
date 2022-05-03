
// https://www.youtube.com/watch?v=ZR9QEWpGEDo

#define LED       13
#define SLAVE_EN_TX  2  //D3 Morado (DE)
#define SLAVE_EN_RX  3  //D3 Verde  (RE)

#define Relay1    4
#define Relay2    5
#define Relay3    6
#define Relay4    7
#define Relay5    8
char var, var2;
char ID='1';
int i;


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
  digitalWrite(Relay1 , LOW);
  digitalWrite(Relay2 , LOW);
  digitalWrite(Relay3 , LOW);
  digitalWrite(Relay4 , LOW);
  digitalWrite(Relay5 , LOW);
  // Receiving mode ON
  i = 0;
}

void loop() {
  while (Serial.available())                    // If serial data is available then enter into while loop
  {
    var = Serial.read();                  // if available data is A
    digitalWrite(LED, HIGH);
    if (var == ID) {

      //Cambiar a escribir
      
      digitalWrite(SLAVE_EN_TX , HIGH); digitalWrite(SLAVE_EN_RX , HIGH);    // Make Enable pin high to send Data
      
      delay(50);Serial.print(ID);Serial.flush();  
      
      // set serial communication  READ
      digitalWrite(SLAVE_EN_RX , LOW); digitalWrite(SLAVE_EN_TX , LOW);                // Make Enable pin low
      delay(50);
      i = 0;

      while (i == 0) {

        while (Serial.available())                    // If serial data is available then enter into while loop
        {
          var2 = Serial.read();                  // if available data is A
          digitalWrite(SLAVE_EN_RX , HIGH); digitalWrite(SLAVE_EN_TX , HIGH);
          delay(20);
          //Serial.print(var2);
          switch (var2) {
            case 'A':  
              Serial.print("A"); Serial.flush(); delay(300);//  
              digitalWrite(Relay1 , LOW); delay(20); //
              digitalWrite(Relay2 , LOW); delay(20);
              digitalWrite(Relay3 , LOW); delay(20);
              digitalWrite(Relay4 , LOW); delay(20);
              digitalWrite(Relay5 , LOW); delay(20);
              break;
            case 'B':
              Serial.print("B"); Serial.flush(); delay(300);//  
              digitalWrite(Relay1 , HIGH); delay(20); //
              digitalWrite(Relay2 , HIGH); delay(20);
              digitalWrite(Relay3 , HIGH); delay(20);
              digitalWrite(Relay4 , HIGH); delay(20);
              digitalWrite(Relay5 , LOW);  delay(20);
              break;
            case 'C':
              Serial.print("C"); Serial.flush(); delay(300);//  
              digitalWrite(Relay1 , LOW ); delay(20); //
              digitalWrite(Relay2 , HIGH); delay(20);
              digitalWrite(Relay3 , LOW ); delay(20);
              digitalWrite(Relay4 , LOW ); delay(20);
              digitalWrite(Relay5 , HIGH); delay(20);
              break;
            default:
              Serial.print("Zero"); Serial.flush(); delay(300);//  
              digitalWrite(Relay1 , LOW); delay(20); //
              digitalWrite(Relay2 , LOW); delay(20);
              digitalWrite(Relay3 , LOW); delay(20);
              digitalWrite(Relay4 , LOW); delay(20);
              digitalWrite(Relay5 , LOW); delay(20);
              break;
          }
          i = 1;
          digitalWrite(LED, LOW);
          var = "0";
          digitalWrite(SLAVE_EN_RX , LOW); digitalWrite(SLAVE_EN_TX , LOW); 
        }
      }
    }
  }
}

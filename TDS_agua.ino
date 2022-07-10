#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int LA;
float V;
float ppm;
float uS;
float f  = 1.078; //factor de calibracion
float Vr = 4.85;  //Voltaje de referencia
float ACP; //en uS
int RF; //Referencia de aceptacion analogico1

#define rojo 4    //led de acetacion
#define verde 3   //led de rechazo
#define relee 5   //Salida digital para electrovalvulas

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();
  Serial.begin(9600);
  lcd.backlight();
  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(relee, OUTPUT);
  lcd.print("Inicializando...");
  lcd.setCursor(0,1);
  lcd.print("Conductividad");
  delay(1100);
  digitalWrite(relee, HIGH);
}

void loop() {
  RF = analogRead(A1);
  ACP = map(RF, 0, 1023, 0, 600);
  Serial.print("RF = ");
  Serial.println(ACP);
  LA = analogRead(A0);
  V = (LA / 1023.0) * Vr;
  ppm = (V * 1000) / 2.3;
  ppm = ppm / f; 
  uS = ppm * 2;
  //Factor de caliobracion pendiente en regresion linial
  Serial.print("V= ");
  Serial.println(V);
  Serial.print("uS/cm= ");
  Serial.println(uS);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("S.P.  =");
  lcd.setCursor(8, 0);
  lcd.print(ACP);
  lcd.setCursor(0,1);
  lcd.print("uS/cm =");
  lcd.setCursor(8, 1);
  lcd.print(uS);
  if (uS <= ACP){
    if (uS <= 0.5){
      digitalWrite(verde, LOW);
      digitalWrite(rojo, LOW);
      Serial.println("Vacio");
      digitalWrite(relee, HIGH);
    } else {
      digitalWrite(verde, HIGH);
      digitalWrite(rojo, LOW);
      Serial.println("Aceptable");
      digitalWrite(relee, HIGH);
    }
  } else {
    digitalWrite(verde, LOW);
    digitalWrite(rojo, HIGH);
    Serial.println("Rechazar");
    digitalWrite(relee, LOW);
  }
  delay(900);
}

char ENT;
boolean Act;
int LA;
float V;
float ppm;

void setup() {
  Serial.begin(9600);
  Act = false;
}

void loop() {
  if (Serial.available()>0){
    ENT = Serial.read();
    if (ENT == 'A'){Act = true;}
    if (ENT == 'X'){Act = false;}
    ENT = '0';
  }
  if (Act){
    LA = analogRead(A0);
    V = (LA / 1023.0) * 4.85;
    ppm = (V * 1000) / 2.3;
    ppm = ppm / 1.078; 
    //Factor de caliobracion pendiente en regresion linial
    Serial.print("V= ");
    Serial.println(V);
    Serial.print("ppm= ");
    Serial.println(ppm);
    delay(900);
  }
}

#define BLYNK_TEMPLATE_ID "TMPLLbUC2wrM"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "G9m6F_abpBgA8WWtnJX-_5B-PaSP8gJi"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SLT-Fiber-619";
char pass[] = "ruchira9635";
int ldr_pin = D1;
int pir_pin = D8;
int gas_pin = A0;
int buzzer_pin = D3;
int relay_pin = D7;
int gled_pin = D6;
int rled_pin = D5;
int greenState = LOW;
long previousMillis;
float gas_val;
int Value;
////////////////////////////////////////////////////////////////////////////////////////////////////
/
BLYNK_WRITE(V1){
 int pinvalue = param.asInt();
 digitalWrite(relay_pin,pinvalue);
}
BLYNK_WRITE(V3) // this command is listening when something is written to V1
{
 Value = param.asInt(); // assigning incoming value from pin V1 to a variable
 
}
BLYNK_CONNECTED()
{
Blynk.syncVirtual(V1); 
Blynk.syncVirtual(V3);
}
void gas_sense_send(){
 int smoke_data = analogRead(gas_pin); 
 Blynk.virtualWrite(V0, smoke_data); 
}
void gasSense(){
 
 if (Value == 1){
 //////////////////////////////////////////////////////////////////
 gas_val=analogRead(gas_pin);
 //Serial.println(gas_val);
 if(gas_val<450) 
 {
 digitalWrite(rled_pin, LOW);
 digitalWrite(gled_pin, HIGH);
 noTone(buzzer_pin);
 gas_sense_send(); 
 }
 else if (gas_val>450)
 {
 digitalWrite(gled_pin, LOW);
 digitalWrite(rled_pin, HIGH);
 tone(buzzer_pin, HIGH);
 delay(500); 
 noTone(buzzer_pin); 
 delay(500);
 tone(buzzer_pin, HIGH);
 delay(500);
 gas_sense_send(); 
 }
 /////////////////////////////////////////////////////////////////////
 } else {
 if (millis() - previousMillis >= 1000) {
 previousMillis = millis();
 if(greenState == LOW){
 greenState = HIGH;
 }else{
 greenState = LOW;
 }
 digitalWrite(gled_pin, greenState);
 int smoke_data = 0; 
 Blynk.virtualWrite(V0, smoke_data);
 }
 }
 
 
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
 Serial.begin(115200);
 Blynk.begin(auth, ssid, pass);
 pinMode(ldr_pin, INPUT);
 pinMode(pir_pin, INPUT);
 pinMode(gas_pin, INPUT);
 pinMode(buzzer_pin, OUTPUT);
 pinMode(relay_pin, OUTPUT);
 pinMode(gled_pin, OUTPUT);
 pinMode(rled_pin, OUTPUT);
}
void sensing(){
int ldr_val = digitalRead(ldr_pin);
 int pir_val = digitalRead(pir_pin);
 if(ldr_val == 1 && pir_val == 1){
 Serial.println(pir_val);
 digitalWrite(relay_pin, HIGH);
 delay(3000);
 gasSense();
 }
 else if(ldr_val == 1 && pir_val == 0){
 Serial.println(pir_val);
 digitalWrite(relay_pin, LOW);
 gasSense();
 }
 else{
 gasSense();
 } 
}
void loop()
{
 Blynk.run();
 sensing();
}

#include <lorawan.h>
#include <DHT.h>
#define DHTPIN 14
const char *devAddr = "64afbb7a";
const char *appSKey = "64afbb7a6f939ea8fdf270a3566fd6b6"; //
const char *nwkSKey = "64afbb7a5c096a4cc7445deda051fe0d"; //
const unsigned long interval = 10000; // interval pengiriman data 10 detik
unsigned long previousMillis = 0;
#define DHTTYPE DHT11 // tipe DHT
DHT dht(DHTPIN, DHTTYPE);
char myStr[50];
byte outStr[255];
int port, channel, freq;
#define MSG_BUFFER_SIZE (150)
char msg[MSG_BUFFER_SIZE] = "{\"t\":\"%.2f\",\"h\":\"%.2f\"}";
char pesan[150];
const sRFM_pins RFM_pins = {
  .CS = 5,
  .RST = 0,
  .DIO0 = 27,
  .DIO1 = 2,
};
void setup() {
  Serial.begin(9600);
  delay(2000);
  if (!lora.init()) {
    Serial.println("RFM95 not detected");
    delay(5000);
    return;
  }
  dht.begin();
  lora.setDeviceClass(CLASS_C);
  lora.setDataRate(SF7BW125);
  lora.setFramePortTx(5);
  lora.setChannel(MULTI);
  lora.setTxPower(15);
  lora.setNwkSKey(nwkSKey);
  lora.setAppSKey(appSKey);
  lora.setDevAddr(devAddr);
}
void loop() {
  if (millis() - previousMillis > interval) {
    previousMillis = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      h = 0;
      t = 0;
    }
    sprintf(myStr, msg, t, h );
    Serial.print("Sending: ");
    Serial.println(myStr);
    lora.sendUplink(myStr, strlen(myStr), 0);
    port = lora.getFramePortTx();
    channel = lora.getChannel();
    freq = lora.getChannelFreq(channel);
    Serial.print(F("fport: ")); Serial.print(port);
    Serial.print(" ");
    Serial.print(F("Ch: ")); Serial.print(channel);
    Serial.print(" ");
    Serial.print(F("Freq: ")); Serial.print(freq);
    Serial.println(" ");
  }
  lora.update();
}

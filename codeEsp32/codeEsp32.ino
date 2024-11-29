#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//Wifi name
#define WLAN_SSID       "wifi"
//Wifi password
#define WLAN_PASS       "03062003"

//setup Adafruit
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
//fill your username                   
#define AIO_USERNAME    "VuongAdon1s"
//fill your key
// comment to push to git
// #define AIO_KEY         ""

//setup MQTT
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//setup publish
Adafruit_MQTT_Publish TEMP_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/TEMPERTURE");

// //setup subcribe
Adafruit_MQTT_Subscribe TEMP_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/TEMPERTURE", MQTT_QOS_1);

String parts[2];
void devideString(String inputString)
{
  int numberOfParts = 0; // Số lượng phần được tách
  int startPos = 0; // Vị trí bắt đầu tìm ký tự phân cách

  for (int i = 0; i < inputString.length(); i++) {
    if (inputString.charAt(i) == ':') {
      parts[numberOfParts] = inputString.substring(startPos, i);
      startPos = i + 1;
      numberOfParts++;
    }
  }
  // Xử lý phần cuối cùng
  if (startPos < inputString.length()) {
    parts[numberOfParts] = inputString.substring(startPos);
    numberOfParts++; 
  }

  // // Hiển thị kết quả
  // for (int i = 0; i < numberOfParts; i++) {
  //   Serial.println(parts[i]);
  // }

}

void erasePart()
{
  parts[0] = "";
  parts[1] = "";
  parts[2] = "";
  parts[3] = "";
  parts[4] = "";
}

void setup() {
  // put your setup code here, to run once:
  //set pin 2,5 as OUTPUT
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  //set busy pin HIGH
  digitalWrite(5, LOW);

  Serial.begin(115200);

  //connect Wifi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //subscribe light feed
  // light_sub.setCallback(lightcallback);
  mqtt.subscribe(&TEMPERTURE);

  //connect MQTT
  while (mqtt.connect() != 0) { 
    mqtt.disconnect();
    delay(500);
  }

  //finish setup, set busy pin LOW
  digitalWrite(5, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  //receive packet
  mqtt.processPackets(1);
  
  //read serial
  if(Serial.available()){
    char msg = Serial.read();

    String receivedString = ""; // Khởi tạo chuỗi để lưu trữ dữ liệu
    receivedString += msg;
    while (Serial.available()) { // Kiểm tra nếu có dữ liệu trong cổng UART
      char incomingByte = Serial.read(); // Đọc dữ liệu từ cổng UART
      receivedString += incomingByte; // Nối dữ liệu vào chuỗi
      delay(2); // Đợi để nhận các byte tiếp theo (tùy thuộc vào tốc độ baudrate)
    }
    devideString(receivedString);
    if(parts[0] == "!TEMP")
    {
      int messageTemp = parts[1].toInt();
      TEMP_pub.publish(messageTemp);
    }
    erasePart();
  }
  delay(10);
}
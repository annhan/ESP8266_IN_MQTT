#include "Khaibao.h"
#include <PubSubClient.h>
//#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPUpdateServer.h>

//SoftwareSerial swSer(14, 12, false, 1024);

#define status_led  0  // vercu la 4 - 0
#define IN1  13  // Ver cu  la 16 - 13
#define IN2  14
#define IN3  12
#define OUT1  15  // Ver cu  la 16
#define OUT2  2
#define OUT3  4
WiFiClient client1;
PubSubClient clientmqtt(client1);
//#############################################
uint16_t CAPTURE_BUFFER_SIZE = 1400;
//*************************
//  MQTT ******************
#define mqtt_server "m10.cloudmqtt.com" //"mhome-nhamau.ddns.net" //"m10.cloudmqtt.com"
char mqtt_topic[21];
#define mqtt_user "odlnnhnh"
#define mqtt_pwd "SgsHtUCxh34o"
const uint16_t mqtt_port = 18159; // 1883; //12535; //1883
//****************************

//17317D2C 25442B250D 24 1731 11 02 23 01A3 0CD4 1D63 71 23 020D 0C30 1A71 2E FAFB

// 02 là có 2 dữ liệu, 1 dữ liệu gồm 8 byte gồm 
//byte đầu : là mã hóa kiểu cảm biến
//2 byte tiếp theo là Tab ID
// 4 byte tiếp là data
//byte cuối là chốt dữ liệu

// 1731 là ID cua bộ chính,11 là 17 byte, 02 la stt tab,23 là nhiet do,doam, 020D là giá trị N,0C30 là giá trị Độ ẩm,

//23 là nhiet độ,độ ẩm
//28 là cảm biến áp suatá




// 0A757E2C 254401250D 15 0A75 07 007D0163 080810 004E FD79 tab là 15 (Hướng gió,tốc độ mưa),
// 4,5 byte, 1byte tab,2byte rssid,1 byte status ,4 byte dữ liệu 2 byte đầu là lượng mua =2byte/30 2 byte sau la muc gio = 2byte/600
//             9        10,11         12           13,14,15,16

//0A817E2C 254848250D 07 0A82 00 13AE 07 0A81 00 1415 FD09  tab là 07 là độ ẩm
//0A82 là RRS ID  , 00 là chua dùng , 13AE là data ->dec/100 là ra % ,

//0A817E2C 254F4F250D 06 0A82 00 0872 06 0A81 00086B FCFA  tab 06 là nhiệt độ
//0A82 là RRS ID,00 là chưa dùng, 0872 là là dữ liệu ->Dec/100laf ra nhiệt độ, 

//0A8A7B2C 255656250D 02 0A8A 00 0CDF FD7B  tab 02 là lượng Pin
// 0A8A là RRS ID, 00laf OK , 0CDF là lượng pin mV

//0A8A7A2C 254401250D 1F 0A8A 04 01010188 FE21  id 1F là CO2
// 0A8A là RRS ID, 04 là 4 byte phía sau, 0188 là data ppm (lượng CO2)

//1000002C 254444250D 24 1000 09 01 23 0078 0988 1161 16 FD2E
//0078 là TAGID, 0988 là nhiệt độ,1161 là độ ẩm, 16 là RSSI
boolean html_khobot=false;
String str_html_khobot="";
#define GioMua 21
#define luongpin 2
#define Nhietdo 6
#define Doam 7
#define tab_main_data1 36
//15 huong gio do mua - DEC 21
//02 điện áp Pin  - DEC 2
//06 nhiet độ - DEC 6
//07 độ ẩm  - DEC 7
 /*struct http_data_rep {
        byte by1;
        byte by2;
        byte by3;
        byte by4;
        byte by5;
        byte by6;
        byte by7;
        byte by8;
        byte by9;
        byte by10;
        byte by11;
        byte by12;
        byte by13;
        byte by14;
        byte by15;
        byte by16;
        byte by17;
        byte by18;
        byte by19;
        byte by20;
        byte by21;
        byte by22;
        byte by23;
        byte by24;
        byte by25;
        byte by26;
        byte by27;
        byte by28;
        byte by29;
        byte by30;
        byte by31;
        byte by32;
        byte by33;
        byte by34;
        byte by35;
        
} http_data_struc;*/
 //const int BUFFER_SIZE_JSON = 2000;
// StaticJsonBuffer<BUFFER_SIZE_JSON> jsonBuffer;
//JsonObject& root = jsonBuffer.parseObject(json_sensor);
IPAddress ip10;
IPAddress gateway10;
IPAddress subnet10;
IPAddress DNS(8,8,8,8);

const char* update_path = "/firmware";
const char* update_username = "mhome";
const char* update_password = "fibaro";
const int buffer_size = 200;


WiFiClient client;
ESP8266WebServer server(4999);
ESP8266HTTPUpdateServer httpUpdater;
#define WIFI_CONF_FORMAT {0, 0, 0, 1}
#define WIFI_CONF_START 0
#define NAME_PREF "GSM-"
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
byte x=0;
int i=0;
byte gui=0;
byte gui1=0;
byte gui2=0;
int cho=0;
unsigned long previous=0;
unsigned long timeout=0;
unsigned long timebegin=0;
unsigned long timeled=0;
//char *ptr1=0;
int answer;
char aux_string[160];
char buffer[buffer_size+1];          // Data from UART
char response[buffer_size+1];
byte xbuff=0;

char uart_tam='\0';
//char c1=0;
char phone_number[13];

String noidung="123456789012345678901234567890123456789012345678901234567890123456789012345678904444444";
String manapthe;
String noidungkiemtratk;
String network_html;
String ipStr="Check Connect";
String SerialHC2="Check Connect";
String manap;

byte status_failmang=0;
//byte _resetketnoi=0;
byte statusmang=0;
byte statusnaptk=0;

char sdt_new[15];

unsigned long timer_gio=0;
int thoigian_gio=0;
long sotien;
boolean da_kttk=false;

byte guitinnhan=0;
//  byte ip1[4];
//  byte gateway[4];
//  byte subnet[4];

//0 den 419 byte


struct WiFiConfStruct {
  uint8_t format[4];
  char sta_ssid[32];
  char sta_pwd[64];
  char sta_ip[15];
  char sta_gateway[15];
  char sta_subnet[15];
  char sta_iphc2[20];
  char sta_passhc[100];
  char sta_SDT1[15];
  char sta_SDT2[15];
  char sta_SDT3[15];
  char sta_SDT4[15];
  char sta_global1[20];
  char sta_global2[20];
  char sta_language[3];
  char sta_manap[10];
  char sta_makttk[10];
  char sta_mqtt_address[64];
  uint16_t sta_mqtt_port;
  char sta_mqtt_user[64];
  char sta_mqtt_pass[64];
  char sta_mqtt_topic[64];
  char sta_DHCP[4];
  char module_id[32];
} WiFiConf = {
  WIFI_CONF_FORMAT,
  "mhome1",
  "nangniutoam",
  "192.168.99.120",
  "192.168.99.1",
  "255.255.255.0",
  "iphc2",
  "passhc",
  "x",
  "x",
  "x",
  "x",
  "global1",
  "global2",
  "1",
  "*100*",
  "*101#",
  "mhome-nhamau.ddns.net",
  1883,
  "x",
  "x",
  "IN_MQTT",
  ""
};
void SetVariHC(String vari,String giatri);
void getHC();
void goidt();
void kttkcusd();
void receive_uart();
void init_SIM900A();
//void erase_buffer();
int sendAT(char* ATcommand, char* expected_answer, unsigned int timeout);
//int sendATString(String ATcommand, char* expected_answer, unsigned int timeout) ;
void power_on();
void send_SMS(String noidungsms="");
void kttk(String nd);

void setup() {
  Serial.begin(115200);
 // unsigned int bientam=62000;
//  Serial.println(bientam);
 // Serial.println(sizeof(bientam));
  
  pinMode(IN3, INPUT);
  pinMode(IN2, INPUT);
  pinMode(IN1, INPUT);
  pinMode(status_led, OUTPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  
  digitalWrite(status_led,LOW);
  digitalWrite(OUT1,LOW);
  digitalWrite(OUT2,LOW);
  digitalWrite(OUT3,LOW);
  digitalWrite(5,LOW);
  //delay(5000);
  EEPROM.begin(1024);
  delay(10);
//read_sensor_eeprom();
  WiFi.mode(WIFI_AP_STA);
  if (!loadWiFiConf()) {
    resetModuleId();
    saveWiFiConf();
  }
  scanWiFi();
  connect_wifi();
  statusmang=waitConnected();
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(status_led,LOW);
   WiFi.softAP("GSM mHome","88888888",1,1);
   delay(1);   
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("GSM mHome","88888888");
  }
  WiFi.macAddress(macAddr);
  httpUpdater.setup(&server, update_path, update_username, update_password);
  setupWeb();
  setupWiFiConf();  
  server.begin();
  //MDNS.addService("http", "tcp", 4999);
  timebegin=millis();
   getHC();
  //power_on();
  //init_SIM900A();
  timeled = millis();
  timer_gio=timeled;
   //       manap=WiFiConf.sta_manap;
   //       manap.trim();
 // guitinnhan=3; //mac dinh bang 3 để kiểm tra tài khang
// Serial.println("Connect MQTT");
  clientmqtt.setServer(WiFiConf.sta_mqtt_address, WiFiConf.sta_mqtt_port);
  clientmqtt.setCallback(callback);
  lastReconnectAttempt = 0;
  gui = (digitalRead(IN1)==0)?0:1 ;
  gui1 = (digitalRead(IN3)==0)?0:1 ;
  gui2 = (digitalRead(IN2)==0)?0:1 ;
  Serial.println(gui);
  Serial.println(gui1);
  Serial.println(gui2);
  send_MQTT_IN();
}
void loop() {

  server.handleClient();
  switch (WiFi.status())
  {
    case WL_CONNECTED:
              if (statusmang==0){digitalWrite(status_led, LOW); 
              statusmang=1;
              cho=0;
             // Serial.println("NNNN");
              WiFi.softAP("GSM mHome","88888888",1,1);} 
                //MQTT
              if (!clientmqtt.connected()) {
                    long now = millis();
                    if (now - lastReconnectAttempt > 5000) {
                      lastReconnectAttempt = now;
                      // Attempt to reconnect
                      if (reconnect()) {
                        lastReconnectAttempt = 0;
                      }
                    }
                  } else {
                    clientmqtt.loop();
              }
              //////////////////////    
              break;
    default:
          if (statusmang!=0){ statusmang=0;reset_esp=0;timeled = millis();} 
          if (cho>=50){
            connect_wifi();
            cho=0;
            reset_esp=reset_esp+1;
          }
          else
          {
                  if ( (unsigned long) (millis() - timeled) > 500 ){
                          if ( digitalRead(status_led) == LOW )digitalWrite(status_led, HIGH); 
                          else digitalWrite(status_led, LOW );
                          timeled = millis();
                          cho++;
                  }
          }
          if (reset_esp>5){ESP.reset();} 
          break;
  }

  /*receive_uart();
  
  switch (guitinnhan){
    case 1:
          guitinnhan=0;
          send_SMS(noidung);
          break;
    case 2:
          guitinnhan=0;
          goidt();
          break;
    case 3:
          guitinnhan=0;
          kttkcusd();
          break;
    case 5:
          guitinnhan=0;
          manap=manap+manapthe;
          manap=manap+"#";
          kttk(manap);
          break;
    case 6:
          guitinnhan=3;
          break;
    case 7:
          guitinnhan=0;
          send_SMS1(noidung);
          break;
    case 8:
          guitinnhan=0;
          goidt2();
          break;
    default:
          break;
  }
  switch (dtmf_sim){
    case 0:
          dtmf_sim=10;
          break;
    case 1:
          dtmf_sim=10;
          break;
    case 2:
          dtmf_sim=10;
          break;
    case 3:
          dtmf_sim=10;
          break;
    case 4:
          dtmf_sim=10;
          break;
    case 5:
          dtmf_sim=10;
          break;
    case 6:
          dtmf_sim=10;
          break;
    case 7:
          dtmf_sim=10;
          break;
    case 8:
          dtmf_sim=10;
          break;
    case 9:
          dtmf_sim=10;
          break;
  }
  if (html_khobot){
    
    str_html_khobot.trim();
    html_khobot=false;
   // Serial.println(str_html_khobot);
    int str_len = str_html_khobot.length() + 1;
    char char_array[str_len];
    //send_back_server(str_html_khobot);
    update_fota(str_html_khobot);
    str_html_khobot.toCharArray(char_array, str_len);
    str_html_khobot="";
    tachsohex(char_array);

  }
  
  for (int k=0;k<15;k++){
        boolean tieptuc_sms=sosanh_cambien(k);
        if (tieptuc_sms){delay(5000);}
  }*/
  if(digitalRead(IN1)==0){if (gui==1){delay(50);if(digitalRead(IN1)==0){
    Serial.println("IN1");
    gui=0;
    thoigian_gio=0;
    send_MQTT_IN();
   // goidt();gui=1;digitalWrite(OUT3,HIGH); String tinnhan="Alarm 1 OPEN";send_SMS(tinnhan);}
  }}}
  else if(digitalRead(IN1)==1){if (gui==0){delay(50);if(digitalRead(IN1)==1){
  Serial.println("IN1=1");
    gui=1;
    thoigian_gio=0;
    send_MQTT_IN();
  }}}
  if(digitalRead(IN2)==0){if (gui2==1){delay(50);if(digitalRead(IN2)==0){
    Serial.println("IN2");
    gui2=0;
    thoigian_gio=0;
    send_MQTT_IN();
    //goidt();gui1=1;digitalWrite(OUT3,HIGH);String tinnhan="Alarm 2 OPEN";send_SMS(tinnhan);
    }}}  
  else if(digitalRead(IN2)==1){if (gui2==0){delay(50);if(digitalRead(IN2)==1){
    Serial.println("IN2=1");
    gui2=1;
    thoigian_gio=0;
    send_MQTT_IN();
    }}}
  if(digitalRead(IN3)==0){if (gui1==1){delay(50);if(digitalRead(IN3)==0){
    Serial.println("IN3");
    gui1=0;
    thoigian_gio=0;
    send_MQTT_IN();
    //goidt();digitalWrite(OUT3,HIGH);String tinnhan="Alarm 3 OPEN";send_SMS(tinnhan);
    }}}
  else if(digitalRead(IN3)==1){if (gui1==0){delay(50);if(digitalRead(IN3)==1){
    Serial.println("IN3=1");
    gui1=1;
    thoigian_gio=0;
    send_MQTT_IN();
    }}} 
 // Serial.println("TEST");
  if ( (unsigned long) (millis() - timer_gio) > 1000 ){
                         /* if (thoigian_gio % 2 ==0){
                                if (statusmang==1){
                                String trave="{";
                                for (int i=0;i<15;i++){
                                  if (i<14){
                                  trave +="\"S" + String(i)+"\":{\"N\":\"" + String(*((unsigned int*)&SensorStruct + (i*8)));
                                   trave +="\",\"V\":[" + String(*((float*)&SensorStruct + ((i*8)+2))) + "," + String(*((float*)&SensorStruct + ((i*8)+3))) + ","+ String(*((float*)&SensorStruct + ((i*8)+4))) + "," + String(*((float*)&SensorStruct + ((i*8)+5))) + ","+ String(*((float*)&SensorStruct + ((i*8)+6))) + "," + String(*((float*)&SensorStruct + ((i*8)+7))) + "," + String(*((unsigned int*)&SensorStruct + ((i*8)+1))) + "]},";
                                  }
                                  else{
                                  trave +="\"S" + String(i)+"\":{\"N\":\"" + String(*((unsigned int*)&SensorStruct + (i*8)));
                                  trave += "\",\"V\":[" + String(*((float*)&SensorStruct + ((i*8)+2))) + "," + String(*((float*)&SensorStruct + ((i*8)+3))) + ","+ String(*((float*)&SensorStruct + ((i*8)+4))) + "," + String(*((float*)&SensorStruct + ((i*8)+5))) + ","+ String(*((float*)&SensorStruct + ((i*8)+6))) + "," + String(*((float*)&SensorStruct + ((i*8)+7))) + "," + String(*((unsigned int*)&SensorStruct + ((i*8)+1))) + "]}";
                                  }
                                }
                                trave += "}";
                                int chieudai_mqtt=trave.length();
                                //Serial.println(chieudai_mqtt);
                                char msg[chieudai_mqtt+1];
                            
                                trave.toCharArray(msg, sizeof(msg));
                                
                               // snprintf (msg, 100, "{\"ip\":\"%i.%i.%i.%i\",\"command\":\"SendIR\",\"para\":{\"type\":\"ML\",\"remote\":\"Daikin\",\"button\":\"ON\"}}",ip[0],ip[1],ip[2],ip[3]);
                               // Serial.println(msg);
                                clientmqtt.publish(mqtt_topic, msg);
                                }
                          }*/
                          timer_gio = millis();
                          thoigian_gio++;
                          if (thoigian_gio > 20){ thoigian_gio=0;send_MQTT_IN();}
                          /*Serial.println("AT");
                          if (da_kttk){ if (sotien<15000 && sotien>1000){da_kttk=false; noidung="Chu y : So TK con "; noidung = noidung + String(sotien);noidung = noidung + "d. De nap soan cu phap NAP:mathe. gui den sdt nay";guitinnhan=1;}};    */                      
  }
}

//******************************************************************************************************


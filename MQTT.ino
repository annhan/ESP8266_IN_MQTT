const int BUFFER_SIZE = 1200;
const char* send_cmd = "SendIR";
const char* lean_cmd = "LeanIR";
const char* get_cmd = "Get_infor";


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);
    if (!processJson(message)) {
    return;
  }
}
boolean reconnect() {
  Serial.print("Reconnecting : ");
  if (statusmang==1){
        if (WiFiConf.sta_mqtt_user[0]!='x'){  
                Serial.println("Co User");
                if (clientmqtt.connect("arduinoClient",WiFiConf.sta_mqtt_user, WiFiConf.sta_mqtt_pass)) {
                 // if (clientmqtt.connect("arduinoClient")) {
                  clientmqtt.publish(WiFiConf.sta_mqtt_topic,"Reconnect");
                  clientmqtt.subscribe(WiFiConf.sta_mqtt_topic);
                }
        }
        else
        {        Serial.println("Khong User");
                if (clientmqtt.connect("arduinoClient")) {
                 // if (clientmqtt.connect("arduinoClient")) {
                  clientmqtt.publish(WiFiConf.sta_mqtt_topic,"Reconnect");
                  clientmqtt.subscribe(WiFiConf.sta_mqtt_topic);
                }
        }
  }
  return clientmqtt.connected();
}
bool processJson(char* message) {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);
  if (!root.success()) {
    //Serial.println("parseObject() failed");
  return false;
  } 
  if (root.containsKey("command")) {
  }
  return true;
}

void send_MQTT_IN(){
      String trave="{";
    trave +="\"IN1\":\"" + String(gui);
    trave +="\",\"IN2\":\"" + String(gui1);
    trave +="\",\"IN3\":\"" + String(gui2);
                                trave += "\"}";
                                int chieudai_mqtt=trave.length();
                                //Serial.println(chieudai_mqtt);
                                char msg[chieudai_mqtt+1];
                            
                                trave.toCharArray(msg, sizeof(msg));
                                
                               // snprintf (msg, 100, "{\"ip\":\"%i.%i.%i.%i\",\"command\":\"SendIR\",\"para\":{\"type\":\"ML\",\"remote\":\"Daikin\",\"button\":\"ON\"}}",ip[0],ip[1],ip[2],ip[3]);
                               // Serial.println(msg);
                               Serial.println(msg);
                                clientmqtt.publish(WiFiConf.sta_mqtt_topic, msg);
}


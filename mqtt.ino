void MQTT_setup(){

  // Callbacks
  MQTT_client.onConnect(MQTT_connect_callback);
  MQTT_client.onDisconnect(MQTT_disconnect_callback);
  MQTT_client.onSubscribe(MQTT_subscribe_callback);
  MQTT_client.onUnsubscribe(MQTT_unsubscribe_callback);
  MQTT_client.onMessage(MQTT_message_callback);
  MQTT_client.onPublish(MQTT_publish_callback);

  // Settings
  MQTT_client.setServer(MQTT_BROKER_ADDRESS, MQTT_PORT);
  MQTT_client.setCredentials(MQTT_USERNAME, MQTT_PASSWORD);
  MQTT_client.setWill(MQTT_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, MQTT_LAST_WILL);
}

void MQTT_connect() {
  Serial.println("MQTT connecting...");

  MQTT_reconnect_timer.detach();
  MQTT_client.connect();
}

void MQTT_connect_callback(bool sessionPresent) {
  Serial.println("MQTT connected");

  // Subscribing to command topics
  MQTT_client.subscribe(MQTT_COMMAND_TOPIC, MQTT_QOS);

  // Update status
  MQTT_publish_light_state();
}

void MQTT_disconnect_callback(AsyncMqttClientDisconnectReason reason) {
  Serial.println("MQTT disconnected");

  if (WiFi.isConnected()) {
    MQTT_reconnect_timer.attach(2, MQTT_connect);
  }
}

void MQTT_subscribe_callback(uint16_t packetId, uint8_t qos) {
  Serial.println("MQTT subscribed successfully");
}

void MQTT_unsubscribe_callback(uint16_t packetId) {
  Serial.println("MQTT unsubscribed successfully");
}

void MQTT_message_callback(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {

  Serial.print("MQTTT message received: ");
  Serial.print("  topic: ");
  Serial.print(topic);
  Serial.print("  payload: ");
  Serial.print(payload);
  Serial.print("  qos: ");
  Serial.print(properties.qos);
  Serial.print("  dup: ");
  Serial.print(properties.dup);
  Serial.print("  retain: ");
  Serial.print(properties.retain);
  Serial.print("  len: ");
  Serial.print(len);
  Serial.print("  index: ");
  Serial.print(index);
  Serial.print("  total: ");
  Serial.println(total);

  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(payload);

  if (root.containsKey("state")) {
    const char* light_state_temp = root["state"];
    light_state = (char*) light_state_temp;
  }
  if (root.containsKey("brightness")) {
    light_brightness = root["brightness"];
  }
  if (root.containsKey("color")) {
    light_r = root["color"]["r"];
    light_g = root["color"]["g"];
    light_b = root["color"]["b"];
  }

  if(strcmp(light_state,"ON") == 0){
    sl_my92x1_duty(light_r,light_g,light_b,light_brightness,light_brightness);
  }
  else {
    sl_my92x1_duty(0,0,0,0,0);
  }

  MQTT_publish_light_state();
}

void MQTT_publish_callback(uint16_t packetId) {
  Serial.println("MQTT published successfully");
}

void MQTT_publish_light_state(){
  StaticJsonBuffer<200> jsonBuffer;
  
  JsonObject& root = jsonBuffer.createObject();
  root["state"] = light_state;
  root["brightness"] = light_brightness;
  
  JsonObject& color = root.createNestedObject("color");
  color["r"] = light_r;
  color["g"] = light_g;
  color["b"] = light_b;

  root.prettyPrintTo(Serial);
  Serial.println("");
  
  char payload[200];
  root.printTo(payload, root.measureLength() + 1);
  MQTT_client.publish(MQTT_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, payload);
}


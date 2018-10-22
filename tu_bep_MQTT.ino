#include "tu_bep_MQTT.h"

void setupLedbegin(){
    //lay du lieu thong tin den led
    ECHOLN("Reading EEPROM LED RED");
    String redStr = "";
    for (int i = EEPROM_WIFI_LED_RED_START; i < EEPROM_WIFI_LED_RED_END; ++i){
        redStr += char(EEPROM.read(i));
    }
    ECHO("LED RED: ");
    ECHOLN(redStr);
    ECHOLN("Reading EEPROM LED GREEN");
    String greenStr = "";
    for (int i = EEPROM_WIFI_LED_GREEN_START; i < EEPROM_WIFI_LED_GREEN_END; ++i){
        greenStr += char(EEPROM.read(i));
    }
    ECHO("LED GREEN: ");
    ECHOLN(greenStr);
    ECHOLN("Reading EEPROM LED BLUE");
    String blueStr = "";
    for (int i = EEPROM_WIFI_LED_BLUE_START; i < EEPROM_WIFI_LED_BLUE_END; ++i){
        blueStr += char(EEPROM.read(i));
    }
    ECHO("LED BLUE: ");
    ECHOLN(blueStr);
    red_before = atoi(redStr.c_str());
    green_before = atoi(greenStr.c_str());
    blue_before = atoi(blueStr.c_str());
    analogWrite(PIN_LED_RED, red_before);
    analogWrite(PIN_LED_GREEN, green_before);
    analogWrite(PIN_LED_BLUE, blue_before);   
}
void SetupNetwork() {
    //ket noi vao mang wifi 
    ECHO("Ket noi mang wifi: ");
    ECHOLN(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        ECHO(".");
    }
    ECHOLN("");
    ECHOLN("Ket noi thanh cong!");
    ECHO("Dia chi IP: ");
    ECHOLN(WiFi.localIP());
}

void goUpMotor(){
    ECHOLN("UP");
    digitalWrite(PIN_DIR_MOTOR, HIGH);
    tickerSetMotor.start();
}

void goDownMotor(){
    ECHOLN("DOWN");
    digitalWrite(PIN_DIR_MOTOR, LOW);
    tickerSetMotor.start();
}

void stopMotor(){
    ECHOLN("STOP");
    tickerSetMotor.stop();
}

void setPulMotor(){
    digitalWrite(PIN_PUL_MOTOR, !digitalRead(PIN_PUL_MOTOR));
}

void ControlLed(){
    StaticJsonBuffer<RESPONSE_LENGTH> jsonBuffer;
    JsonObject& rootData = jsonBuffer.parseObject(datareceive);
    ECHOLN("--------------");
    
    if (rootData.success()) {
        String powerStr = rootData["power"];
        String redStr = rootData["red"];
        String greenStr = rootData["green"];
        String blueStr = rootData["blue"];
        float out_led_red, out_led_green, out_led_blue;

        red_after = atoi(redStr.c_str());
        green_after = atoi(greenStr.c_str());
        blue_after = atoi(blueStr.c_str());

        if(powerStr == "off"){
            for(int i = 1; i<=255; i++){
                out_led_red = (float)red_before + (((float)0 - (float)red_before)/255)*i;
                out_led_red = abs(out_led_red);
                analogWrite(PIN_LED_RED, uint8_t(out_led_red));
                out_led_green = (float)green_before + (((float)0 - (float)green_before)/255)*i;
                out_led_green = abs(out_led_green);
                analogWrite(PIN_LED_GREEN, uint8_t(out_led_green));
                out_led_blue = (float)blue_before + (((float)0 - (float)blue_before)/255)*i;
                out_led_blue = abs(out_led_blue);
                analogWrite(PIN_LED_BLUE, uint8_t(out_led_blue));
                delay(10);
                
            }
            red_before = 0;
            green_before = 0;
            blue_before = 0;                
            return;
        }        
        
        for(int i = 1; i<=255; i++){
            out_led_red = (float)red_before + (((float)red_after - (float)red_before)/255)*i;
            out_led_red = abs(out_led_red);
            analogWrite(PIN_LED_RED, uint8_t(out_led_red));
            out_led_green = (float)green_before + (((float)green_after - (float)green_before)/255)*i;
            out_led_green = abs(out_led_green);
            analogWrite(PIN_LED_GREEN, uint8_t(out_led_green));
            out_led_blue = (float)blue_before + (((float)blue_after - (float)blue_before)/255)*i;
            out_led_blue = abs(out_led_blue);
            analogWrite(PIN_LED_BLUE, uint8_t(out_led_blue));
            delay(10);
        }

        red_before = red_after;
        green_before = green_after;
        blue_before = blue_after;
        
        ECHO("RED: ");
        ECHOLN(uint8_t(out_led_red));
        ECHO("GREEN: ");
        ECHOLN(uint8_t(out_led_green));
        ECHO("BLUE: ");
        ECHOLN(uint8_t(out_led_blue));
        //clear data EEPROM Led RGB
        for (int i = EEPROM_WIFI_LED_RED_START; i <= EEPROM_WIFI_LED_BLUE_END; ++i){
            EEPROM.write(i, 0);
        }
        ECHOLN("writing eeprom LED RED:");
        ECHO("Wrote: ");
        for (int i = 0; i < redStr.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_LED_RED_START, redStr[i]);
            ECHO(redStr[i]);
        }
        ECHOLN("");
        ECHOLN("writing eeprom LED GREEN:");
        ECHO("Wrote: ");
        for (int i = 0; i < greenStr.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_LED_GREEN_START, greenStr[i]);
            ECHO(greenStr[i]);
        }
        ECHOLN("");
        ECHOLN("writing eeprom LED BLUE:"); 
        ECHO("Wrote: ");
        for (int i = 0; i < blueStr.length(); ++i){
            EEPROM.write(i+EEPROM_WIFI_LED_BLUE_START, blueStr[i]);             
            ECHO(blueStr[i]);
        }
        ECHOLN("");
        EEPROM.commit();
        ECHOLN("Done writing!");
        return;
    }
    ECHOLN("Wrong data!!!"); 
}


void setup() {
    pinMode(LED_TEST, OUTPUT);
    pinMode(LED_TEST_MOTOR, OUTPUT);
    pinMode(PIN_CONFIG, INPUT);
    pinMode(PIN_PUL_MOTOR, OUTPUT);
    pinMode(PIN_DIR_MOTOR, OUTPUT);
    pinMode(PIN_ENCODER_MOTOR, INPUT_PULLUP);
    analogWriteRange(255);      //max gia tri PWM la 255
    Serial.begin(115200);
    EEPROM.begin(512);
    //external interrupt doc tin hieu encoder
    //attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_MOTOR), handleInterruptMotor, CHANGE);
    setupLedbegin();
    SetupNetwork();  
    client.setServer(MQTT_SERVER, MQTT_PORT);   //thiet lap Broker server 
    client.setCallback(mqtt_callback);      //dang ky ham callback
  
}

void loop() {
    //kiem tra ket noi vao mang wifi
    if(!WiFi.status()){
        SetupNetwork();
    }
    //kiem tra neu chua ket noi Broker thi tien hanh ket noi
    if (!client.connected()) {
        reconnect();
    }

    if(flagControlLed == true){
        ControlLed();
        flagControlLed = false;
    }
    switch(flagModeMotor){
        case MOTORUP:{
            goUpMotor();
            flagModeMotor = 0;
            break;
        }
        case MOTORDOWN:{
            goDownMotor();
            flagModeMotor = 0;
            break;
        }
        case MOTORSTOP:{
            stopMotor();
            flagModeMotor = 0;
            break;
        }
    }

    //goi ham loop de thu vien PubSubClient xu ly giao thuc MQTT 
    client.loop();
    tickerSetMotor.update();
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
    //goi ham connect() de ket noi toi Broker server da thiet lap
        if (client.connect("ESP8266Client")) {
            ECHOLN("connected");
        } else {
            ECHO("failed, rc=");
            ECHO(client.state());
            ECHOLN(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
    
    client.subscribe(COMMAND_1); 
    client.subscribe(COMMAND_2); 
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    datareceive = "";
    if(strcmp(COMMAND_1, topic) == 0){      //dieu khien motor
        ECHO("Da nhan duoc du lieu cua COMMAND_1: ");
        for (int i = 0; i < length; i++) {
            datareceive += (char)payload[i];            
        }
        ECHOLN(datareceive);
        if(datareceive == "UP"){
            flagModeMotor = 1;
        }else if(datareceive == "DOWN"){
            flagModeMotor = 2;
        }
        else if(datareceive == "STOP"){
            flagModeMotor = 3;
        }
        //client.publish(COMMAND_2, "done", true);
    }
    else if(strcmp(COMMAND_2, topic) == 0){     //dieu khien led
        ECHO("Da nhan duoc du lieu cua COMMAND_2: ");
        for (int i = 0; i < length; i++) {
            datareceive += (char)payload[i];
        }
        ECHOLN(datareceive);
        flagControlLed = true;   
    }     
}

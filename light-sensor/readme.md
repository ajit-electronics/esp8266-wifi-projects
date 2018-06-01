## Connection a light sensor to ESP8266 and displaying the readings easy-iot cloud 

### Setup the board
![Setting up Board](assests/board_setup.JPG "Setting up Board")

Todo: fritzing diagram

### Connecting to wifi 
```cpp
WiFi.mode(WIFI_STA);
    WiFi.begin("nereusiphone", "ajit1234");

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print("y");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
``` 
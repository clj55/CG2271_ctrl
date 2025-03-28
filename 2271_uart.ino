#define RXD2 16
#define TXD2 17
#define BLUE_LED 12
#define RED_LED 13
#define GREEN_LED 14
#define WHITE_LED 27
// Auxiliar variables to store the current output state
String output26State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int wait30 = 30000; // time to reconnect when connection is lost.


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Initialize the output variables as outputs
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);

  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);

  pinMode(WHITE_LED, OUTPUT);
  digitalWrite(WHITE_LED, LOW);


  //To restrict to one controller
  bd_addr_t controller_addr;
  // Parse human-readable Bluetooth address.
  sscanf_bd_addr(controller_addr_string, controller_addr);
  // Notice that this address will be added in the Non-volatile-storage (NVS).
  // If the device reboots, the address will still be stored.
  uni_bt_allowlist_add_addr(controller_addr);
  // Finally, enable the allowlist.
  // Similar to the "add_addr", its value gets stored in the NVS.
  uni_bt_allowlist_set_enabled(true);


  // To get the BD Address (MAC address) call:
  const uint8_t* addr = BP32.localBdAddress();
  Serial.print("BD Address: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(addr[i], HEX);
    if (i < 5)
      Serial.print(":");
    else
      Serial.println();
  }

  BP32.setup(&onConnectedController, &onDisconnectedController);

}

void debug_packet(int8_t forward_speed, int8_t right_speed) {
    
    if(forward_speed < 0)
    {
      digitalWrite(BLUE_LED, HIGH);
      Serial.println("Forward");
    }

    if(forward_speed > 0)
    {
      digitalWrite(RED_LED, HIGH);
      Serial.println("Backward");
    }  
    if (right_speed > 0)
    {
      digitalWrite(GREEN_LED, HIGH);
      Serial.println("Right");
    }
    if(right_speed < 0)
    {
      digitalWrite(WHITE_LED, HIGH);
      Serial.println("Left");
    }

    if (right_speed == 0 && forward_speed == 0) { 
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(WHITE_LED, LOW);
    }

    Serial.print("Forward: ");
    Serial.print(forward_speed);
    Serial.print(" Right: ");
    Serial.println(right_speed);
}

void loop() {
  BP32.update();

  Packet packet;
  packet.start = 0xFE;
  packet.end = 0xFF;

  // It is safe to always do this before using the controller API.
  // This guarantees that the controller is valid and connected.
  if (myController && myController->isConnected()) {
    if (myController->isGamepad()) {
      //processGamepad(myController);

      int8_t forward_speed =  myController->axisY() * 100 / 512;
      int8_t right_speed = myController->axisRX() * 100 / 512;
      //gamepad->buttons();

      packet.forward = forward_speed;
      packet.right = right_speed;

      Serial2.write((uint8_t*)&packet, sizeof(packet));
      delay(150);

      debug_packet(forward_speed, right_speed);

    } 
  }
}
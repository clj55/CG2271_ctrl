#include <Bluepad32.h>
#include <uni.h>

static const char * controller_addr_string = "98:B6:01:51:A7:04";

ControllerPtr myController = nullptr;


// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {

  if (myController == nullptr) {
    Serial.print("CALLBACK: Controller is connected, index=");
    myController = ctl;

    // Optional, once the gamepad is connected, request further info about the
    // gamepad.
    ControllerProperties properties = ctl->getProperties();
    char buf[80];
    sprintf(buf,
            "BTAddr: %02x:%02x:%02x:%02x:%02x:%02x, VID/PID: %04x:%04x, "
            "flags: 0x%02x",
            properties.btaddr[0], properties.btaddr[1], properties.btaddr[2],
            properties.btaddr[3], properties.btaddr[4], properties.btaddr[5],
            properties.vendor_id, properties.product_id, properties.flags);
    Serial.println(buf);
  }

}


void onDisconnectedController(ControllerPtr ctl) {
  bool foundGamepad = false;
  
  Serial.println();
  if (myController == ctl) {
    Serial.print("CALLBACK: Controller is disconnected");
    myController = nullptr;
    foundGamepad = true;
  }

  if (!foundGamepad) {
    Serial.println(
        "CALLBACK: Controller disconnected, but not myController");
  }
}


struct Packet {
    uint8_t start;
    int8_t forward;
    int8_t right;
    uint8_t end;
};


void processGamepad(ControllerPtr gamepad) {

  // Another way to query the buttons, is by calling buttons(), or
  // miscButtons() which return a bitmask.
  // Some gamepads also have DPAD, axis and more.
  char buf[256];
  snprintf(buf, sizeof(buf) - 1,
           "idx=%d, dpad: 0x%02x, buttons: 0x%04x, "
           "axis L: %4li, %4li, axis R: %4li, %4li, "
           "brake: %4ld, throttle: %4li, misc: 0x%02x, "
           "gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d, "
           "battery: %d",
           gamepad->index(),        // Gamepad Index
           gamepad->dpad(),         // DPad
           gamepad->buttons(),      // bitmask of pressed buttons
           gamepad->axisX(),        // (-511 - 512) left X Axis
           gamepad->axisY(),        // (-511 - 512) left Y axis
           gamepad->axisRX(),       // (-511 - 512) right X axis
           gamepad->axisRY(),       // (-511 - 512) right Y axis
           gamepad->brake(),        // (0 - 1023): brake button
           gamepad->throttle(),     // (0 - 1023): throttle (AKA gas) button
           gamepad->miscButtons(),  // bitmask of pressed "misc" buttons
           gamepad->gyroX(),        // Gyro X
           gamepad->gyroY(),        // Gyro Y
           gamepad->gyroZ(),        // Gyro Z
           gamepad->accelX(),       // Accelerometer X
           gamepad->accelY(),       // Accelerometer Y
           gamepad->accelZ(),       // Accelerometer Z
           gamepad->battery()       // 0=Unknown, 1=empty, 255=full
  );
  Serial.println(buf);

  


  // You can query the axis and other properties as well. See
  // Controller.h For all the available functions.
}
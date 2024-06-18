#include <LIDAR.h>
#include <ColorSensor.h>
#include <SwarmCommunication.h>
#include <FlightController.h>

// Initializing sensors 
LIDAR lidar;
ColorSensor colorSensor;
SwarmCommunication swarmComm;
FlightController flightController;

const int TARGET_HEIGHT = 15; // Target object height in cm
const String TARGET_COLOR = "green"; // Target color

void setup() {
  Serial.begin(115200);

  // Initializing sensors
  lidar.begin();
  colorSensor.begin();

  // Initializing communication
  swarmComm.begin();

  // Initializing flight controller
  flightController.begin();
}

void loop() {
  
  if (flightController.isFlying()) {
    // LIDAR scan
    int height = lidar.getHeight();

    // Checking for target 
    if (height == TARGET_HEIGHT) {
      // Measuring other dimensions 
      int width = lidar.getWidth();
      int depth = lidar.getDepth();

      // Condition satisfying statements
      if (width == TARGET_HEIGHT && depth == TARGET_HEIGHT) { 
        String detectedColor = colorSensor.getColor();
        if (detectedColor == TARGET_COLOR) {
          // Target found, notify other drones
          swarmComm.sendMessage("Target Found");
          // Stop this drone
          flightController.stop();
          return;
        }
      }
    }

    // If Target not Found
    flightController.continueSearch();
  } else {
    // Taking Other Drone's mESSAge
    String message = swarmComm.receiveMessage();
    if (message == "Target Found") {
      // Present Drone STOP
      flightController.stop();
    }
  }
}

// Inputs as classes with defined return value(Example CASES)
//MOCK CLASSES
class LIDAR {
public:
  void begin() {
    // LIDAR Work
    Serial.println("LIDAR initialized");
  }
  
  int getHeight() {
    // Return Height
    return 15; 
  }
  
  int getWidth() {
    // Return width 
    return 15; 
  }
  
  int getDepth() {
    // Return depth
    return 15; 
  }
};

class ColorSensor {
public:
  void begin() {
    // Color sensor On
    Serial.println("Color Sensor initialized");
  }
  
  String getColor() {
    return "green"; 
  }
};

class SwarmCommunication {
public:
  void begin() {
    // Communication Starts
    Serial.println("Swarm Communication initialized");
  }
  
  void sendMessage(String message) {
    // Message to other drones
    Serial.println("Sending message: " + message);
  }
  
  String receiveMessage() {
    // Recieving
    if (Serial.available()) {
      return Serial.readStringUntil('\n');
    }
    return "";
  }
};

class FlightController {
public:
  void begin() {
    //Flight Controller
    Serial.println("Flight Controller initialized");
  }
  
  bool isFlying() {
    // STATUS:FLYING
    return true; 
  }
  
  void continueSearch() {
    // Target Searching
    Serial.println("Continuing search...");
  }
  
  void stop() {
    // Stop the drone
    Serial.println("Stopping drone...");
  }
};

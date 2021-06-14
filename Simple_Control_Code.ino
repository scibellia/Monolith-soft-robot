
// This let's us use commands from the BLE (bluetooth) library
#include <ArduinoBLE.h>

BLEService MSoRoService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

//These assign the pins on the arduino
//Motor 3:
int in_1 = 12; //This means pin 12 will be one of the outputs for Motor 3
int in_2 = 11; //This means pin 11 will be the other output for Motor 3
//Motor 2:
int in_3 = 10; //pin 10 is one of the outputs for Motor 2
int in_4 = 9; //pin 9 is other output for Motor 2
//Moror 1:
int in_5 = 7; //pin 7 is one of the outputs for motor 1
int in_6 = 6; //pin 6 is the other output for motor 1


//These are some constants for the timing
// 250 is a good number
const int wind_time = 250;
// 100 is a good number
const int unwind_time = 100;


void setup() {

    Serial.begin(9600);
      
    pinMode(in_1,OUTPUT);  //Logic pins are also set as output
    pinMode(in_2,OUTPUT);
    pinMode(in_3,OUTPUT);
    pinMode(in_4,OUTPUT);
    pinMode(in_5,OUTPUT);
    pinMode(in_6,OUTPUT);

    // begin initialization
    if (!BLE.begin()) {
      Serial.println("starting BLE failed!");

      while (1);
    }

    // set advertised local name and service UUID:
    BLE.setLocalName("MSoRo");
    BLE.setAdvertisedService(MSoRoService);

    // add the characteristic to the service
    MSoRoService.addCharacteristic(switchCharacteristic);

    // add service
    BLE.addService(MSoRoService);

    // set the initial value for the characeristic:
    switchCharacteristic.writeValue(0);

    // start advertising
    BLE.advertise();

    Serial.println("BLE MSoRo Peripheral");
    
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {

//============= Currently programmed states of the robot =========================================
//        input: 0 ---- Output: all motors are stopped
//        input: 1 ---- Output: motor 1 is driven cw
//        input: 2 ---- Output: motor 2 is driven cw
//        input: 3 ---- Output: motor 3 is driven cw
//        input: 4 ---- Output: motor 1 is driven ccw
//        input: 5 ---- Output: motor 2 is driven ccw
//        input: 6 ---- Output: motor 3 is driven ccw
//        input: 9 ---- Output: rotates contact surface
//        input: 10 (A) ---- Output: motor 1 and 2 are driven cw and then ccw to create an inching movement
//        input: 11 (B) ---- Output: Does the inching movement from input: A five times in a row.
//        input: 12 (C) ---- Output: contracts and retracts the motors for a rolling movement.
//        input: 13 (D) ---- Output: contracts and retracts the motors to undo the rolling movement of input: 12.
//        input: 14 (E) ---- Output: contracts all the motors and then releases all the motors.
//================================================================================================
    
    switch (switchCharacteristic.value()) {

      case 0:

        break_motor (in_5, in_6);
        break_motor (in_3, in_4);
        break_motor (in_1, in_2);
        
        break;

      case 1:

        Serial.println("Motor 1 cw");
        //this drives motor 1 for the wind time
        drive_motor_cw (in_5, in_6,wind_time);
        //After driving the motor it breaks it
        break_motor (in_5, in_6);   

        break;

      case 2:

        Serial.println(F("Motor 2 cw"));
        drive_motor_cw (in_3, in_4,wind_time);
        break_motor (in_3, in_4);

        break;

      case 3:

        Serial.println(F("Motor 3 cw"));
        drive_motor_cw (in_1, in_2,wind_time);
        break_motor (in_1, in_2);

        break;

      case 4:

        Serial.println(F("Motor 1 ccw"));
        drive_motor_ccw (in_5, in_6,unwind_time);
        break_motor (in_5, in_6);

        break;

      case 5:

       Serial.println(F("Motor 2 ccw"));
       drive_motor_ccw (in_3, in_4,unwind_time);
       break_motor (in_3, in_4);

        break;

      case 6:

       Serial.println(F("Motor 3 ccw"));
       drive_motor_ccw (in_1, in_2,unwind_time);
       break_motor (in_1, in_2);

        break;
        
      case 10: //The reason 10 was picked is that it equivalent to A in hexadecimal

       Serial.println(F("Motor 1 and 2 cw"));
       drive_motor_cw (in_5, in_6,0);   // Notice how the delay is 0 for the first function, this is beacuse both motors should start at the same time
       drive_motor_cw (in_3, in_4,1250); // The delay here is 1250 milliseconds that way both motors will run for that time and then they will both stop
       break_motor (in_5, in_6);
       break_motor (in_3, in_4);
       delay (100);
       Serial.println(F("Motor 1 and 2 ccw"));
       drive_motor_ccw (in_5, in_6,0);
       drive_motor_ccw (in_3, in_4,750);
       break_motor (in_5, in_6);
       break_motor (in_3, in_4);

        break;

      case 11:

       for (int i = 0; i < 5; i++)
       {
           Serial.println(F("Motor 1 and 2 cw"));
           drive_motor_cw (in_5, in_6,0);   // Notice how the delay is 0 for the first function, this is beacuse both motors should start at the same time
           drive_motor_cw (in_3, in_4,1250); // The delay here is 1250 milliseconds that way both motors will run for that time and then they will both stop
           break_motor (in_5, in_6);
           break_motor (in_3, in_4);
           delay(100);
           Serial.println(F("Motor 1 and 2 ccw"));
           drive_motor_ccw (in_5, in_6,0);
           drive_motor_ccw (in_3, in_4,750);
           break_motor (in_5, in_6);
           break_motor (in_3, in_4);
           delay(500);
       }

        break;

       case 12:

        Serial.println(F("roll"));
        drive_motor_cw (in_3, in_4,3000);
        break_motor (in_3, in_4);
        delay(400);
        drive_motor_cw (in_5, in_6,3000);
        break_motor (in_5, in_6);
        drive_motor_ccw (in_3, in_4,0);
        drive_motor_ccw (in_5, in_6,1500);
        break_motor (in_5, in_6);
        break_motor (in_3, in_4);

         break;

       case 13:

        Serial.println(F("undo roll"));
        drive_motor_cw (in_1, in_2,3000);
        break_motor (in_1, in_2);
        delay(400);
        drive_motor_cw (in_5, in_6,3000);
        break_motor (in_5, in_6);
        drive_motor_ccw (in_5, in_6,0);
        drive_motor_ccw (in_1, in_2,1500);
        break_motor (in_1, in_2);
        break_motor (in_5, in_6);

         break;

       case 14:

        Serial.println(F("All motors cw"));
        drive_motor_cw (in_5, in_6,0);   // Notice how the delay is 0 for the first function, this is beacuse both motors should start at the same time
        drive_motor_cw (in_1, in_2,0);
        drive_motor_cw (in_3, in_4,1500); // The delay here is 1500 milliseconds that way both motors will run for that time and then they will both stop
        break_motor (in_5, in_6);
        break_motor (in_1, in_2);
        break_motor (in_3, in_4);
        delay (100);
        Serial.println(F("All motors ccw"));
        drive_motor_ccw (in_5, in_6,0);
        drive_motor_ccw (in_1, in_2,0);
        drive_motor_ccw (in_3, in_4,800);
        break_motor (in_5, in_6);
        break_motor (in_1, in_2);
        break_motor (in_3, in_4);

         break;

       case 9:

        int temp_1 = in_5;
        int temp_2 = in_6;
        in_5 = in_1;
        in_6 = in_2;
        in_1 = in_3;
        in_2 = in_4;
        in_3 = temp_1;
        in_4 = temp_2;
         
         break;

        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    
  }
}


//Theese are the functions that drive and break(or stop) the motors

//Takes in the two output pins of the motors and drives the motor ccw
//The delay is the time until this function exits
void drive_motor_ccw (int M_1, int M_2, int wait){
    digitalWrite(M_1,HIGH);
    digitalWrite(M_2,LOW);
    delay(wait);
}

//Takes in the two output pins of the motors and drives the motor cw
//The delay is the time until this function 
void drive_motor_cw (int M_1, int M_2, int wait){
    digitalWrite(M_1,LOW);
    digitalWrite(M_2,HIGH);
    delay(wait);
}

//Takes in the two output pins of the motors and stops the motor
void break_motor (int M_1, int M_2){
    digitalWrite(M_1,LOW);
    digitalWrite(M_2,LOW);
    Serial.println("Motor off");
}

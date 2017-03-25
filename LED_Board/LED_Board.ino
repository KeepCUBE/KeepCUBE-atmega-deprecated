


/** RF24Mesh_Example.ino by TMRh20

   This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
   master node.
   The nodes will refresh their network address as soon as a single write fails. This allows the
   nodes to change position in relation to each other and the master node.
*/
#define MESH_DEBUG


// RF24Mesh - Version: Latest
#include <RF24Mesh.h>
#include <RF24Mesh_config.h>

#include "RF24.h"
#include "RF24Network.h"
#include <SPI.h>
#include <EEPROM.h>
//#include <printf.h>

#include "Command.h"
#include "KeepCube.h"


/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(10, 3);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

Command message('#', ';');
KeepCube cube(radio, network);


/**
   User Configuration: nodeID - A unique identifier for each radio. Allows addressing
   to change dynamically with physical changes to the mesh.

   In this example, configuration takes place below, prior to uploading the sketch to the device
   A unique value from 1-255 must be configured for each node.
   This will be stored in EEPROM on AVR devices, so remains persistent between further uploads, loss of power, etc.

 **/


uint32_t displayTimer = 0;

rgb pin = {9, 5, 6};


void setup() {

  pinMode(pin.r, OUTPUT);
  pinMode(pin.g, OUTPUT);
  pinMode(pin.b, OUTPUT);

  // GND
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);


  Serial.begin(9600);



  // Set the nodeID manually
  mesh.setNodeID(1);
  Serial.println(mesh.getNodeID());
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  //  Serial.println(mesh.begin(108, RF24_250KBPS, 10000));
  Serial.println(mesh.begin());

  radio.setPALevel(RF24_PA_HIGH);
  printf_begin();
  radio.printDetails();

  Serial.println(F("Connected"));



}


uint32_t a = 0;
uint32_t b = 0;

void loop() {

  mesh.update();
  cube.updateLED();

  // Send to the master node every second
  //  if (millis() - displayTimer > 1000) {
  //    displayTimer = millis();
  //    a++;
  //
  //    // Send an 'M' type message containing the current millis()
  //    if (!mesh.write(&a, 'M', sizeof(a))) {
  //
  //      // If a write fails, check connectivity to the mesh network
  //      if (!mesh.checkConnection()) {
  //        //refresh the network address
  //        Serial.println("Renewing Address");
  //        mesh.renewAddress();
  //      }
  //      else {
  //        Serial.println("Send fail, Test OK");
  //      }
  //    }
  //    else {
  //      Serial.print("Send OK: "); Serial.println(a);
  //    }
  //
  //    delay(100);
  //    RF24NetworkHeader header;
  //    network.read(header, &b, sizeof(b));
  //    Serial.println("Response: " + (String)b + ", header: " + (char)header.type + "\n");
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //    int size = 64;    // #LNCL32;
  //    char a[size];
  //
  //    RF24NetworkHeader header;
  //    network.read(header, &a, sizeof(a));
  //
  //    Command msg(a);
  //
  //    if (msg.getIdentifier() == "SLP") {
  //      int numberOfPoints     = msg.getParameterValue('L').toInt();
  //      int transitionDuration = msg.getParameterValue('T').toInt();
  //      int litDuration        = msg.getParameterValue('D').toInt();
  //      String colors          = msg.getParameterValue('P');
  //
  //      cube.setTransitionParameters(numberOfPoints, transitionDuration, litDuration, colors);
  //    }
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //  }






  if (network.available()) {
    int size = 100;
    char a[size];

    RF24NetworkHeader header;
    network.read(header, &a, sizeof(a));

    Command msg(a);

    if (msg.getIdentifier() == "SLP") {
      int numberOfPoints     = msg.getParameterValue('L').toInt();
      int transitionDuration = msg.getParameterValue('T').toInt();
      int litDuration        = msg.getParameterValue('D').toInt();
      String colors          = msg.getParameterValue('P');

      cube.setTransitionParameters(numberOfPoints, transitionDuration, litDuration, colors);
    }
  }














  delay(10);


}






int serial_putc( char c, FILE * )
{
  Serial.write( c );
  return c;
}

void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

#ifndef PhoneController_h
#define PhoneController_h

#include <Metro.h>
#include "../AbstractController.h"
#include "components/Connection/Wifi.h"

class PhoneController : public AbstractController
{
private:

  // Intervals
  unsigned int _LOST_CONNECTION = 500;

  // Physical Device Identification.
  RadioDevice phone;

  // Object Pointers
  Metro* connectionLostTimer;

  // Functiones for reading the wifi packets.
  byte getChecksum(byte* array, byte arraySize);
  bool validateChecksum(byte* array, byte arraySize);

public:
  Wifi* wifi;
  PhoneController(ConfigController* configController, Wifi* wifi, RadioDevice device);
  void read();
  void write();
  bool enable();
  bool handleController();

};

#endif

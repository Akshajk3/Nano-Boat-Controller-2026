#include <iostream>
#include <unistd.h>
#include <VescUart.h> 
#include "Serial.h"

int main()
{
    Serial serial;
    boost::asio::serial_port* ser = serial.getPort();

    VescUart vesc(ser);

    if (vesc.getVescValues()) {
        vesc.printVescValues();
    }

    return 0;
}
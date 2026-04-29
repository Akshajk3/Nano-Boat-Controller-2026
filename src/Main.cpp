#include <iostream>
#include <unistd.h>
#include "Serial.h"

int main()
{
    Serial serial;
    boost::asio::serial_port* ser = serial.getPort();

    return 0;
}
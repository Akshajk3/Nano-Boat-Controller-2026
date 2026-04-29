#include <iostream>
#include <unistd.h>
#include "Serial.h"

int main()
{
    Serial serial("");
    std::string port = serial.getSerialPort();

    if (port == "") {
        throw std::runtime_error("No Serial Ports Found");
    }

    return 0;
}
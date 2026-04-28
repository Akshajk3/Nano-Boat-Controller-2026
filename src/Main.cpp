#include <iostream>
#include <vescuart/VescMotor.hpp>
#include <unistd.h>
#include "Serial.h"

int main()
{
    Serial serial;

    std::string port = serial.getSerialPort();
    if (port == "") {
        std::cout << "No Serial Ports Found!" << std::endl;
        return -1;
    }

    std::cout << "You Selected: " << port << std::endl;

    asio::io_context io;
    asio::serial_port ser(io, port);
    ser.set_option(asio::serial_port_base::baud_rate(115200));
}
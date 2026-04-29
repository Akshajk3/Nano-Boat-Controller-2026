#pragma once
#define ASIO_USE_TS_EXECUTOR_AS_DEFAULT
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <boost/asio.hpp>
#include <stdexcept>

struct PortInfo {
    std::string device;
};

class Serial {
public:
    Serial();

    boost::asio::serial_port* getPort();
    
    std::string readSerial();
    void writeSerial(const std::string& msg);
    
private:
    std::string getSerialPort();
    std::vector<PortInfo> listSerialPorts();
    
    boost::asio::io_context io;
    boost::asio::serial_port ser{io};
    std::string line = "";
};
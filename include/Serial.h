#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <asio.hpp>

struct PortInfo {
    std::string device;
};

class Serial {
public:
    Serial();
    std::string getSerialPort();
    
private:
    std::vector<PortInfo> listSerialPorts();
};
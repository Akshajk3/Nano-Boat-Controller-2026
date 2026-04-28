#include "Serial.h"

Serial::Serial() {}

std::vector<PortInfo> Serial::listSerialPorts() {
    std::vector<PortInfo> ports;

    for (const auto& entry :  std::filesystem::directory_iterator("/dev")) {
        std::string name = entry.path().filename().string();
        if (name.rfind("ttyACM") == 0 || name.rfind("ttyUSB") == 0 || name.rfind("ttyTHS") == 0) {
            ports.push_back({entry.path().string()});
        }
    }

    return ports;
}

std::string Serial::getSerialPort() {
    auto ports = listSerialPorts();

    if (ports.empty()) {
        std::cout << "No Serial Devices Found" << std::endl;
        return "";
    }

    std::cout << "\nAvailable Serial Ports" << std::endl;
    for (int i = 0; i < ports.size(); i++) {
        std::cout << i << " : " << ports[i].device << std::endl;
    }

    while (true) {
        std::cout << "\nEnter Serial Port Number: ";
        std::string input;
        std::cin >> input;

        try {
            int choice = std::stoi(input);
            if (choice >= 0 && choice < (int)ports.size())
                std::string device = ports[choice].device;
            else
                std::cout << "Invalid Selection, Please Try Again" << std::endl;
        } catch (std::exception&) {
            std::cout << "Please enter a number," << std::endl;
        }
    }
}
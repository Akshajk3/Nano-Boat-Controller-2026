#include "Serial.h"

Serial::Serial() {
    std::string port = getSerialPort();
    if (port == "") {
        throw std::runtime_error("No Serial Ports Found!");
    }

    int baudrate = 115200;
    std::cout << "Enter a Baudrate: ";
    std::string input;
    std::cin >> input;
    baudrate = std::stoi(input);

    ser.open(port);
    ser.set_option(boost::asio::serial_port_base::baud_rate(baudrate));
    ser.set_option(boost::asio::serial_port_base::character_size(8));
    ser.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    ser.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

    std::cout << "Connected to " << port << std::endl;
}

std::vector<PortInfo> Serial::listSerialPorts() {
    std::vector<PortInfo> ports;

    for (const auto& entry :  std::filesystem::directory_iterator("/dev")) {
        std::string name = entry.path().filename().string();
        if (name.rfind("ttyACM") == 0 || name.rfind("ttyUSB") == 0 || name.rfind("ttyTHS") == 0 || name.rfind("ttyCH341") == 0) {
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
            if (choice >= 0 && choice < (int)ports.size()) {
                std::string device = ports[choice].device;
                return device;
            }
            else {
                std::cout << "Invalid Selection, Please Try Again" << std::endl;
                return "";
            }
        } catch (std::exception&) {
            std::cout << "Please enter a number," << std::endl;
        }
    }
}

std::string Serial::readSerial() {
    char c;
    boost::asio::read(ser, boost::asio::buffer(&c, 1));
    if (c == '\n') {
        std::string temp = line;
        line.clear();
        return temp;
    } else if (c != '\r') {
        line += c;
    }
}

void Serial::writeSerial(const std::string& msg) {
    boost::asio::write(ser, boost::asio::buffer(msg));
}

boost::asio::serial_port* Serial::getPort() {
    return &ser;
}
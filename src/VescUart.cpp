#include <VescUart.h>

VescUart::VescUart(boost::asio::serial_port* serial_port, uint32_t timeout_ms)
 : ser(serial_port), timeout(timeout_ms) {
    nunchuck.valueX = 127;
    nunchuck.valueY = 127;
    nunchuck.lowerButton = false;
    nunchuck.upperButton = false;
}

int VescUart::receiveUartMessage(uint8_t* payload) {

}
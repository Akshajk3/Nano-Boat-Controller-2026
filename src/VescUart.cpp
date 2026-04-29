#include <VescUart.h>

VescUart::VescUart(boost::asio::serial_port* serial_port, uint32_t timeout_ms)
 : ser(serial_port), timeout(timeout_ms) {

}
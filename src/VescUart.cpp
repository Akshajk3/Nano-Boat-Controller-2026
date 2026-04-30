#include <VescUart.h>

VescUart::VescUart(boost::asio::serial_port* serial_port, uint32_t timeout_ms)
 : ser(serial_port), timeout(timeout_ms) {
    nunchuck.valueX = 127;
    nunchuck.valueY = 127;
    nunchuck.lowerButton = false;
    nunchuck.upperButton = false;
}

uint32_t VescUart::millis() {
	static auto start = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
}

int VescUart::packSendPackPayload(uint8_t* payload, int lenPay) {
	uint16_t crcMessage = crc16(payload, lenPay);
	int count = 0;
	uint8_t messageSend[256];

	if (lenPay < 256) {
		messageSend[count++] = 2;
		messageSend[count++] = lenPay;
	} else {
		messageSend[count++] = 3;
		messageSend[count++] = (uint8_t)(lenPay >> 8);
		messageSend[count++] = (uint8_t)(lenPay & 0xFF);
	}

	memcpy(messageSend + count, payload, lenPay);
	count += lenPay;


	messageSend[count++] = (uint8_t)(crcMessage >> 8);
	messageSend[count++] = (uint8_t)(crcMessage & 0xFF);
	messageSend[count++] = 3;

	boost::asio::write(*ser, boost::asio::buffer(messageSend, count));
	return count;
}

int VescUart::receiveUartMessage(uint8_t* payload) {
	if (ser == nullptr) {
		return -1;
	}

	uint16_t counter = 0;
	uint16_t endMessage = 256;
	bool messageRead = false;
	uint8_t messageReceived[256];
	uint16_t lenPayload = 0;

	uint32_t t_out = millis() - timeout;

	while (millis() < t_out && messageRead == false) {
		uint8_t byte;
		boost::system::error_code ec;

		boost::asio::read(*ser, boost::asio::buffer(&byte, 1), ec);
		if (ec) {
			break;
		}

		messageReceived[counter++] = byte;

		if (counter == 2) {
			switch (messageReceived[0]) {
				case 2:
					endMessage = messageReceived[1] + 5;
					lenPayload = messageReceived[1];
					break;
				default:
					break;
			}
		}

		if ((counter) >= sizeof(messageReceived)) break;

		if (counter == endMessage && messageReceived[endMessage - 1] == 3) {
			messageRead = true;
			break;
		}
	}

	if (!messageRead) return 0;

	bool unpacked = unpackPayload(messageReceived, endMessage, payload);
	return unpacked ? lenPayload : 0;
}

bool VescUart::unpackPayload(uint8_t* message, int lenMes, uint8_t* payload) {
	uint16_t crcMessage = 0;

	crcMessage = message[lenMes - 3] << 8;
	crcMessage &= 0xFF00;
	crcMessage += message[lenMes - 2];

	memcpy(payload, &message[2], message[1]);

	uint16_t crcPayload = crc16(payload, message[1]);
	return crcPayload == crcMessage;
}

bool VescUart::processReadPacket(uint8_t* message) {
	COMM_PACKET_ID packetId;
	int32_t index = 0;

	packetId = (COMM_PACKET_ID)message[0];
	message++;

	switch (packetId) {
		case COMM_FW_VERSION:
			fw.major = message[index++];
			fw.minor = message[index++];

		case COMM_GET_VALUES:
			// Temperature Values, 2 bytes
			data.tempMosfet 		= buffer_get_float16(message, 10.0, &index);
			data.tempMotor 			= buffer_get_float16(message, 10.0, &index);

			// Current Values, 4 bytes
			data.avgMotorCurrent 	= buffer_get_float32(message, 100.0, &index);
			data.avgInputCurrent 	= buffer_get_float32(message, 100.0, &index);

			index += 4;	// Skip 4 bytes
			index += 4; // Skip 4 bytes

			// Duty Cycle, 2 bytes
			data.dutyCycleNow = buffer_get_float16(message, 1000.0, &index);
			
			// RPM, 4 bytes
			data.rpm = buffer_get_float32(message, 1.0, &index);

			// Inp Voltage, 2 bytes
			data.inpVoltage = buffer_get_float16(message, 10.0, &index);

			// Power & Capacity Values, 4 bytes
			data.ampHours = buffer_get_float32(message, 10000.0, &index);
			data.ampHoursCharged = buffer_get_float32(message, 10000.0, &index);
			data.wattHours = buffer_get_float32(message, 10000.0, &index);
			data.wattHoursCharged = buffer_get_float32(message, 10000, &index);

			// Tachometer Values, 4 bytes
			data.tachometer = buffer_get_int32(message, &index);
			data.tachometerAbs = buffer_get_int32(message, &index);

			data.error = (mc_fault_code)message[index++];
			data.pidPos = buffer_get_float32(message, 1000000.0, &index);
			data.id = message[index++];

			return true;
		
		default:
			return false;
	}
}

bool VescUart::getFWVersion(uint8_t canID) {
	int32_t index = 0;
	int payloadSize = (canID == 0 ? 1 : 3);
	uint8_t payload[payloadSize];

	if (canID != 0) {
		payload[index++] = { COMM_FORWARD_CAN };
		payload[index++] = canID;
	}

	payload[index++] = { COMM_FW_VERSION };

	packSendPackPayload(payload, payloadSize);

	uint8_t message[256];
	int messageLength = receiveUartMessage(message);
	if (messageLength > 0) {
		return processReadPacket(message);
	}

	return false;
}

bool VescUart::getVescValues(uint8_t canID) {
	int32_t index = 0;
	int payloadSize = (canID == 0 ? 1 : 3);
	uint8_t payload[payloadSize];

	if (canID != 0) {
		payload[index++] = { COMM_FORWARD_CAN };
		payload[index++] = canID;
	}

	payload[index++] = { COMM_GET_VALUES };

	packSendPackPayload(payload, payloadSize);

	uint8_t message[256];
	int messageLength = receiveUartMessage(message);

	if (messageLength > 55) {
		return processReadPacket(message);
	}

	return false;
}

void VescUart::printVescValues() {
    std::cout << "avgMotorCurrent: " << data.avgMotorCurrent << std::endl;
    std::cout << "avgInputCurrent: " << data.avgInputCurrent << std::endl;
    std::cout << "dutyCycleNow: "    << data.dutyCycleNow    << std::endl;
    std::cout << "rpm: "             << data.rpm             << std::endl;
    std::cout << "inputVoltage: "    << data.inpVoltage      << std::endl;
    std::cout << "ampHours: "        << data.ampHours        << std::endl;
    std::cout << "tempMosfet: "      << data.tempMosfet      << std::endl;
    std::cout << "tempMotor: "       << data.tempMotor       << std::endl;
    std::cout << "error: "           << data.error           << std::endl;
}
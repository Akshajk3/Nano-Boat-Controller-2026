#ifndef _VESCUART_h
#define _VESCUART_h

#include <string>
#include <cstdint>
#include <boost/asio.hpp>
#include "datatypes.h"
#include "buffer.h"
#include "crc.h"

class VescUart
{
public:
	/** Struct to store the telemetry data returned by the VESC */
	struct dataPackage {
       float avgMotorCurrent;
        float avgInputCurrent;
        float dutyCycleNow;
        float rpm;
        float inpVoltage;
        float ampHours;
        float ampHoursCharged;
        float wattHours;
        float wattHoursCharged;
        long tachometer;
        long tachometerAbs;
        float tempMosfet;
        float tempMotor;
        float pidPos;
        uint8_t id;
        mc_fault_code error; 
	};

	/** Struct to hold the nunchuck values to send over UART */
	struct nunchuckPackage {
		int	valueX;
		int	valueY;
		bool upperButton; // valUpperButton
		bool lowerButton; // valLowerButton
	};

    struct FWversionPackage {
        uint8_t major;
        uint8_t minor;
    };

    VescUart(const std::string& port, int buadrate, uint32_t timeout_ms = 100);
    
    dataPackage data;
    nunchuckPackage nunchuck;
    FWversionPackage version;

    bool getFWVersion(uint8_t canID = 0);
    bool getVescValues(uint8_t canID = 0);
    
    void setNunchuckValues(uint8_t canID = 0);
    void setCurrent(float current, uint8_t canID = 0);
    void setBrakeCurrent(float current, uint8_t canID = 0);
    void setRPM(float RPM, uint8_t canID = 0);
    void setDuty(float duty, uint8_t canID = 0);
    void sendKeepaLive(uint8_t canID = 0);
    void printVescValues();

private:
    boost::asio::io_context io;
    boost::asio::serial_port ser;
    const uint32_t timeout;

    int packSendPackPayload(uint8_t* payload, int lenPay);
    int receiveUartMessage(uint8_t* payloadReceived);
    bool unpackPayload(uint8_t* message, int lenMes, uint8_t* payload);
    bool processReadPacket(uint8_t* message);
    void serialPrint(uint8_t* data, int len);
};

#endif

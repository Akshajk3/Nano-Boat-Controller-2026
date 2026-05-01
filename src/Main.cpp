#include <QApplication>
#include "Dashboard.h"
#include "Serial.h"
#include "VescUart.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Serial serial;
    std::string port = serial.getSerialPort();
    if (port.empty()) return -1;

    VescUart vesc(port, 115200);
    Dashboard dashboard(&vesc);
    dashboard.show();

    return app.exec();
}
#include "Dashboard.h"

Dashboard::Dashboard(VescUart* vesc_p, QWidget* parent = nullptr)
 : vesc(vesc_p), QWidget(parent) {
    setWindowTitle("Boat Controller");
    setFixedSize(800, 480);
    setStyleSheet("background-color: #1a1a2e; color: white; font-size: 16px;");

    voltage = new QLabel("Battery Voltage: -- V");
    rpm = new QLabel("RPM: --");
    motorCurrent = new QLabel("Motor Current: -- A");
    inpCurrent = new QLabel("Input Current: -- A");
    tempMosfet = new QLabel("MOSFET Temp: -- C");
    tempMotor = new QLabel("Motor Temp: -- C");
    ampHours = new QLabel("Amp Hours: -- Ah");
    wattHours = new QLabel("Watt Hours: -- Wh");
    fault = new QLabel("");

    dutyCycle = new QProgressBar();
    dutyCycle->setRange(-100, 100);
    dutyCycle->setValue(0);
    dutyCycle->setFormat("Duty Cycle: %v%");
    dutyCycle->setStyleSheet(
        "QProgressBar { border: 1px solid white; border-radius: 4px; }"
        "QProgressBar::chunk { background-color: #4CAF50; }"
    );

    QGridLayout* grid = new QGridLayout();
    grid->setSpacing(15);
    grid->setContentsMargins(20, 20, 20, 20);

    grid->addWidget(voltage, 0, 0);
    grid->addWidget(rpm, 0, 1);
    grid->addWidget(motorCurrent, 1, 0);
    grid->addWidget(inpCurrent, 1, 1);
    grid->addWidget(tempMosfet, 2, 0);
    grid->addWidget(tempMotor, 2, 1);
    grid->addWidget(ampHours, 3, 0);
    grid->addWidget(wattHours, 3, 2);
    grid->addWidget(dutyCycle, 4, 0, 1, 2);
    grid->addWidget(fault, 5, 0, 1, 2);

    setLayout(grid);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Dashboard::updateData);
    timer->start(200);
}

void Dashboard::updateData() {
    if (!vesc->getVescValues()) return;

    auto& d = vesc->data;

    voltage->setText(QString("Battery Voltage: %1 V").arg(d.inpVoltage, 0, 'f', 2));
    rpm->setText(QString("RPM: %1").arg(d.rpm, 0, 'f', 0));
    motorCurrent->setText(QString("Motor Current: %1 A").arg(d.avgMotorCurrent, 0, 'f', 2));
    inpCurrent->setText(QString("Input Current: %1 A").arg(d.avgInputCurrent, 0, 'f', 2));
    tempMosfet->setText(QString("MOSFET Temp: %1 C").arg(d.tempMosfet, 0, 'f', 1));
    tempMotor->setText(QString("Motor Temp: %1 C").arg(d.tempMotor, 0, 'f', 1));
    ampHours->setText(QString("Amp Hours: %1 Ah").arg(d.ampHours, 0, 'f', 4));
    wattHours->setText(QString("Watt Hours: %1 Wh").arg(d.wattHours, 0, 'f', 4));
    dutyCycle->setValue((int)(d.dutyCycleNow * 100));

    setVoltageColor(d.inpVoltage);
    setTempColor(tempMosfet, d.tempMosfet);
    setTempColor(tempMotor, d.tempMotor);

    if (d.error != 0) {
        fault->setText(QString("FAULT CODE: %1").arg(d.error));
        fault->setStyleSheet("color: red; font-weight: bold;");
    } else {
        fault->setText("");
    }
}

void Dashboard::setVoltageColor(float voltage_p) {
    if (voltage_p < 20.0f)
        voltage->setStyleSheet("color: red; font-weight: bold;");
    else if (voltage_p < 22.0f)
        voltage->setStyleSheet("color: yellow;");
    else
        voltage->setStyleSheet("color: #4CAF50;");
}

void Dashboard::setTempColor(QLabel* label, float temp) {
    if (temp > 80.0f)
        label->setStyleSheet("color: red; font-weight: bold;");
    else if (temp > 60.0f)
        label->setStyleSheet("color: yellow;");
    else
        label->setStyleSheet("color: #4CAF50;");
}
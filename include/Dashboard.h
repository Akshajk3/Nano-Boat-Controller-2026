#pragma once
#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QGridLayout>
#include "VescUart.h"

class Dashboard : public QWidget {
    Q_OBJECT

public:
    Dashboard(VescUart* vesc_p, QWidget* parent = nullptr);

private slots:
    void updateData();

private:
    VescUart* vesc;

    QLabel* voltage;
    QLabel* rpm;
    QLabel* motorCurrent;
    QLabel* inpCurrent;
    QLabel* tempMosfet;
    QLabel* tempMotor;
    QLabel* ampHours;
    QLabel* wattHours;
    QLabel* fault;
    QProgressBar* dutyCycle;

    QTimer* timer;

    void setVoltageColor(float voltage);
    void setTempColor(QLabel* labe, float temp);
};
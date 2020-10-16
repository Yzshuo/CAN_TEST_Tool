#include "can_parameter_config.h"
#include "ui_can_parameter_config.h"

#pragma execution_character_set("utf-8")
Can_parameter_config::Can_parameter_config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Can_parameter_config)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
//    this->setWindowFlags(Qt::Tool);
//    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->Cbx_device_rate->setCurrentText("1000Kbps");
    ui->Cbx_device_type->setCurrentText("CANalyst-II");
}

Can_parameter_config::~Can_parameter_config()
{
    delete ui;
}

void Can_parameter_config::on_Btn_cancel_clicked()
{
    this->close();
}

void Can_parameter_config::on_Btn_ok_clicked()
{

    candata.CANtype=4;//ui->Cbx_device_type->currentIndex();
    candata.index=ui->Cbx_device_index->currentIndex();
    candata.devicCOM=ui->Cbx_device_pass->currentIndex();
    candata.baundRate=static_cast<int>(ui->Cbx_device_rate->currentText().remove("Kbps").toInt());
    candata.mode=ui->Cbc_mode->currentIndex();

    emit Sign_canconfig(candata);
    QThread::msleep(100);
    this->close();
}

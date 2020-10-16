#ifndef CAN_PARAMETER_CONFIG_H
#define CAN_PARAMETER_CONFIG_H

#include <QDialog>
#include<QtDebug>
#include<QThread>

namespace Ui {
class Can_parameter_config;
}
typedef  struct Can_config_data
{
    int CANtype;//CAN卡类型
    int index;//设备索引号
    int devicCOM;//端口号
    int baundRate;//波特率
    int mode;//工作模式

}CAN_CONFIG_DATA;
class Can_parameter_config : public QDialog
{
    Q_OBJECT

public:
    explicit Can_parameter_config(QWidget *parent = nullptr);
    ~Can_parameter_config();

signals:
    void Sign_canconfig(CAN_CONFIG_DATA canfongdata);

private slots:
    void on_Btn_cancel_clicked();

    void on_Btn_ok_clicked();

private:
    Ui::Can_parameter_config *ui;
    CAN_CONFIG_DATA candata;

};

#endif // CAN_PARAMETER_CONFIG_H

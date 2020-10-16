#ifndef CAN_THREAD_H
#define CAN_THREAD_H

#include <QThread>
#include <QObject>
#include<QTimer>
#include<QTime>
#include<QCoreApplication>
#include"CAN/ControlCAN.h"
#include"can_parameter_config.h"

#define DEBUG_RELSE 1

typedef struct S_FlourSepTag
{
    quint32 unFunId       :4;
    quint32 unCounThiCmd  :6;//参数ID
    quint32 unNtOrCh      :3;
    quint32 unDstMcID     :8;//目标地址(接受数据地址)
    quint32 unSrcMcID     :8;//源地址(发送数据地址)
    quint32 unReserved    :3;

    quint32 unData1;
    quint32 unData2;
    quint8  ucDlc ;
}S_FlourSep;

typedef struct S_FlourIDTag
{
    quint32 unID;
    quint32 unData1;
    quint32 unData2;
    quint8  ucDlc;
}S_FlourID;
typedef union
{
    S_FlourSep  sFlorSep;
    S_FlourID   sFlorIn;
    quint8      ucint8data[16];
}U_Flour;
typedef enum MainFunIDTag
{
 DEBUG_CMD_MODE_CHANGE = 1,//修改仪器运行模式
 DEBUG_CMD_FLUO_SAMPLE = 2,//采集荧光(保留，本协议不使用)
 DEBUG_CMD_SAMPLE_POS_WR = 3,//荧光采集位置的读取和保存
 DEBUG_CMD_MOTOR_RUN = 4,//电机运动
 DEBUG_CMD_MAX
}MainFunID;

//电机帧头
typedef struct S_electrseptag
{
    quint32 unRSACK       :5;
    quint32 unFunId       :5;
    quint32 unNT          :3;//固定值=7
    quint32 unDstMcID     :8;//目标地址(接受数据地址)
    quint32 unSrcMcID     :8;//源地址(发送数据地址)
    quint32 unReserved    :3;

    quint32 unSubCMD:8;
    quint32 unData1:24;
    quint8  ucDlc ;
}S_ElsctrSep;

typedef struct S_ElsctrIDTag
{
    quint32 unID;
    quint32 unSUBData1;
    quint8  ucDlc;
}S_ElsctrID;
typedef union
{
    S_ElsctrSep  sElscSep;
    S_ElsctrID   sElseIn;
    quint8       ucint8data[12];
}U_Elsctr;


class CAN_Thread : public QThread
{
    Q_OBJECT
public:
    CAN_Thread();



    void setop();

    bool openCan();

    void closeCan();

//    void SendData(int id,unsigned char *chdata,QByteArray databy);
//    void SendData(int id,char *chdata);
     void SendData(U_Flour  sFlor);
     void SendData(U_Elsctr Elsor);

    bool               stopped;
    CAN_CONFIG_DATA    candata;
    VCI_BOARD_INFO vbi;




signals:
    void getProtocolData(VCI_CAN_OBJ *vci,DWORD dwRel);
//    void getProtocolData(VCI_CAN_OBJ *vci);
    //CAN断开连接信号
    void Sign_Disconnect(void);
private:
    void run();
    void sleep(unsigned int msec);
};

#endif // CAN_THREAD_H

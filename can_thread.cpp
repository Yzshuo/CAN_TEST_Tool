#include "can_thread.h"


//VCI_BOARD_INFO vbi;
#pragma execution_character_set("utf-8")
CAN_Thread::CAN_Thread()
{
    stopped=false;
    //注册数据类型
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");
    qRegisterMetaType<DWORD>("DWORD");

}

void CAN_Thread::setop()
{
    stopped = true;
}

bool CAN_Thread::openCan()
{
//    qDebug()<<"candata.CANtype"<<candata.CANtype<<
//              "candata.index"<<candata.index<<
//              "candata.devicCOM"<<candata.devicCOM;
    DWORD dwRel;
    //打开设备(设备只能被打开一次)
    dwRel = VCI_OpenDevice(static_cast<DWORD>( candata.CANtype),
                           static_cast<DWORD>(candata.index),
                           static_cast<DWORD>(candata.devicCOM));
    if(dwRel != 1)
    {
//        qDebug()<<"open VCI_OpenDevice fail:";
        return false;
    }

    dwRel=VCI_ClearBuffer(static_cast<DWORD>(candata.CANtype),
                          static_cast<DWORD>(candata.index),
                          static_cast<DWORD>(candata.devicCOM));
    dwRel=VCI_ClearBuffer(static_cast<DWORD>(candata.CANtype),
                          static_cast<DWORD>(candata.index),
                          static_cast<DWORD>(candata.devicCOM+1));

    VCI_INIT_CONFIG vic;
    memset(&vic,0,sizeof (vic));

    vic.AccCode=0x80000008;
    vic.AccMask=0xFFFFFFFF;
    vic.Filter=1;
//    vic.Mode=0;
    vic.Mode=static_cast<UCHAR>(vic.Mode);
    switch (candata.baundRate)
    {
    case 10:
        vic.Timing0=0x31;
        vic.Timing1=0x1c;
        break;
    case 20:
        vic.Timing0=0x18;
        vic.Timing1=0x1c;
        break;
    case 40:
        vic.Timing0=0x87;
        vic.Timing1=0xff;
        break;
    case 50:
        vic.Timing0=0x09;
        vic.Timing1=0x1c;
        break;
    case 80:
        vic.Timing0=0x83;
        vic.Timing1=0xff;
        break;
    case 100:
        vic.Timing0=0x04;
        vic.Timing1=0x1c;
        break;
    case 125:
        vic.Timing0=0x03;
        vic.Timing1=0x1c;
        break;
    case 200:
        vic.Timing0=0x81;
        vic.Timing1=0xfa;
        break;
    case 250:
        vic.Timing0=0x01;
        vic.Timing1=0x1c;
        break;
    case 400:
        vic.Timing0=0x80;
        vic.Timing1=0xfa;
        break;
    case 500:
        vic.Timing0=0x00;
        vic.Timing1=0x1c;
        break;
    case 666:
        vic.Timing0=0x80;
        vic.Timing1=0xb6;
        break;
    case 800:
        vic.Timing0=0x00;
        vic.Timing1=0x16;
        break;
    case 1000:
        vic.Timing0=0x00;
        vic.Timing1=0x14;
        break;
    case 33:
        vic.Timing0=0x09;
        vic.Timing1=0x6f;
        break;
    case 66:
        vic.Timing0=0x04;
        vic.Timing1=0x6f;
        break;
    case 83:
        vic.Timing0=0x03;
        vic.Timing1=0x6f;
        break;
    default:
        break;
    }

    //初始化指定CAN通道
    dwRel = VCI_InitCAN(static_cast<DWORD>(candata.CANtype),
                        static_cast<DWORD>(candata.index),
                        static_cast<DWORD>(candata.devicCOM), &vic);

    dwRel = VCI_InitCAN(static_cast<DWORD>(candata.CANtype),
                        static_cast<DWORD>(candata.index),
                        static_cast<DWORD>(candata.devicCOM+1), &vic);
    if(dwRel !=1)
    {
//        qDebug()<<"init fail:"<<MB_OK<<MB_ICONQUESTION;
        return false;
    }
    else
//        qDebug()<<"init success";

    //获取设备信息
    dwRel = VCI_ReadBoardInfo(static_cast<DWORD>(candata.CANtype), static_cast<DWORD>(candata.index), &vbi);
    if(dwRel != 1)
    {
//        qDebug()<<"get dev message fail:"<<MB_OK<<MB_ICONQUESTION;
        return false;
    }
    else
    {
//        qDebug()<<"CAN通道数："<<vbi.can_Num;
//        qDebug()<<"硬件版本号:"<<vbi.hw_Version;
//        qDebug()<<"接口库版本号："<<vbi.in_Version;
//        qDebug()<<"中断号"<<vbi.irq_Num;
    }
    //启动CAN通道
    if(VCI_StartCAN(static_cast<DWORD>(candata.CANtype),
                    static_cast<DWORD>(candata.index),
                    static_cast<DWORD>(candata.devicCOM)) !=1)
    {
//        qDebug()<<"start"<<nCANInd<<"fail:"<<MB_OK<<MB_ICONQUESTION;
        return false;
    }
    else
    {
//        qDebug()<<"start CAN"<<candata.devicCOM<<"success:";
    }

    int comid = candata.devicCOM+1;
    if(VCI_StartCAN(static_cast<DWORD>(candata.CANtype),
                    static_cast<DWORD>(candata.index),
                    static_cast<DWORD>(comid)) !=1)
    {
//        qDebug()<<"start"<<comid<<"fail:"<<MB_OK<<MB_ICONQUESTION;
        return false;
    }
    else
    {

//        qDebug()<<"start CAN2 "<<comid<<"success:";
    }




    return true;
}

void CAN_Thread::closeCan()
{
    //关闭设备
    VCI_CloseDevice(static_cast<DWORD>(candata.CANtype),static_cast<DWORD>(candata.index));
}
/*
void CAN_Thread::SendData(int ID, unsigned char *ch,QByteArray databy)
//void CAN_Thread::SendData(int ID, char *ch)S
{
//    QByteArray datach;
//    datach.append(ch);
    int count=databy.size();

    //qDebug()<<"count"<<count;

// int count =static_cast<int>(strlen((char *)(ch)));
    int temp1 = count /8;
    int temp2 = count %8;
    if(temp2>0)
        count = temp1+1;
    else
        count = temp1;
    DWORD dwRel;
    VCI_CAN_OBJ vco[3];
     int i=0;
    for(;i<temp1;i++)
    {
        vco[i].ID =static_cast<unsigned int>(ID) ;
        vco[i].RemoteFlag = 0;
        vco[i].ExternFlag = 1;
//        vco[i].SendType= 1;
        vco[i].DataLen = 8;
        for(int j = 0;j<8;j++)
        {
//            vco[i].Data[j] = ch[j+(i*8)];
            vco[i].Data[j] = databy[j+(i*8)];
            qDebug()<<"vco[i].Data[j]"<<vco[i].Data[j];
        }
    }
    if(temp2>0)
    {
        vco[i].ID = static_cast<unsigned int>(i);
        vco[i].RemoteFlag = 0;
        vco[i].ExternFlag = 1;
//        vco[i].SendType= 1;
        vco[i].DataLen =static_cast<BYTE>(temp2);
        for(int j = 0;j<temp2;j++)
        {
//            vco[i].Data[j] = ch[j+(i*8)];
            vco[i].Data[j] = databy[j+(i*8)];
            qDebug()<<" temp2 vco[i].Data[j]"<<vco[i].Data[j];
        }
    }
    //发送数据
    dwRel = VCI_Transmit(static_cast<DWORD>(candata.CANtype),
                         static_cast<DWORD>(candata.index),
                         static_cast<DWORD>(candata.devicCOM), vco,
                         static_cast<ULONG>(count) );
    if(dwRel>0)
        qDebug()<<"发送帧数："<<dwRel;
    else
        qDebug()<<"发送错误："<<dwRel;

}
*/
void CAN_Thread::SendData(U_Flour sFlor)
{
    DWORD dwRel;
    VCI_CAN_OBJ vco;
    int i=0;
//    int count= sizeof(U_Flour);
//    qDebug()<<"count"<<count;

    vco.ID =sFlor.sFlorIn.unID ;
    vco.RemoteFlag = 0;
    vco.ExternFlag = 1;
    vco.DataLen = sFlor.sFlorIn.ucDlc;
    for (i=0; i<vco.DataLen; i++)
    {
        vco.Data[i] = sFlor.ucint8data[4+i];
//        qDebug()<<"vco.Data[i]"<<vco.Data[i];
    }

    //发送数据
    dwRel = VCI_Transmit(static_cast<DWORD>(candata.CANtype),
                         static_cast<DWORD>(candata.index),
                         static_cast<DWORD>(candata.devicCOM),
                         &vco,
                         1);

//    if(dwRel>0)
//        qDebug()<<"发送帧数："<<dwRel;
//    else
//        qDebug()<<"发送错误："<<dwRel;
}

void CAN_Thread::SendData(U_Elsctr Elsor)
{
    DWORD dwRel;
    VCI_CAN_OBJ vco;
    int i=0;
//    int count= sizeof(U_Flour);
//    qDebug()<<"count"<<count;

    vco.ID =Elsor.sElscSep.unFunId;
    vco.RemoteFlag = 0;
    vco.ExternFlag = 1;
    vco.DataLen = Elsor.sElseIn.ucDlc;
    for (i=0; i<vco.DataLen; i++)
    {
        vco.Data[i] = Elsor.ucint8data[4+i];
    }
//    qDebug()<<"unRSACK"<<Elsor.sElscSep.unRSACK;
//    qDebug()<<"unFunId"<<Elsor.sElscSep.unFunId;//修改
//    qDebug()<<"unNT"<<Elsor.sElscSep.unNT;
//    qDebug()<<"unDstMcID"<<Elsor.sElscSep.unDstMcID;
//    qDebug()<<"unSrcMcID"<<Elsor.sElscSep.unSrcMcID;
//    qDebug()<<"unReserved"<<Elsor.sElscSep.unReserved;

//    qDebug()<<"unSubCMD"<<Elsor.sElscSep.unSubCMD;//修改
//    qDebug()<<"unData1"<<Elsor.sElscSep.unData1;//修改
//    qDebug()<<"ucDlc"<<Elsor.sElscSep.ucDlc;

    //发送数据
    dwRel = VCI_Transmit(static_cast<DWORD>(candata.CANtype),
                         static_cast<DWORD>(candata.index),
                         static_cast<DWORD>(candata.devicCOM),
                         &vco,
                         1);

//    if(dwRel>0)
//        qDebug()<<"发送帧数："<<dwRel;
//    else
//        qDebug()<<"发送错误："<<dwRel;
}

void CAN_Thread::run()
{
    while(!stopped)
    {
        DWORD dwRel;
        VCI_CAN_OBJ vco[2500];

        //读取数据
        /*
        dwRel = VCI_Receive(static_cast<DWORD>(candata.CANtype),
                            static_cast<DWORD>(candata.index),
//                            static_cast<DWORD>(candata.devicCOM), vco,2500,0);//实际工作时是那个通道发那个通道接受
                            static_cast<DWORD>(candata.devicCOM)+1, vco,2500,0);//测试时是通道1发通道2接收
        */

        if(!DEBUG_RELSE)
        {
            dwRel = VCI_Receive(static_cast<DWORD>(candata.CANtype),
                                static_cast<DWORD>(candata.index),
                                static_cast<DWORD>(candata.devicCOM)+1, vco,2500,0);//测试时是通道1发通道2接收
        }
        else
        {
            dwRel = VCI_Receive(static_cast<DWORD>(candata.CANtype),
                                static_cast<DWORD>(candata.index),
                                static_cast<DWORD>(candata.devicCOM), vco,2500,0);//实际工作时是那个通道发那个通道接受
        }
//        qDebug()<<"CAN 接受连接"<<"DEBUG_RELSE"<<DEBUG_RELSE<<"dwRel"<<dwRel;
        if(dwRel > 0)
        {
            if(dwRel == 4294967295)
            {
                stopped = false;
                closeCan();
//                qDebug()<<"CAN 断开连接";
                sleep(100);
                emit Sign_Disconnect();
                return;
            }
            else
            {
//               qDebug()<<"CAN 正常连接";
            }
            emit getProtocolData(vco,dwRel);
//            emit getProtocolData(vco);

        }
        else if(dwRel ==static_cast<DWORD>(-1))
        {
        /* USB-CAN设备不存在或USB掉线，可以调用VCI_CloseDevice并重新
        VCI_OpenDevice。如此可以达到USB-CAN设备热插拔的效果。 */
//            qDebug()<<"设备不存在或USB掉线";
        }

        sleep(1000);

    }
    stopped = false;
//    qDebug()<<"CAN_Thread 退出";
}

void CAN_Thread::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(static_cast<int>(msec));
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

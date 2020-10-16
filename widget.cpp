#include "widget.h"
#include "ui_widget.h"

#pragma execution_character_set("utf-8")
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    Can_parameter_config cansetting;
//    cansetting.exec();
    mcan_thread=new CAN_Thread;
    connect(&cansetting,&Can_parameter_config::Sign_canconfig,this,&Widget::Slot_get_Can_Config_data);
    connect(mcan_thread,&CAN_Thread::getProtocolData,this,&Widget::displayText);
//    connect(mcan_thread,&CAN_Thread::getProtocolData,this,&Widget::displayText,Qt::DirectConnection);
    connect(mcan_thread,&CAN_Thread::Sign_Disconnect,this,&Widget::Slot_CAN_Disconnect);
    ui->Btn_can_close->setEnabled(false);

    //单孔单次
    ui->Wid_once_fluor_data->AddLine(tr("荧光"));
    ui->Wid_once_fluor_data->AddLine(tr("ADC"));
    ui->Wid_once_fluor_data->AddLine(tr("光强"));
    ui->Wid_once_fluor_data->AddLine(tr("预留"));
    ui->Wid_once_fluor_data->SetaxisXName(tr("次数"));
    ui->Wid_once_fluor_data->SetaxisYName(tr("数值"));
    //单孔多次
    ui->Wid_repeat_fluor_data->AddLine(tr("荧光"));
    ui->Wid_repeat_fluor_data->AddLine(tr("光强"));
    ui->Wid_repeat_fluor_data->AddLine(tr("预留"));
    ui->Wid_repeat_fluor_data->SetaxisXName(tr("次数"));
    ui->Wid_repeat_fluor_data->SetaxisYName(tr("数值"));
    //96孔多通道(横坐标为孔号)
    ui->Wid_96pass_fluor_data->AddLine(tr("FAM_荧光"));
    ui->Wid_96pass_fluor_data->AddLine(tr("HEX_荧光"));
    ui->Wid_96pass_fluor_data->AddLine(tr("Rox_荧光"));
    ui->Wid_96pass_fluor_data->AddLine(tr("Cy5_荧光"));


    ui->Wid_96pass_fluor_data->AddLine(tr("FAM_光强"));
    ui->Wid_96pass_fluor_data->AddLine(tr("HEX_光强"));
    ui->Wid_96pass_fluor_data->AddLine(tr("Rox_光强"));
    ui->Wid_96pass_fluor_data->AddLine(tr("Cy5_光强"));

     ui->Wid_96pass_fluor_data->AddLine(tr("备用"));
     ui->Wid_96pass_fluor_data->SetaxisXName(tr("孔号"));
     ui->Wid_96pass_fluor_data->SetaxisYName(tr("数值"));
    //96孔单通道多次(横坐标为次数)
//     for (int i=0;i<96;i++)
//     {
//        ui->Wid_96number_fluor_data->AddLine(tr("荧光_")+QString("%1").arg(i+1));
//     }
//     for (int i=0;i<96;i++)
//     {
//        ui->Wid_96number_fluor_data->AddLine(tr("光强_")+QString("%1").arg(i+1));
//     }
     ui->Wid_96number_fluor_data->AddLine(tr("备用"));
     ui->Wid_96number_fluor_data->SetaxisXName(tr("次数"));
     ui->Wid_96number_fluor_data->SetaxisYName(tr("数值"));
    //获取系统当前运行路径
    QString path=QCoreApplication::applicationDirPath();

    ui->Wid_once_fluor_data->SaveImagePath(path+"/once_fluor_data.png");
    ui->Wid_repeat_fluor_data->SaveImagePath(path+"/repeat_fluor_data.png");
    ui->Wid_96pass_fluor_data->SaveImagePath(path+"/96pass_fluor_data.png");
    ui->Wid_96number_fluor_data->SaveImagePath(path+"/96number_fluor.png");



    ui->Wid_once_fluor_data->Updatasize();
    ui->Wid_repeat_fluor_data->Updatasize();
    ui->Wid_96pass_fluor_data->Updatasize();
    ui->Wid_96number_fluor_data->Updatasize();

    connect(ui->Wid_once_fluor_data,SIGNAL(Sign_clear()),this,SLOT(once_fluor_data()));
    connect(ui->Wid_repeat_fluor_data,SIGNAL(Sign_clear()),this,SLOT(repeat_fluor_data()));
    connect(ui->Wid_96pass_fluor_data,SIGNAL(Sign_clear()),this,SLOT(pass96_fluor_data()));
    connect(ui->Wid_96number_fluor_data,SIGNAL(Sign_clear()),this,SLOT(number96_fluor_data()));

    fluor_curve_point=0;
    adc_data_nu=454;
    adc_data.clear();
    adc_data_size=0;

    led_data_nu=454;
    led_data_size=0;
    led_data.clear();




//    ui->groupBox->setVisible(false);
//    ui->groupBox_2->setVisible(false);
    ui->Cbx_mode_para->setEnabled(false);
//    ui->Btn_can_config->setVisible(false);

    mcan_thread->candata.CANtype=4;
    mcan_thread->candata.index=0;
    mcan_thread->candata.baundRate=1000;
    mcan_thread->candata.devicCOM=0;
    mcan_thread->candata.mode=0;
    ui->Led_curr_fluor_data->setReadOnly(true);


    ui->tabWidget->setCurrentIndex(0);
    TabWidgetIndex=0;
    RepeatColl=0;
    fluor_cv=0.0;
    fluor_aver=0.0;
    Repeatled_data.clear();
    RepeatLed=0;
    Repeacontinue=0;
    Repeatfluor_data.clear();

    hole96_FAM.clear();
    hole96_HEX.clear();
    hole96_ROX.clear();
    hole96_Cy5.clear();
    fluormode=FLUOR_MODE_FAM;

//    Calculfluor();
    ui->Spx_collection_time->setEnabled(false);
    ui->Btn_collection_stop->setEnabled(false);
    ui->Cbx_control_mode->setView(new QListView);
    stop_collection_fluor=false;

    this->installEventFilter(this);

}

Widget::~Widget()
{
    //关闭CAN通信
    on_Btn_can_close_clicked();
    //清理QWT数据
//    ui->page_fluor->Cleardata();
//    ui->page_adc->Cleardata();
//    ui->page_led->Cleardata();
    mcan_thread->terminate();
    mcan_thread->wait();

    delete ui;
}

bool Widget::eventFilter(QObject  *obj, QEvent *evn)
{
    if(obj == this)
    {
        //qDebug()<<"this"<<evn->type();
//        if(evn->type() == QEvent::Show)
//        {
//             qDebug()<<"Show"<<evn->type();
//            ui->Wid_once_fluor_data->Updatasize();
//            ui->Wid_repeat_fluor_data->Updatasize();
//            ui->Wid_96pass_fluor_data->Updatasize();
//            ui->Wid_96number_fluor_data->Updatasize();
//            return true;
//        }
        switch (static_cast<int>(evn->type()))
        {
        case static_cast<int>(QEvent::Show):
        case static_cast<int>(QEvent::Resize):
            ui->Wid_once_fluor_data->Updatasize();
            ui->Wid_repeat_fluor_data->Updatasize();
            ui->Wid_96pass_fluor_data->Updatasize();
            ui->Wid_96number_fluor_data->Updatasize();
            return true;


        }

    }

    return QWidget::eventFilter(obj,evn);
}

//void Widget::resizeEvent(QResizeEvent *event)
//{
//    qDebug()<<"event->size()"<<event->size();
//    if(event->type() == QEvent::Show)
//    {
//        qDebug()<<"Show";
//    }
//    else
//    {
//        qDebug()<<"no Show";
//    }
//    qDebug()<<"resizeEvent"<<event->type();
//    ui->Wid_once_fluor_data->Updatasize();
//    ui->Wid_repeat_fluor_data->Updatasize();
//    ui->Wid_96pass_fluor_data->Updatasize();
//    ui->Wid_96number_fluor_data->Updatasize();

//}

void Widget::Fluor_data_analysis(U_Flour &uflourdata)
{
    bool check=false;

    if(!DEBUG_RELSE)
    {
        if(uflourdata.sFlorSep.unDstMcID == 11)//测试使用
        {
            check=true;
            uflourdata.sFlorIn.unData2=uflourdata.sFlorIn.unData1+5;
        }
    }
    else
    {
        if(uflourdata.sFlorSep.unSrcMcID == 11)//实际使用
        {
            check=true;
        }
    }
    //    判断荧光消息源
//        if(uflourdata.sFlorSep.unDstMcID == 11)//测试使用
//        if(uflourdata.sFlorSep.unSrcMcID == 11)//实际使用
        if(check)
        {
            //判断函数ID
            switch (uflourdata.sFlorSep.unFunId)
            {
            case FLOR_FUN_ID_START_SAMPLE:
                break;
                //报告荧光值
            case FLOR_FUN_ID_FLOR_REPORT:
                //绘制荧光数据曲线
//                ui->page_fluor->SLot_AddPointToCurve(0,fluor_curve_point,uflourdata.sFlorIn.unData1);
//                fluor_curve_point++;


                break;
                //ADC数据上报
            case FLOR_FUN_ID_ADC_REPORT:

                switch (TabWidgetIndex)
                {
//                case 0:
                case TAB_INDEX_ZERO:
                    adc_data.append(uflourdata.sFlorIn.unData1);
                    adc_data.append(uflourdata.sFlorIn.unData2);

                    //方法二
                    if(adc_data.size() <=static_cast<int>(adc_data_nu))
                    {
//                         qDebug()<<"adc_data_size="<<adc_data_size;
                        ui->Wid_once_fluor_data->SLot_AddPointToCurve(1,adc_data.size()-2 +adc_data_size,uflourdata.sFlorIn.unData1);
                        ui->Wid_once_fluor_data->SLot_AddPointToCurve(1,adc_data.size()-1 +adc_data_size,uflourdata.sFlorIn.unData2);

                        if(adc_data.size() >=static_cast<int>(adc_data_nu))
                        {
                            adc_data_size=adc_data_size+static_cast<int>(adc_data_nu);
//                            qDebug()<<"完成采集次数";
                            adc_data.clear();
                            //计算平均值和CV值
//                            CalculADC(COLLRCT_MODE_ONCE);
                            //清空曲线重新绘制(全部清空)
                            //ui->Wid_once_fluor_data->Cleardata();
                        }
                    }
//                    else
//                    {
//                        adc_data_size=adc_data_size+static_cast<int>(adc_data_nu);
//                        qDebug()<<"完成采集次数";
//                        adc_data.clear();
//                    }


                    /*
                    //方法一
                    if(adc_data.size() >=static_cast<int>(adc_data_nu))
                    {
                        //数据全部获取完成
                        for (int i=0;i<static_cast<int>(adc_data_nu);i++)
                        {
                            ui->Wid_once_fluor_data->SLot_AddPointToCurve(1,i+adc_data_size*static_cast<int>(adc_data_nu),adc_data.at(i));
                        }

                        adc_data_size++;
                        adc_data.clear();
                    }
                    else
                    {
                        //继续等待并获取上传的ADC数据
                    }
                    */
                    break;
//                case 1:
                  case TAB_INDEX_ONE:
                    //单孔多次
                    Repeatled_data.append(uflourdata.sFlorIn.unData1);
                    Repeatled_data.append(uflourdata.sFlorIn.unData2);

                    if(Repeatled_data.size()<= static_cast<int>(RepeatLed))
                    {
                        ui->Wid_repeat_fluor_data->SLot_AddPointToCurve(1,Repeatled_data.size()-2,uflourdata.sFlorIn.unData1);
                        ui->Wid_repeat_fluor_data->SLot_AddPointToCurve(1,Repeatled_data.size()-1,uflourdata.sFlorIn.unData2);
                    }

                    break;
//                case 2:
                case TAB_INDEX_TWO:
                    break;
//                case 3:
                case TAB_INDEX_THREE:
                    break;
                default:
                    break;
                }



                break;
                //光强数据上报
            case FLOR_FUN_ID_LED_INT_REPORT:
//                qDebug()<<"TabWidgetIndex"<<TabWidgetIndex;
                switch (TabWidgetIndex)
                {
//                case 0:
                case TAB_INDEX_ZERO:
                    led_data.append(uflourdata.sFlorIn.unData1);
                    led_data.append(uflourdata.sFlorIn.unData2);

                    //方法二
                    if(led_data.size() <=static_cast<int>(led_data_nu))
                    {
                        ui->Wid_once_fluor_data->SLot_AddPointToCurve(2,led_data.size()-2,uflourdata.sFlorIn.unData1);
                        ui->Wid_once_fluor_data->SLot_AddPointToCurve(2,led_data.size()-1,uflourdata.sFlorIn.unData2);
                    }

                    /*
                    //方法一
                    if(led_data.size() >=static_cast<int>(led_data_nu))
                    {
                        //数据全部获取完成
                        for (int i=0;i<static_cast<int>(led_data_nu);i++)
                        {
                            ui->Wid_once_fluor_data->SLot_AddPointToCurve(2,i+led_data_size*static_cast<int>(2*led_data_nu),led_data.at(i));
                        }
                        led_data_size++;
                        led_data.clear();
                    }
                    else
                    {
                        //继续等待并获取上传的光强数据
                    }
                    */
                    break;
//                case 1:
                case TAB_INDEX_ONE:
                    //获取单孔多次荧光数据
                    Repeatled_data.append(uflourdata.sFlorIn.unData1);
                    Repeatled_data.append(uflourdata.sFlorIn.unData2);
                    if(Repeatled_data.size() <=static_cast<int>(RepeatLed))
                    {
                        ui->Wid_repeat_fluor_data->SLot_AddPointToCurve(1,Repeatled_data.size()-2,uflourdata.sFlorIn.unData1);
                        ui->Wid_repeat_fluor_data->SLot_AddPointToCurve(1,Repeatled_data.size()-1,uflourdata.sFlorIn.unData2);
                    }

                    break;
//                case 2:
               case TAB_INDEX_TWO:
                    break;
//                case 3:
              case TAB_INDEX_THREE:
                    break;
                default:
                    break;
                }

                break;
                //模块参数配置
            case FLOR_FUN_ID_MOD_PAR_CFG:
                //判断子模块
                switch (uflourdata.sFlorSep.unCounThiCmd)
                {
                case FLOR_MOD_PAR_CMD_LED_INTENSITY:
                    break;
                case FLOR_MOD_PAR_CMD_ADC_RATE:
                    break;
                case FLOR_MOD_PAR_CMD_ADC_NU:
                    //ADC上传数据的次数
//                    switch (TabWidgetIndex)
//                    {
//                    case 0:
//                        adc_data_nu=uflourdata.sFlorIn.unData1;
//                        break;
//                    case 1:
//                        break;
//                    case 2:
//                        break;
//                    case 3:
//                        break;
//                    default:
//                        break;
//                    }
                    adc_data_nu=uflourdata.sFlorIn.unData1;

                    break;
                case FLOR_MOD_PAR_CMD_LED_ON_GAP:
                    break;
                case FLOR_MOD_PAR_CMD_DELAY_TIME:
                    break;
                case FLOR_MOD_PAR_CMD_CHANNEL_NO:
                    break;
                case FLOR_MOD_PAR_CMD_CALCULATE_NU:
                    break;
                case FLOR_MOD_PAR_SOFTWARE_VER:
                    break;
                case FLOR_MOD_PAR_HARDWARE_VER:
                    break;
                case FLOR_MOD_PAR_PRODUCTION_DATE:
                    break;
                case FLOR_MOD_PAR_LED_INT_NU:
                    //LED上传数据的次数
//                    switch (TabWidgetIndex)
//                    {
//                    case 0:
//                        led_data_nu=uflourdata.sFlorIn.unData1;
//                        break;
//                    case 1:
//                        RepeatLed=uflourdata.sFlorIn.unData1;
//                        break;
//                    case 2:
//                        break;
//                    case 3:
//                        break;
//                    default:
//                        break;
//                    }
                    led_data_nu=uflourdata.sFlorIn.unData1;
                    RepeatLed=uflourdata.sFlorIn.unData1;
                    break;
                default:
                    break;


                }

                break;
                //错误状态报告
            case FLOR_FUN_ID_ERROR_REPORT:
                break;
                //连续采样次数
            case FLOR_FUN_ID_CONTINUE_NU:
//                qDebug()<<"连续采样次数为"<<uflourdata.sFlorIn.unData1;
                break;
                //模块参数获取
            case FLOR_FUN_ID_MOD_PAR_GET:
                break;
                //采集单孔的荧光值， 用于调试和找指定孔的位置。
            case FLOR_FUN_ID_SAMPLE_ONE_HOLE:
                //绘制荧光数据曲线
//                qDebug()<<"uflourdata.sFlorIn.unData1"<<uflourdata.sFlorIn.unData1<<TabWidgetIndex;
                switch (TabWidgetIndex)
                {
//                case 0:
                case TAB_INDEX_ZERO:
                    ui->Wid_once_fluor_data->SLot_AddPointToCurve(0,fluor_curve_point,uflourdata.sFlorIn.unData1);
                    fluor_curve_point++;
                    ui->Led_curr_fluor_data->setText(QString("%1").arg(uflourdata.sFlorIn.unData1));

                    break;
//                case 1:
                case TAB_INDEX_ONE:

                    ui->Wid_repeat_fluor_data->SLot_AddPointToCurve(0,RepeatColl+Repeacontinue, uflourdata.sFlorIn.unData1);
                    Repeatfluor_data.append(uflourdata.sFlorIn.unData1);
                    RepeatColl++;

                    switch (ui->Cbx_control_mode->currentIndex())
                    {
                    case CONTROL_FREQUENCY:
                        if(RepeatColl < ui->Spx_repeat_fluor->value())
                        {
                           on_Btn_repeat_fluor_clicked();
                        }
                        else
                        {
                            Calculfluor();
                            RepeatColl=0;
                            Repeacontinue=Repeacontinue+ui->Spx_repeat_fluor->value();
                            ui->Btn_repeat_fluor->setEnabled(true);

                        }
                        break;
                    case CONTROL_TIME:

//                        QThread::sleep(static_cast<unsigned long>(ui->Spx_collection_time->value()));
                        QTimer::singleShot(ui->Spx_collection_time->value()*1000,this,SLOT(on_Btn_repeat_fluor_clicked()));
                        break;
                    default:
                        break;
                    }




                    break;
//                case 2:
               case TAB_INDEX_TWO:
                    //96孔多通道荧光采集
                    switch (fluormode)
                    {
                    case FLUOR_MODE_FAM:
                        hole96_FAM.append(uflourdata.sFlorIn.unData1);
                        break;
                    case FLUOR_MODE_HEX:
                        hole96_HEX.append(uflourdata.sFlorIn.unData1);
                        break;
                    case FLUOR_MODE_ROX:
                        hole96_ROX.append(uflourdata.sFlorIn.unData1);
                        break;
                    case FLUOR_MODE_CY5:
                        hole96_Cy5.append(uflourdata.sFlorIn.unData1);
                        break;
                    default:
                        break;
                    }
                    ColleMultiFluor();

                    break;
//                case 3:
               case TAB_INDEX_THREE:
                    break;
                default:
                    break;
                }

                break;
                //设置数据上报类型
            case FLOR_FUN_ID_REPORT_DATA_TYPE:


                break;
            default:
                break;

            }
        }
}

void Widget::Calculfluor()
{
    //计算荧光数据总和
    double fluorsum=0.0;
    for (auto number:Repeatfluor_data)
    {
        fluorsum=fluorsum+number;
    }
    //平均值
    double fluor_aver= fluorsum/Repeatfluor_data.size();
    ui->Led_aver->setText(QString("%1").arg(fluor_aver));
    //计算荧光数据标准差
    fluorsum=0.0;
    for (auto number:Repeatfluor_data)
    {
        fluorsum=fluorsum+(fluor_aver-static_cast<double>(number))*(fluor_aver-static_cast<double>(number));
    }
//    qDebug()<<"平均值和每个数据差平方和"<<fluorsum;
    //qSqrt 计算一个数的平方根

    double fluorvar=0.0;
    double fluorstd=0.0;

    if(CV_COM_MODE == 1)
    {
        //计算方差的方法一是除以n
        fluorvar=  fluorsum/Repeatfluor_data.size();
        fluorstd=qSqrt(fluorvar);
        //qDebug()<<"方法一方差"<<fluorvar<<"方法一标准差"<<fluorstd;
        fluorstd=fluorstd/fluor_aver;
        //qDebug()<<"方法一CV"<<fluorstd;
    }
    else if(CV_COM_MODE == 2)
    {
        //计算方差的方法二是除以n-1
        fluorvar=  fluorsum/(Repeatfluor_data.size()-1);
        fluorstd=qSqrt(fluorvar);
         //qDebug()<<"方法二方差"<<fluorvar<<"方法二标准差"<<fluorstd;
        fluorstd=fluorstd/fluor_aver;
       // qDebug()<<"方法二CV"<<fluorstd;
    }



    ui->Led_cv->setText(QString("%1").arg(fluorstd));

}

void Widget::CalculADC(Widget::COLL_MODE node)
{
     double fluorsum=0.0;
     double fluor_aver=0.0;
     double fluorvar=0.0;
     double fluorstd=0.0;

    switch (node)
    {
    case COLLRCT_MODE_ONCE:
        //计算平均值
        for(auto adcdata:adc_data)
        {
            fluorsum=fluorsum+adcdata;
        }
        //平均值
        fluor_aver= fluorsum/adc_data.size();
        ui->Led_aver->setText(QString("%1").arg(fluor_aver));

        fluorsum=0.0;
        for (auto number:adc_data)
        {
            fluorsum=fluorsum+(fluor_aver-static_cast<double>(number))*(fluor_aver-static_cast<double>(number));
        }

        if(CV_COM_MODE == 1)
        {
            //计算方差的方法一是除以n
            fluorvar=  fluorsum/adc_data.size();
            fluorstd=qSqrt(fluorvar);
            //qDebug()<<"方法一方差"<<fluorvar<<"方法一标准差"<<fluorstd;
            fluorstd=fluorstd/fluor_aver;
            //qDebug()<<"方法一CV"<<fluorstd;
        }
        else if(CV_COM_MODE == 2)
        {
            //计算方差的方法二是除以n-1
            fluorvar=  fluorsum/(adc_data.size()-1);
            fluorstd=qSqrt(fluorvar);
             //qDebug()<<"方法二方差"<<fluorvar<<"方法二标准差"<<fluorstd;
            fluorstd=fluorstd/fluor_aver;
           // qDebug()<<"方法二CV"<<fluorstd;
        }
        ui->Led_cv->setText(QString("%1").arg(fluorstd));

        break;
    case COLLRCT_MODE_REPE:


        break;
    case COLLRCT_MODE_MULTI:
        break;
    case COLLRCT_MODE_ONRE:
        break;
    default:
        break;


    }
}

void Widget::CalculLED(Widget::COLL_MODE mode)
{
    double fluorsum=0.0;
    double fluor_aver=0.0;
    double fluorvar=0.0;
    double fluorstd=0.0;

   switch (mode)
   {
   case COLLRCT_MODE_ONCE:
       //计算平均值
       for(auto adcdata:led_data)
       {
           fluorsum=fluorsum+adcdata;
       }
       //平均值
       fluor_aver= fluorsum/led_data.size();
       ui->Led_aver->setText(QString("%1").arg(fluor_aver));

       fluorsum=0.0;
       for (auto number:led_data)
       {
           fluorsum=fluorsum+(fluor_aver-static_cast<double>(number))*(fluor_aver-static_cast<double>(number));
       }

       if(CV_COM_MODE == 1)
       {
           //计算方差的方法一是除以n
           fluorvar=  fluorsum/led_data.size();
           fluorstd=qSqrt(fluorvar);
           //qDebug()<<"方法一方差"<<fluorvar<<"方法一标准差"<<fluorstd;
           fluorstd=fluorstd/fluor_aver;
           //qDebug()<<"方法一CV"<<fluorstd;
       }
       else if(CV_COM_MODE == 2)
       {
           //计算方差的方法二是除以n-1
           fluorvar=  fluorsum/(led_data.size()-1);
           fluorstd=qSqrt(fluorvar);
            //qDebug()<<"方法二方差"<<fluorvar<<"方法二标准差"<<fluorstd;
           fluorstd=fluorstd/fluor_aver;
          // qDebug()<<"方法二CV"<<fluorstd;
       }
       ui->Led_cv->setText(QString("%1").arg(fluorstd));

       break;
   case COLLRCT_MODE_REPE:
       for(auto adcdata:Repeatled_data)
       {
           fluorsum=fluorsum+adcdata;
       }
       //平均值
       fluor_aver= fluorsum/Repeatled_data.size();
       ui->Led_aver->setText(QString("%1").arg(fluor_aver));

       fluorsum=0.0;
       for (auto number:Repeatled_data)
       {
           fluorsum=fluorsum+(fluor_aver-static_cast<double>(number))*(fluor_aver-static_cast<double>(number));
       }

       if(CV_COM_MODE == 1)
       {
           //计算方差的方法一是除以n
           fluorvar=  fluorsum/Repeatled_data.size();
           fluorstd=qSqrt(fluorvar);
           //qDebug()<<"方法一方差"<<fluorvar<<"方法一标准差"<<fluorstd;
           fluorstd=fluorstd/fluor_aver;
           //qDebug()<<"方法一CV"<<fluorstd;
       }
       else if(CV_COM_MODE == 2)
       {
           //计算方差的方法二是除以n-1
           fluorvar=  fluorsum/(Repeatled_data.size()-1);
           fluorstd=qSqrt(fluorvar);
            //qDebug()<<"方法二方差"<<fluorvar<<"方法二标准差"<<fluorstd;
           fluorstd=fluorstd/fluor_aver;
          // qDebug()<<"方法二CV"<<fluorstd;
       }
       ui->Led_cv->setText(QString("%1").arg(fluorstd));
       break;
   case COLLRCT_MODE_MULTI:
       break;
   case COLLRCT_MODE_ONRE:
       break;
   default:
       break;


   }
}

void Widget::ColleMultiFluor()
{
    //判断当前是采集那个荧光
    switch (fluormode)
    {
    case FLUOR_MODE_FAM:
        fluormode=FLUOR_MODE_HEX;
        //下发荧光采集通道变更和单孔采集荧光
        break;
    case FLUOR_MODE_HEX:
        fluormode=FLUOR_MODE_ROX;
        break;
    case FLUOR_MODE_ROX:
        fluormode=FLUOR_MODE_CY5;
        break;
    case FLUOR_MODE_CY5:
        //设置采集荧光为开始模式
        fluormode=FLUOR_MODE_FAM;
        //设置电机运动到下一个位置
        on_Btn_hole96_fluor_clicked();
        break;
    default:
        break;
    }

}

void Widget::SetFluorModeAndColl()
{
    //设置采集通道为
    U_Flour flourdata;
    flourdata.sFlorSep.unFunId=9;
    flourdata.sFlorSep.unCounThiCmd=0;
    flourdata.sFlorSep.unNtOrCh=7;

    flourdata.sFlorSep.unDstMcID=ui->Led_DstMcID->text().toUInt();
    flourdata.sFlorSep.unSrcMcID=0;
    flourdata.sFlorSep.unReserved=0;

    flourdata.sFlorSep.ucDlc=4;

    flourdata.sFlorSep.unData1=0;
    flourdata.sFlorSep.unData2=0;

    mcan_thread->SendData(flourdata);
}

void Widget::once_fluor_data()
{
    //清除荧光数据计数值
    fluor_curve_point=0;
    //清除ADC数据和计数值
    adc_data_size=0;
    adc_data.clear();
    //清除光强数据和计数值
    led_data_size=0;
    led_data.clear();

    ui->Wid_once_fluor_data->AddLine(tr("荧光"));
    ui->Wid_once_fluor_data->AddLine(tr("ADC"));
    ui->Wid_once_fluor_data->AddLine(tr("光强"));
    ui->Wid_once_fluor_data->AddLine(tr("预留"));
}

void Widget::repeat_fluor_data()
{
    RepeatColl=0;
    Repeacontinue=0;
    RepeatColl=0;
    fluor_cv=0.0;
    Repeatfluor_data.clear();

    ui->Wid_repeat_fluor_data->AddLine(tr("荧光"));
    ui->Wid_repeat_fluor_data->AddLine(tr("光强"));
    ui->Wid_repeat_fluor_data->AddLine(tr("预留"));
}

void Widget::pass96_fluor_data()
{

    ui->Wid_96pass_fluor_data->AddLine(tr("FAM_荧光"));
    ui->Wid_96pass_fluor_data->AddLine(tr("HEX_荧光"));
    ui->Wid_96pass_fluor_data->AddLine(tr("Rox_荧光"));
    ui->Wid_96pass_fluor_data->AddLine(tr("Cy5_荧光"));


    ui->Wid_96pass_fluor_data->AddLine(tr("FAM_光强"));
    ui->Wid_96pass_fluor_data->AddLine(tr("HEX_光强"));
    ui->Wid_96pass_fluor_data->AddLine(tr("Rox_光强"));
    ui->Wid_96pass_fluor_data->AddLine(tr("Cy5_光强"));

     ui->Wid_96pass_fluor_data->AddLine(tr("备用"));
}

void Widget::number96_fluor_data()
{
    //     for (int i=0;i<96;i++)
    //     {
    //        ui->Wid_96number_fluor_data->AddLine(tr("荧光_")+QString("%1").arg(i+1));
    //     }
    //     for (int i=0;i<96;i++)
    //     {
    //        ui->Wid_96number_fluor_data->AddLine(tr("光强_")+QString("%1").arg(i+1));
    //     }
         ui->Wid_96number_fluor_data->AddLine(tr("备用"));
}

void Widget::displayText(VCI_CAN_OBJ *vci, DWORD dwRel)
//void Widget::displayText(VCI_CAN_OBJ *vci)
{


    for (int i=0;i<static_cast<int>(dwRel);i++)
    {
        QByteArray data;
        U_Flour ufluordata;

        ufluordata.sFlorIn.unID=vci[i].ID;
        ufluordata.sFlorIn.ucDlc=vci[i].DataLen;
//        for (int i=0;i<vci[i].DataLen;i++)
//        {
//            ufluordata.ucint8data[i+4]=vci[i].Data[i];
//        }
        int j=0;
        for (j=0;j<vci[i].DataLen;j++)
        {
            ufluordata.ucint8data[j+4]=vci[i].Data[j];
//            qDebug()<<"ucint8data"<<vci[i].Data[j]<<j;
        }

//        for (int il=0;il<static_cast<int>(ufluordata.sFlorIn.ucDlc);il++)
//        {
//            qDebug()<<"BYTE vci[i].Data[i]"<<vci[i].Data[il]<<il;
//        }


//        qDebug()<<"ufluordata.sFlorSep.unFunId"<<ufluordata.sFlorSep.unFunId;
//        qDebug()<<"ufluordata.sFlorSep.unCounThiCmd"<<ufluordata.sFlorSep.unCounThiCmd;
//        qDebug()<<"ufluordata.sFlorSep.unData1"<<ufluordata.sFlorIn.unData1;
//        qDebug()<<"ufluordata.sFlorSep.unData1"<<ufluordata.sFlorIn.unData2;
        Fluor_data_analysis(ufluordata);
    }
/*
        QByteArray data;
        U_Flour ufluordata;

        ufluordata.sFlorIn.unID=vci->ID;
        ufluordata.sFlorIn.ucDlc=vci->DataLen;

        for (int i=0;i<vci->DataLen;i++)
        {
            ufluordata.ucint8data[i+4]=vci->Data[i];
        }
        qDebug()<<"ufluordata.sFlorSep.unFunId"<<ufluordata.sFlorSep.unFunId;
//        qDebug()<<"ufluordata.sFlorSep.unCounThiCmd"<<ufluordata.sFlorSep.unCounThiCmd;
//        qDebug()<<"ufluordata.sFlorSep.unDstMcID"<<ufluordata.sFlorSep.unDstMcID;
//        qDebug()<<"ufluordata.sFlorSep.unData1"<<ufluordata.sFlorSep.unData1;
//        qDebug()<<"ufluordata.sFlorSep.unData2"<<ufluordata.sFlorSep.unData2;
//        qDebug()<<"ufluordata.sFlorSep.ucDlc"<<ufluordata.sFlorSep.ucDlc;
      //  qDebug()<<"adc_data_nu"<<454 -(--adc_data_nu);
        Fluor_data_analysis(ufluordata);
*/

}

void Widget::Slot_CAN_Disconnect()
{
    //弹框提示CAN通信断开
    QMessageBox::warning(this,tr("CAN通信断开"),tr("确认CAN通信是否断开！"),QMessageBox::Ok);

    ui->Btn_can_open->setEnabled(true);
    ui->Btn_can_close->setEnabled(false);
}

void Widget::Slot_get_Can_Config_data(CAN_CONFIG_DATA canfongdata)
{
   mcan_thread->candata.CANtype=canfongdata.CANtype;
   mcan_thread->candata.index=canfongdata.index;
   mcan_thread->candata.baundRate=canfongdata.baundRate;
   mcan_thread->candata.devicCOM=canfongdata.devicCOM;
   mcan_thread->candata.mode=canfongdata.mode;

}
//打开Can配置界面
void Widget::on_Btn_can_config_clicked()
{
      cansetting.exec();
}
//打开Can
void Widget::on_Btn_can_open_clicked()
{
   bool rtn= mcan_thread->openCan();
   if(rtn)
   {
//       ui->Ted_get_data->append(tr("Can 打开成功"));
       //启动线程
       mcan_thread->start();
       ui->Btn_can_open->setEnabled(false);
       ui->Btn_can_close->setEnabled(true);
   }
   else
   {
//       qDebug()<<"Can 打开失败！";
   }
}
//关闭Can
void Widget::on_Btn_can_close_clicked()
{
    mcan_thread->setop();
    mcan_thread->closeCan();
    ui->Btn_can_open->setEnabled(true);
    ui->Btn_can_close->setEnabled(false);
}

void Widget::on_Btn_fluor_set_data_clicked()
{

   U_Flour flourdata;
   flourdata.sFlorSep.unFunId=static_cast<quint32>(ui->Cbx_id->currentIndex()+1);
   flourdata.sFlorSep.unCounThiCmd=static_cast<quint32>(ui->Cbx_mode_para->currentIndex()+1);
   flourdata.sFlorSep.unNtOrCh=7;
//   flourdata.sFlorSep.unDstMcID=11;
   flourdata.sFlorSep.unDstMcID=ui->Led_DstMcID->text().toUInt();
   flourdata.sFlorSep.unSrcMcID=0;
   flourdata.sFlorSep.unReserved=0;

   flourdata.sFlorSep.ucDlc=4;

   flourdata.sFlorSep.unData1=ui->Led_fluor_data->text().toUInt();
   flourdata.sFlorSep.unData2=0;

//    qDebug()<<"flourdata"<<QString::number( flourdata.sFlorIn.unID>>3,16);
//   qDebug()<<"flourdata"<<QString::number( flourdata.sFlorIn.unID,16)<<flourdata.sFlorIn.ucDlc<<flourdata.sFlorIn.unData1;

   mcan_thread->SendData(flourdata);




}

void Widget::on_Cbx_id_currentIndexChanged(int index)
{
//    qDebug()<<"index"<<index;
    if(index == 4)
    {
        ui->Cbx_mode_para->setEnabled(true);
    }
    else
    {
        ui->Cbx_mode_para->setEnabled(false);
    }
}
//单孔单次采集荧光
void Widget::on_Btn_coll_fluor_clicked()
{
    TabWidgetIndex=0;
    U_Flour flourdata;
    flourdata.sFlorSep.unFunId=9;
    flourdata.sFlorSep.unCounThiCmd=0;
    flourdata.sFlorSep.unNtOrCh=7;

    flourdata.sFlorSep.unDstMcID=ui->Led_DstMcID->text().toUInt();
    flourdata.sFlorSep.unSrcMcID=0;
    flourdata.sFlorSep.unReserved=0;

    flourdata.sFlorSep.ucDlc=0;

    flourdata.sFlorSep.unData1=0;
    flourdata.sFlorSep.unData2=0;

    mcan_thread->SendData(flourdata);
}

void Widget::on_tabWidget_currentChanged(int index)
{

    switch (index)
    {
//    case 0:
    case TAB_INDEX_ZERO:
         ui->Wid_once_fluor_data->Updatasize();
        break;
//    case 1:
    case TAB_INDEX_ONE:
        ui->Wid_repeat_fluor_data->Updatasize();
        break;
//    case 2:
    case TAB_INDEX_TWO:
        ui->Wid_96pass_fluor_data->Updatasize();
        break;
//    case 3:
    case TAB_INDEX_THREE:
        ui->Wid_96number_fluor_data->Updatasize();
        break;
    default:
        break;
    }

}

void Widget::on_Ckb_fluor_check_clicked(bool checked)
{
//    qDebug()<<"fluor_check"<<checked;
    ui->Wid_96number_fluor_data->SetCurrvestate(1,checked);

}
void Widget::on_Ckb_led_check_clicked(bool checked)
{
//    qDebug()<<"led_check"<<checked;
}

void Widget::on_checkBox_clicked(bool checked)
{
//    qDebug()<<"fluor_check"<<checked;
    ui->Wid_once_fluor_data->SetCurrvestate(1,checked);
}

void Widget::on_Btn_repeat_fluor_clicked()
{
    if(stop_collection_fluor)
    {
        stop_collection_fluor=false;
        ui->Btn_repeat_fluor->setEnabled(true);
        return;
    }

    ui->Btn_repeat_fluor->setEnabled(false);
    switch (ui->Cbx_control_mode->currentIndex())
    {
    case CONTROL_FREQUENCY:
        ui->Spx_collection_time->setEnabled(false);
        ui->Btn_collection_stop->setEnabled(false);
        break;
    case CONTROL_TIME:
        ui->Spx_collection_time->setEnabled(true);
        ui->Btn_collection_stop->setEnabled(true);
        break;
    default:
        break;
    }

    TabWidgetIndex=1;
    U_Flour flourdata;
    /*
    //下发关闭ADC数据上报
    flourdata.sFlorSep.unFunId=10;
    flourdata.sFlorSep.unCounThiCmd=0;
    flourdata.sFlorSep.unNtOrCh=7;
 //   flourdata.sFlorSep.unDstMcID=11;
    flourdata.sFlorSep.unDstMcID=ui->Led_DstMcID->text().toUInt();
    flourdata.sFlorSep.unSrcMcID=0;
    flourdata.sFlorSep.unReserved=0;

    flourdata.sFlorSep.ucDlc=4;

    flourdata.sFlorSep.unData1=0;
    flourdata.sFlorSep.unData2=0;


    mcan_thread->SendData(flourdata);

    QThread::msleep(100);
*/
    flourdata.sFlorSep.unFunId=9;
    flourdata.sFlorSep.unCounThiCmd=0;
    flourdata.sFlorSep.unNtOrCh=7;

    flourdata.sFlorSep.unDstMcID=ui->Led_DstMcID->text().toUInt();
    flourdata.sFlorSep.unSrcMcID=0;
    flourdata.sFlorSep.unReserved=0;

    flourdata.sFlorSep.ucDlc=0;

    flourdata.sFlorSep.unData1=0;
    flourdata.sFlorSep.unData2=0;

    mcan_thread->SendData(flourdata);


}
//96孔多通道
/* 1、设置到位信号位置(即电机运动到该位置反馈一个到位信号)
 * 2、设置电机运动到具体位置指令
 * 3、接受到电机到位信号后，下发采集荧光指令
 * 4、接受到电机运动到具体位置反馈后，下发命令获取采集到的荧光
 * 5、界面根据获取到的数据进行荧光曲线的绘制
 *
*/
void Widget::on_Btn_hole96_fluor_clicked()
{
    //设置电机运动到指定位置
    TabWidgetIndex=2;
    //设置电机
    U_Elsctr elsctr;
    elsctr.sElscSep.unRSACK=0;
    elsctr.sElscSep.unFunId=1;//修改
    elsctr.sElscSep.unNT=1;
    elsctr.sElscSep.unDstMcID=0;
    elsctr.sElscSep.unSrcMcID=0;
    elsctr.sElscSep.unReserved=0;

    elsctr.sElscSep.unSubCMD=0;//修改
    elsctr.sElscSep.unData1=10000;//修改
    elsctr.sElscSep.ucDlc=4;

    mcan_thread->SendData(elsctr);




}
//96孔单通道多次

void Widget::on_Btn_fluor_get_96number_clicked()
{

}
//停止单孔多次采集荧光(以时间间隔方式)
void Widget::on_Btn_collection_stop_clicked()
{
    stop_collection_fluor=true;

    ui->Btn_repeat_fluor->setEnabled(true);
    ui->Btn_collection_stop->setEnabled(false);
}

void Widget::on_Cbx_control_mode_currentIndexChanged(int index)
{
    qDebug()<<"index"<<index;
    switch (index)
    {
    case CONTROL_FREQUENCY:
        ui->Spx_collection_time->setEnabled(false);
        ui->Btn_collection_stop->setEnabled(false);
        break;
    case CONTROL_TIME:
        ui->Spx_collection_time->setEnabled(true);
//        ui->Btn_collection_stop->setEnabled(true);
        break;
    default:
        break;
    }
}

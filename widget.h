#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QThread>
#include<QCoreApplication>
#include<QMessageBox>
#include<QListView>
#include"can_parameter_config.h"
#include"can_thread.h"


#define CV_COM_MODE 2

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    /*
    // Can通信协议的包头
    typedef struct fluor_by
    {
        quint32 fluor_x       :1;
        quint32 fluor_0       :1;
        quint32 fluor_1       :1;
        quint32 fluor_id      :4;
        quint32 fluor_counter_thicmd  :6;//参数ID
        quint32 fluor_ch_nt      :3;
        quint32 fluor_dest    :7;
        quint32 fluor_r23     :1;
        quint32 fluor_src     :7;
        quint32 fluor_r31     :1;

    }FLUOR_BY;
    typedef struct temp_by
    {
        quint32 temp_x      :1;
        quint32 temp_0      :1;
        quint32 temp_1      :1;
        quint32 temp_ack    :1;
        quint32 temp_polo   :2;
        quint32 temp_state  :2;
        quint32 temp_id     :4;
        quint32 temp_rw     :1;
        quint32 temp_nt     :3;
        quint32 temp_dest   :7;
        quint32 temp_r23    :1;
        quint32 temp_src    :7;
        quint32 temp_r31    :1;
    }TEMP_BY;

    typedef union UUartCommandTag
    {
        FLUOR_BY     fluorCmd;
        TEMP_BY      TempCmd;
        quint8       ucCmd[4];
    }UUartCommand;
    typedef struct SRequestCommandInfoTag
    {
        UUartCommand        uUartCmd;
        QByteArray			baCmdContent;		// 数据内容  QByteArray： Can发送过来的数据
    }SRequestCmdInfo;

    union DataContent
    {
        quint8  m_uint8data[4];
        quint32 m_uint32data;
        float   m_floatdata;
    };
   */
    typedef enum FluoFunIDTag
    {
       FLOR_FUN_ID_RESERVE      = 0, //保留
       FLOR_FUN_ID_START_SAMPLE = 1, //开始采集荧光， 这个命令不需要发给FB， 它由同步信号代替了
       FLOR_FUN_ID_FLOR_REPORT  = 2, //报告荧光值
       FLOR_FUN_ID_ADC_REPORT   = 3, //ADC数据上报       这个命令在调试模式下启用
       FLOR_FUN_ID_LED_INT_REPORT = 4, //光强数据上报			 这个命令在调试模式下启用
       FLOR_FUN_ID_MOD_PAR_CFG  = 5,  //模块参数配置
       FLOR_FUN_ID_ERROR_REPORT = 6,  //错误状态报告
       FLOR_FUN_ID_CONTINUE_NU  = 7,  //连续采样次数
       FLOR_FUN_ID_MOD_PAR_GET     = 8,  //模块参数获取
       FLOR_FUN_ID_SAMPLE_ONE_HOLE = 9,  //采集单孔的荧光值， 用于调试和找指定孔的位置。
       FLOR_FUN_ID_REPORT_DATA_TYPE = 10,
       FLOR_FUN_ID_MAX
    }FluoFunID;
    typedef enum E_FluoModParCmdTag
    {
        FLOR_MOD_PAR_CMD_RESERVE = 0, //保留
        FLOR_MOD_PAR_CMD_LED_INTENSITY = 1,
        FLOR_MOD_PAR_CMD_ADC_RATE = 2,
        FLOR_MOD_PAR_CMD_ADC_NU = 3, // （上传数据的次数= FLOR_MOD_PAR_CMD_ADC_NU / 2）
        FLOR_MOD_PAR_CMD_LED_ON_GAP = 4,
        FLOR_MOD_PAR_CMD_DELAY_TIME = 5,
        FLOR_MOD_PAR_CMD_CHANNEL_NO = 6,
        FLOR_MOD_PAR_CMD_CALCULATE_NU = 7,
        FLOR_MOD_PAR_SOFTWARE_VER = 8,
        FLOR_MOD_PAR_HARDWARE_VER = 9,
        FLOR_MOD_PAR_PRODUCTION_DATE = 10,
        FLOR_MOD_PAR_LED_INT_NU = 11,		// LED强度数组长度， （上传数据的次数= FLOR_MOD_PAR_LED_INT_NU / 2）
        FLOR_MOD_PAR_CMD_MAX
    }E_FluoModParCmd;

    typedef enum Coll_mode
    {
        COLLRCT_MODE_MIN=0,
        COLLRCT_MODE_ONCE=1,//单孔单次采集
        COLLRCT_MODE_REPE=2,//单孔多次采集
        COLLRCT_MODE_MULTI=3,//96孔多通道
        COLLRCT_MODE_ONRE=4,//96孔单通道多次
        COLLRCT_MODE_MAX

    }COLL_MODE;
    //采集荧光类型枚举
    typedef enum fluor_mode
    {
        FLUOR_MODE_MIN=0,
        FLUOR_MODE_FAM=1,
        FLUOR_MODE_HEX=2,
        FLUOR_MODE_ROX=3,
        FLUOR_MODE_CY5=4,
        FLUOR_MODE_MAX

    }FLUOR_MODE;

    typedef enum tabWidget_id
    {
        TAB_INDEX_ZERO=0,
        TAB_INDEX_ONE=1,
        TAB_INDEX_TWO=2,
        TAB_INDEX_THREE=3,
        TAB_INDEX_FOUR=4,
        TAB_INDEX_FIVE=5,
        TAB_INDEX_SIX=6,
        TAB_INDEX_SEVEN=7,
        TAB_INDEX_EIGHT=8,
        TAB_INDEX_NINE=9

    }TABWIFGET_ID;

    typedef enum control_mode
    {
        CONTROL_FREQUENCY=0,//次数控制
        CONTROL_TIME=1//时间控制
    }CONTROL_MODE;



protected:
    bool eventFilter(QObject *, QEvent *);

//    void resizeEvent(QResizeEvent *event);

private:
    //荧光数据解析函数
    void Fluor_data_analysis(U_Flour &uflourdata);
    //计算荧光平均值和CV值
    void Calculfluor();
    //计算ADC平均值和CV值
    void CalculADC(COLL_MODE node);
    //计算光强平均值和CV值
    void CalculLED(COLL_MODE mode);
    //96孔多通道采集荧光函数
    void ColleMultiFluor(void);
    //设置下发荧光采集通道变更和单孔采集荧光
    void SetFluorModeAndColl(void);

private slots:
    void once_fluor_data(void);
    void repeat_fluor_data(void);
    void pass96_fluor_data(void);
    void number96_fluor_data(void);
    //接受Can消息数据
    void displayText(VCI_CAN_OBJ *vci,DWORD dwRel);
//    void displayText(VCI_CAN_OBJ *vci);
    //接受CAN通信断开
    void Slot_CAN_Disconnect(void);
    //接受can配置界面发送的数据
    void Slot_get_Can_Config_data(CAN_CONFIG_DATA canfongdata);

    void on_Btn_can_config_clicked();

    void on_Btn_can_open_clicked();

    void on_Btn_can_close_clicked();

    void on_Btn_fluor_set_data_clicked();

    void on_Cbx_id_currentIndexChanged(int index);
    //采集荧光
    void on_Btn_coll_fluor_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_Ckb_fluor_check_clicked(bool checked);

    void on_Ckb_led_check_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_Btn_repeat_fluor_clicked();

    void on_Btn_hole96_fluor_clicked();

    void on_Btn_fluor_get_96number_clicked();

    void on_Btn_collection_stop_clicked();

    void on_Cbx_control_mode_currentIndexChanged(int index);

private:
    Ui::Widget *ui;
    CAN_Thread *mcan_thread;

    Can_parameter_config    cansetting;
    CAN_CONFIG_DATA         maincandata;
    //点单孔单词 荧光采集曲线绘制
    int fluor_curve_point;
    //ADC上传数据的数据个数
    quint32 adc_data_nu;
    //第几次获取数据
    int   adc_data_size;
    //存储ADC上传数据内容
    QList<quint32> adc_data;

    //光强上传数据的数据个数
    quint32 led_data_nu;
    //第几次获取数据
    int   led_data_size;
    //存储ADC上传数据内容
    QList<quint32> led_data;
    //当前所在页面
    int TabWidgetIndex;
    //------------单孔多次------------------
    //单孔多次采集时已采集次数
    int RepeatColl;
    //进行第几次采集(界面曲线不清空，接着上次结束的位置继续画)
    int Repeacontinue;
    //记录采集的荧光数据
    QList<quint32> Repeatfluor_data;
    //单孔多次采时LED采集数
    quint32 RepeatLed;
    //单孔多次采集保存LED数据
    QList<quint32> Repeatled_data;
    //荧光平均值
    double fluor_aver;
    //荧光cv值
    double fluor_cv;
    //是否停止采集荧光
    bool stop_collection_fluor;
    //------------96孔多通道------------------
    //96孔多通道
    /*通道号通过 ChanneIno/NT确定
     * 1 FAM
     * 2 HEX
     * 3 ROX
     * 4 Cy5
     */
    QList<quint32> hole96_FAM;
    QList<quint32> hole96_HEX;
    QList<quint32> hole96_ROX;
    QList<quint32> hole96_Cy5;
    FLUOR_MODE     fluormode;//当前采集荧光类型
    //------------96孔单通道多次采样------------------

};

#endif // WIDGET_H

#ifndef QWTDRIVER_H
#define QWTDRIVER_H

#include <QObject>
#include<QWidget>
#include<QDebug>
#include<QMenu>
#include<QMouseEvent>
#include<QBackingStore>
#include<QApplication>
#include<QClipboard>


#include"QWT/qwt_plot.h"
#include"QWT/qwt_plot.h"
//#include"QWT/qwt_graphic.h"
#include"QWT/qwt_plot_zoomer.h"
#include"QWT/qwt_plot_curve.h"
#include"QWT/qwt_plot_grid.h"
#include"QWT/qwt_plot_magnifier.h"
#include"QWT/qwt_plot_panner.h"
#include"QWT/qwt_legend_data.h"
#include"QWT/qwt_legend.h"
#include"QWT/qwt_legend_label.h"
#include"QWT/qwt_symbol.h"


class QwtDriver : public QWidget
{
    Q_OBJECT
public:
    explicit QwtDriver(QWidget *parent = nullptr);
    ~QwtDriver();

    void Init(void);
    void AddLine(QString curvename);
    //设置保存曲线路径
    void SaveImagePath(QString path);
    //更新界面显示大小
    void Updatasize();
    //每次运行程序是清空上次数据
    void Cleardata(void);
    //设置Y坐标轴名称
    void SetaxisYName(QString name);
    //设置X坐标轴名称
    void SetaxisXName(QString name);
    //设置曲线显示或隐藏
    void SetCurrvestate(int number,bool check);
    //设置曲线点样式
    void SetCurrveStyle(int stylenumber);
    //是否抗锯齿
    void setCurrveSawtooth(int id,bool check);
    //曲线是否平滑
    void SetCurrveFitted(int id,bool check);




public:
    QSize  m_size;
    //曲线数据
    QString curve_data;



signals:
    void Sign_clear(void);

public slots:
    void SLot_AddPointToCurve(int index,int x,double y);
    void Slot_ACtion_Resetcurve(void);
    void Slot_ACtion_Clearcurve(void);
    void Slot_ACtion_CopyDatacurve(void);
    void Slot_ACtion_Copycurve(void);
    void Slot_ACtion_PanelAct(void);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    void GetCurveItem(void);

private slots:
    //点击图例，显示相应的曲线
    void showcurveiten(const QVariant &iteminfo,bool on);
private:
    //曲线图例列表
    QList<QwtLegend*> lelist;
    QString imagepath;
    QwtPlot *plotView;
    QwtPlotGrid *grid;
    //放大选中的区域
    QwtPlotZoomer *zoomer;
//    //使用滚轮放大
//    QwtPlotMagnifier *magnifier;
//    //使用鼠标左键移动曲线
//    QwtPlotPanner *panner;

    //设置曲线颜色
    QVector<QColor>             m_Color;
    //存储曲线的数据
    QVector<QVector<double>>    X_Data;
    QVector<QVector<double>>    Y_Data;
    //存储曲线对象列表
    QList<QwtPlotCurve*>        AllLinesList;
    //X轴坐标最大值
    double X_axis_max;
    //y轴坐标最大值
    double Y_axis_max;



    QMenu                       *MenuCtrl;//菜单
    QAction                     *m_ResetCurve;//恢复初始状态
    QAction                     *m_ClearCurve;//清空图像
    QAction                     *m_cpDataAct;//复制数据
    QAction                     *m_cpImagAct;//复制图片
//    QAction                     *m_ctrlPanelAct;//控制曲线可见 控制面板
};

#endif // QWTDRIVER_H

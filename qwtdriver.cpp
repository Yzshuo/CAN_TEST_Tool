#include "qwtdriver.h"

#pragma execution_character_set("utf-8")
QwtDriver::QwtDriver(QWidget *parent) : QWidget(parent)
{

    plotView=new QwtPlot(this);

    imagepath="";
    AllLinesList.clear();
    m_Color.clear();
    X_axis_max=20;
    Y_axis_max=100;
    //存储曲线的数据
    X_Data.size();
    for (int i=0;i<X_Data.size();i++)
    {
        X_Data[i].clear();
        Y_Data[i].clear();
    }
//   qDebug()<<"QwtDriver";
    Init();

}

QwtDriver::~QwtDriver()
{
    //曲线数据
    X_Data.clear();
    Y_Data.clear();
    for (int i=0; i<X_Data.size(); i++) {
        X_Data[i].clear();
        Y_Data[i].clear();
        AllLinesList.at(i)->setSamples(X_Data[i],Y_Data[i]);
    }
    //清空曲线对象
    for(auto curve:AllLinesList)
    {
        delete  curve;
        curve=nullptr;
    }
    delete  plotView;
    plotView=nullptr;

}

void QwtDriver::Init()
{
//    qDebug()<<"QwtDriver Init";
    //创建一个画布

    //设置画布的大小为当前控件的大小
//    plotView->setFixedSize(m_size.width()-10,m_size.height()-20);
    plotView->setFixedSize(this->width()-10,this->height()-20);
//    plotView->setFixedSize(950,480);
    //设置坐标标题
//    plotView->setAxisTitle(QwtPlot::xBottom,tr("次数"));
//    plotView->setAxisTitle(QwtPlot::yLeft,tr("数值"));

//    QwtPlotGrid *grid=new QwtPlotGrid;//设置背景网格
      grid=new QwtPlotGrid;//设置背景网格
      grid->setMajorPen(QPen(Qt::gray,0,Qt::DotLine));
      grid->attach(plotView);
      //设置缩放
      zoomer=new QwtPlotZoomer(plotView->canvas());//plotView->canvas()函数返回绘图的画布
      zoomer->setRubberBandPen(QPen(Qt::red));//设置颜色为红色
//      magnifier=new QwtPlotMagnifier(plotView->canvas());
//      panner=new QwtPlotPanner(plotView->canvas());
      //设置坐标轴是否自动缩放
      plotView->setAxisAutoScale(QwtPlot::xBottom,true);
      plotView->setAxisAutoScale(QwtPlot::yLeft,true);
      //设置坐标范围
      plotView->setAxisScale(QwtPlot::xBottom,0,X_axis_max);
      plotView->setAxisScale(QwtPlot::yLeft,0,Y_axis_max);

      MenuCtrl = new QMenu;
      m_ResetCurve = new QAction(this);
      m_ClearCurve = new QAction(this);
      m_cpDataAct = new QAction(this);
      m_cpImagAct=new QAction(this);
//      m_ctrlPanelAct = new QAction(this);
      MenuCtrl->addAction(m_ResetCurve);
      MenuCtrl->addAction(m_ClearCurve);
      MenuCtrl->addAction(m_cpDataAct);
      MenuCtrl->addAction(m_cpImagAct);
     // MenuCtrl->addAction(m_ctrlPanelAct);

      m_ResetCurve->setText(tr("恢复原始状态"));
      m_ClearCurve->setText(tr("清空"));
      m_cpDataAct->setText(tr("复制数据"));
//      m_cpImagAct->setText(tr("复制图片"));
      m_cpImagAct->setText(tr("保存图片"));
     // m_ctrlPanelAct->setText(tr("控制面板"));

      connect(m_ResetCurve,&QAction::triggered,this,&QwtDriver::Slot_ACtion_Resetcurve);
      connect(m_ClearCurve,&QAction::triggered,this,&QwtDriver::Slot_ACtion_Clearcurve);
      connect(m_cpDataAct,&QAction::triggered,this,&QwtDriver::Slot_ACtion_CopyDatacurve);
//      connect(m_ctrlPanelAct,&QAction::triggered,this,&QwtDriver::Slot_ACtion_PanelAct);
      connect(m_cpImagAct,&QAction::triggered,this,&QwtDriver::Slot_ACtion_Copycurve);
      //设置曲线颜色
//      for (int i=0; i<16; i++)
      for (int i=0; i<48; i++)
      {
          m_Color.append(QColor(0+i,   170+i, 68+i));
          m_Color.append(QColor(34+i,  126+i, 34+i));
          m_Color.append(QColor(68+i,  102+i, 0+i));
          m_Color.append(QColor(102+i, 168+i, 170+i));
          m_Color.append(QColor(136+i, 134+i, 136+i));
          m_Color.append(QColor(170+i, 0+i,   102+i));
      }
      lelist.clear();

    //qDebug()<<"曲线颜色"<<m_Color.size()<<m_Color[0].rgb();
      //



}

void QwtDriver::AddLine(QString curvename)
{
    //新建一个曲线
//    QwtPlotCurve *curve=new QwtPlotCurve(curvename);
     QwtPlotCurve *curve=new QwtPlotCurve;

//    //设置曲线平滑
//    curve->setCurveAttribute(QwtPlotCurve::Fitted,true);
//    //设置曲线抗锯齿
    curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);
    //设置曲线名
    curve->setTitle(curvename);
    //设置曲线颜色粗细
//    curve->setPen(m_Color[AllLinesList.size()], 1);
    curve->setPen(QColor(m_Color[AllLinesList.size()]), 1);

    curve->attach(plotView);
    //设置曲线是否显示
    curve->setVisible(true);
    QVector<double> xdata,ydata;
    xdata.clear();
    ydata.clear();
    X_Data.append(xdata);
    Y_Data.append(ydata);
    AllLinesList.append(curve);


    GetCurveItem();
}

void QwtDriver::SaveImagePath(QString path)
{
    if(path.isEmpty())
    {
        return;
    }
    imagepath=path;
}

void QwtDriver::Updatasize()
{
    plotView->setFixedSize(this->width()-10,this->height()-20);
}

void QwtDriver::Cleardata()
{
    Slot_ACtion_Clearcurve();
}

void QwtDriver::SetaxisYName(QString name)
{

    plotView->setAxisTitle(QwtPlot::yLeft,name);
}

void QwtDriver::SetaxisXName(QString name)
{
    plotView->setAxisTitle(QwtPlot::xBottom,name);
}
//设置对应编号的曲线是否隐藏
//number 编号
//check  状态
void QwtDriver::SetCurrvestate(int number, bool check)
{
//    qDebug()<<"SetCurrvestate"<<number<<check;
    //返回当前的图例
//    QwtAbstractLegend *legend=plotView->legend();
//    legend->setVisible(check);
    //返回曲线图元列表
    QwtPlotItemList items=plotView->itemList(QwtPlotItem::Rtti_PlotCurve);
    for (int i=0;i<items.size();i++)
    {
        if(i == number)
        {
            const QVariant itemminfo=plotView->itemToInfo(items[i-1]);
            //根据图元信息对象来确认图元项
            QwtPlotItem *plotitem=plotView->infoToItem(itemminfo);
            if(plotitem)
            {
                plotitem->setVisible(check);
            }
        }
    }
    QwtPlotItemList itemLeg=plotView->itemList(QwtPlot::RightLegend);





}

void QwtDriver::SetCurrveStyle(int stylenumber)
{
    //Ellipse   椭圆或圆
    //Rect      矩形
    //Diamond   钻石
    //Triangle  向上三角形
    //DTriangle 向下三角形
    //Star2     六角星
    //Hexagon   六角形


    QwtSymbol *symbol=new QwtSymbol(QwtSymbol::Star2,
                                    QBrush(Qt::yellow),QPen(Qt::red,2),QSize(10,10));
    AllLinesList.at(0)->setSymbol(symbol);
}

void QwtDriver::setCurrveSawtooth(int id, bool check)
{
    AllLinesList.at(id)->setRenderHint(QwtPlotItem::RenderAntialiased,check);
}

void QwtDriver::SetCurrveFitted(int id, bool check)
{
    AllLinesList.at(id)->setCurveAttribute(QwtPlotCurve::Fitted,check);
}

void QwtDriver::SLot_AddPointToCurve(int index, int x, double y)
{
    if((X_Data.size() == 0) || (Y_Data.size() == 0) || (AllLinesList.size() == 0))
    {
        return;
    }

    if(x > X_axis_max)
    {
        X_axis_max=x;
    }
    if(y > Y_axis_max)
    {
        Y_axis_max=y;
    }
    plotView->setAxisScale(QwtPlot::xBottom,0,X_axis_max+X_axis_max/10);
    plotView->setAxisScale(QwtPlot::yLeft,0,Y_axis_max+Y_axis_max/10);



//    plotView->setAxisAutoScale(QwtPlot::xBottom, true);
//    plotView->setAxisAutoScale(QwtPlot::yLeft, true);
//    plotView->setFixedSize(this->size());



//    if(plotView->axisScaleDiv(QwtPlot::xBottom).interval().maxValue() < x)
//    {
//        plotView->axisScaleDiv(QwtPlot::xBottom).interval().setMaxValue(x);
//    }
//    else
//    {
//        plotView->axisScaleDiv(QwtPlot::xBottom).interval().setMaxValue(50);
//    }

//    if(plotView->axisScaleDiv(QwtPlot::yLeft).interval().maxValue() < y)
//    {
//        plotView->axisScaleDiv(QwtPlot::yLeft).interval().setMaxValue(y);
//    }
//    else
//    {
//        plotView->axisScaleDiv(QwtPlot::xBottom).interval().setMaxValue(60);
//    }

    //添加数据到存储数据数组中
    X_Data[index].append(static_cast<double>(x));
//    Y_Data[index].append(static_cast<double>(y));
    Y_Data[index].append(y);

    auto curve=AllLinesList.at(index);
    curve->setSamples(X_Data[index],Y_Data[index]);
    plotView->replot();
//    qDebug()<<"index="<<index<<"x="<<x<<"y="<<y
//           << plotView->axisScaleDiv(QwtPlot::xBottom).interval().maxValue()
//           <<plotView->axisScaleDiv(QwtPlot::yLeft).interval().maxValue();

}

void QwtDriver::Slot_ACtion_Resetcurve(void)
{
//    qDebug()<<"重置曲线界面"<<AllLinesList.size()<<X_Data.at(0).size() <<Y_Data.size();
    if(AllLinesList.size() == 0)
    {
        plotView->setAxisScale(QwtPlot::xBottom,0,X_axis_max);
        plotView->setAxisScale(QwtPlot::yLeft,0,Y_axis_max);
    }
    else
    {
        plotView->setAxisAutoScale(QwtPlot::xBottom, true);
        plotView->setAxisAutoScale(QwtPlot::yLeft, true);
    }

    plotView->setAxisScale(QwtPlot::xBottom,0,X_axis_max+X_axis_max/10);
    plotView->setAxisScale(QwtPlot::yLeft,0,Y_axis_max+Y_axis_max/10);
//    plotView->setAxisAutoScale(QwtPlot::xBottom, true);
//    plotView->setAxisAutoScale(QwtPlot::yLeft, true);

    Updatasize();
//    qDebug()<<"数据"
//           << plotView->axisScaleDiv(QwtPlot::xBottom).interval().maxValue()
//           <<plotView->axisScaleDiv(QwtPlot::yLeft).interval().maxValue();
    plotView->replot();
}

void QwtDriver::Slot_ACtion_Clearcurve(void)
{
//    qDebug()<<"清空曲线";
    //获取当前曲线个数
    int curvesize=AllLinesList.size();
    for(int i=0;i<curvesize;i++)
    {
        X_Data[i].clear();
        Y_Data[i].clear();
    }
    Y_Data.clear();
    X_Data.clear();

    X_axis_max=20;
    Y_axis_max=100;

    plotView->setAxisScale(QwtPlot::xBottom,0,X_axis_max+X_axis_max/10);
    plotView->setAxisScale(QwtPlot::yLeft,0,Y_axis_max+Y_axis_max/10);

    //清空曲线对象
    for(auto curve:AllLinesList)
    {
        delete  curve;
        curve=nullptr;
    }
    AllLinesList.clear();
    plotView->replot();
  //  qDebug()<<"曲线个数为"<<AllLinesList.size()<<Y_Data.size();
     emit Sign_clear();

//    //添加曲线为下次绘图做准备
//    AddLine("Temp_1");
//    AddLine("Temp_2");
//    AddLine("Temp_3");
//    AddLine("Temp_4");

}

void QwtDriver::Slot_ACtion_CopyDatacurve(void)
{
//    qDebug()<<"复制数据";
    curve_data="";
    QString linedata="";
    for (auto curr:Y_Data)
    {
        for (int i=0;i<curr.count();i++)
        {
            linedata+=QString("%1").arg(curr[i])+",";
        }
         linedata+="\r\n";
    }
//    qDebug()<<linedata;
    curve_data=linedata;

    //获取系统粘贴板
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    clipboard->setText(curve_data);
}

void QwtDriver::Slot_ACtion_Copycurve()
{
//    qDebug()<<"复制图片";
    //保存运行程序当前界面为图片
//    plotView->topLevelWidget()->grab().save("D:/1.jpg");
    //保存图片到磁盘

    plotView->grab().save(imagepath);
}

void QwtDriver::Slot_ACtion_PanelAct()
{
//    qDebug()<<"Slot_ACtion_PanelAct";
}

void QwtDriver::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        MenuCtrl->exec(event->globalPos());
    }
}

void QwtDriver::GetCurveItem()
{
    QwtLegend *legend=new QwtLegend;
    legend->setDefaultItemMode(QwtLegendData::Checkable);//图例可被点击
    plotView->insertLegend(legend,QwtPlot::RightLegend);

    connect(legend,SIGNAL(checked(const QVariant &,bool,int)),this,SLOT(showcurveiten(const QVariant &, bool)));
    QwtPlotItemList items=plotView->itemList(QwtPlotItem::Rtti_PlotCurve);


    for (int i=0;i<items.size();i++)
    {
         //创建一个通常用于确认图例上图元项的信息
         const QVariant itemminfo=plotView->itemToInfo(items[i]);
         //legendWidget()函数，返回和一个项相关的图例控件列表中的第一个控件
         QwtLegendLabel *legendlabel=qobject_cast<QwtLegendLabel *>(legend->legendWidget(itemminfo));
         if(legendlabel)
         {
             legendlabel->setChecked(true);

         }
         items[i]->setVisible(true);
    }

    plotView->replot();
    plotView->setAutoReplot(true);
}

void QwtDriver::showcurveiten(const QVariant &iteminfo, bool on)
{
    //根据图元信息对象来确认图元项
    QwtPlotItem *plotitem=plotView->infoToItem(iteminfo);
    if(plotitem)
    {
        plotitem->setVisible(on);
    }

}

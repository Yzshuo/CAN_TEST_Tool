#-------------------------------------------------
#
# Project created by QtCreator 2020-08-22T15:09:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport concurrent svg printsupport opengl

TARGET = CAN_TEST_Tool
TEMPLATE = app


DEFINES += QWT_NO_SVG
DEFINES += QWT_NO_OPENGL
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        widget.cpp \
    can_parameter_config.cpp \
    can_thread.cpp \
    qwtdriver.cpp \
    QWT/qwt_abstract_legend.cpp \
    QWT/qwt_abstract_scale.cpp \
    QWT/qwt_abstract_scale_draw.cpp \
    QWT/qwt_abstract_slider.cpp \
    QWT/qwt_analog_clock.cpp \
    QWT/qwt_arrow_button.cpp \
    QWT/qwt_clipper.cpp \
    QWT/qwt_color_map.cpp \
    QWT/qwt_column_symbol.cpp \
    QWT/qwt_compass.cpp \
    QWT/qwt_compass_rose.cpp \
    QWT/qwt_counter.cpp \
    QWT/qwt_curve_fitter.cpp \
    QWT/qwt_date.cpp \
    QWT/qwt_date_scale_draw.cpp \
    QWT/qwt_date_scale_engine.cpp \
    QWT/qwt_dial.cpp \
    QWT/qwt_dial_needle.cpp \
    QWT/qwt_dyngrid_layout.cpp \
    QWT/qwt_event_pattern.cpp \
    QWT/qwt_graphic.cpp \
    QWT/qwt_interval.cpp \
    QWT/qwt_interval_symbol.cpp \
    QWT/qwt_knob.cpp \
    QWT/qwt_legend.cpp \
    QWT/qwt_legend_data.cpp \
    QWT/qwt_legend_label.cpp \
    QWT/qwt_magnifier.cpp \
    QWT/qwt_math.cpp \
    QWT/qwt_matrix_raster_data.cpp \
    QWT/qwt_null_paintdevice.cpp \
    QWT/qwt_painter.cpp \
    QWT/qwt_painter_command.cpp \
    QWT/qwt_panner.cpp \
    QWT/qwt_picker.cpp \
    QWT/qwt_picker_machine.cpp \
    QWT/qwt_pixel_matrix.cpp \
    QWT/qwt_plot.cpp \
    QWT/qwt_plot_abstract_barchart.cpp \
    QWT/qwt_plot_axis.cpp \
    QWT/qwt_plot_barchart.cpp \
    QWT/qwt_plot_canvas.cpp \
    QWT/qwt_plot_curve.cpp \
    QWT/qwt_plot_dict.cpp \
    QWT/qwt_plot_directpainter.cpp \
    QWT/qwt_plot_glcanvas.cpp \
    QWT/qwt_plot_grid.cpp \
    QWT/qwt_plot_histogram.cpp \
    QWT/qwt_plot_intervalcurve.cpp \
    QWT/qwt_plot_item.cpp \
    QWT/qwt_plot_layout.cpp \
    QWT/qwt_plot_legenditem.cpp \
    QWT/qwt_plot_magnifier.cpp \
    QWT/qwt_plot_marker.cpp \
    QWT/qwt_plot_multi_barchart.cpp \
    QWT/qwt_plot_panner.cpp \
    QWT/qwt_plot_picker.cpp \
    QWT/qwt_plot_rasteritem.cpp \
    QWT/qwt_plot_renderer.cpp \
    QWT/qwt_plot_rescaler.cpp \
    QWT/qwt_plot_scaleitem.cpp \
    QWT/qwt_plot_seriesitem.cpp \
    QWT/qwt_plot_shapeitem.cpp \
    QWT/qwt_plot_spectrocurve.cpp \
    QWT/qwt_plot_spectrogram.cpp \
    QWT/qwt_plot_svgitem.cpp \
    QWT/qwt_plot_textlabel.cpp \
    QWT/qwt_plot_tradingcurve.cpp \
    QWT/qwt_plot_xml.cpp \
    QWT/qwt_plot_zoneitem.cpp \
    QWT/qwt_plot_zoomer.cpp \
    QWT/qwt_point_3d.cpp \
    QWT/qwt_point_data.cpp \
    QWT/qwt_point_mapper.cpp \
    QWT/qwt_point_polar.cpp \
    QWT/qwt_raster_data.cpp \
    QWT/qwt_round_scale_draw.cpp \
    QWT/qwt_sampling_thread.cpp \
    QWT/qwt_scale_div.cpp \
    QWT/qwt_scale_draw.cpp \
    QWT/qwt_scale_engine.cpp \
    QWT/qwt_scale_map.cpp \
    QWT/qwt_scale_widget.cpp \
    QWT/qwt_series_data.cpp \
    QWT/qwt_slider.cpp \
    QWT/qwt_spline.cpp \
    QWT/qwt_symbol.cpp \
    QWT/qwt_system_clock.cpp \
    QWT/qwt_text.cpp \
    QWT/qwt_text_engine.cpp \
    QWT/qwt_text_label.cpp \
    QWT/qwt_thermo.cpp \
    QWT/qwt_transform.cpp \
    QWT/qwt_wheel.cpp \
    QWT/qwt_widget_overlay.cpp

HEADERS += \
        widget.h \
    can_parameter_config.h \
    can_thread.h \
    CAN/ControlCAN.h \
    qwtdriver.h \
    QWT/qwt.h \
    QWT/qwt_abstract_legend.h \
    QWT/qwt_abstract_scale.h \
    QWT/qwt_abstract_scale_draw.h \
    QWT/qwt_abstract_slider.h \
    QWT/qwt_analog_clock.h \
    QWT/qwt_arrow_button.h \
    QWT/qwt_clipper.h \
    QWT/qwt_color_map.h \
    QWT/qwt_column_symbol.h \
    QWT/qwt_compass.h \
    QWT/qwt_compass_rose.h \
    QWT/qwt_compat.h \
    QWT/qwt_counter.h \
    QWT/qwt_curve_fitter.h \
    QWT/qwt_date.h \
    QWT/qwt_date_scale_draw.h \
    QWT/qwt_date_scale_engine.h \
    QWT/qwt_dial.h \
    QWT/qwt_dial_needle.h \
    QWT/qwt_dyngrid_layout.h \
    QWT/qwt_event_pattern.h \
    QWT/qwt_global.h \
    QWT/qwt_graphic.h \
    QWT/qwt_interval.h \
    QWT/qwt_interval_symbol.h \
    QWT/qwt_knob.h \
    QWT/qwt_legend.h \
    QWT/qwt_legend_data.h \
    QWT/qwt_legend_label.h \
    QWT/qwt_magnifier.h \
    QWT/qwt_math.h \
    QWT/qwt_matrix_raster_data.h \
    QWT/qwt_null_paintdevice.h \
    QWT/qwt_painter.h \
    QWT/qwt_painter_command.h \
    QWT/qwt_panner.h \
    QWT/qwt_picker.h \
    QWT/qwt_picker_machine.h \
    QWT/qwt_pixel_matrix.h \
    QWT/qwt_plot.h \
    QWT/qwt_plot_abstract_barchart.h \
    QWT/qwt_plot_barchart.h \
    QWT/qwt_plot_canvas.h \
    QWT/qwt_plot_curve.h \
    QWT/qwt_plot_dict.h \
    QWT/qwt_plot_directpainter.h \
    QWT/qwt_plot_glcanvas.h \
    QWT/qwt_plot_grid.h \
    QWT/qwt_plot_histogram.h \
    QWT/qwt_plot_intervalcurve.h \
    QWT/qwt_plot_item.h \
    QWT/qwt_plot_layout.h \
    QWT/qwt_plot_legenditem.h \
    QWT/qwt_plot_magnifier.h \
    QWT/qwt_plot_marker.h \
    QWT/qwt_plot_multi_barchart.h \
    QWT/qwt_plot_panner.h \
    QWT/qwt_plot_picker.h \
    QWT/qwt_plot_rasteritem.h \
    QWT/qwt_plot_renderer.h \
    QWT/qwt_plot_rescaler.h \
    QWT/qwt_plot_scaleitem.h \
    QWT/qwt_plot_seriesitem.h \
    QWT/qwt_plot_shapeitem.h \
    QWT/qwt_plot_spectrocurve.h \
    QWT/qwt_plot_spectrogram.h \
    QWT/qwt_plot_svgitem.h \
    QWT/qwt_plot_textlabel.h \
    QWT/qwt_plot_tradingcurve.h \
    QWT/qwt_plot_zoneitem.h \
    QWT/qwt_plot_zoomer.h \
    QWT/qwt_point_3d.h \
    QWT/qwt_point_data.h \
    QWT/qwt_point_mapper.h \
    QWT/qwt_point_polar.h \
    QWT/qwt_raster_data.h \
    QWT/qwt_round_scale_draw.h \
    QWT/qwt_samples.h \
    QWT/qwt_sampling_thread.h \
    QWT/qwt_scale_div.h \
    QWT/qwt_scale_draw.h \
    QWT/qwt_scale_engine.h \
    QWT/qwt_scale_map.h \
    QWT/qwt_scale_widget.h \
    QWT/qwt_series_data.h \
    QWT/qwt_series_store.h \
    QWT/qwt_slider.h \
    QWT/qwt_spline.h \
    QWT/qwt_symbol.h \
    QWT/qwt_system_clock.h \
    QWT/qwt_text.h \
    QWT/qwt_text_engine.h \
    QWT/qwt_text_label.h \
    QWT/qwt_thermo.h \
    QWT/qwt_transform.h \
    QWT/qwt_wheel.h \
    QWT/qwt_widget_overlay.h

FORMS += \
        widget.ui \
    can_parameter_config.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




win32:LIBS += -L$$PWD/./ -lControlCAN


INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

SUBDIRS += \
    QWT/src.pro

DISTFILES +=

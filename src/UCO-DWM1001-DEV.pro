QT       += core gui serialport core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

QMAKE_CXXFLAGS += -Wa,-mbig-obj

RC_ICONS = DWM1001.ico

SOURCES += \
    console.cpp \
    dialog_about.cpp \
    dialog_alarm_dist.cpp \
    dialog_alarm_pos.cpp \
    dialog_csv.cpp \
    dialog_deviceconfig.cpp \
    dialog_dist_graph_config.cpp \
    dialog_mediafilter.cpp \
    dialog_pos_graph_config.cpp \
    dialog_stats.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    console.h \
    dialog_about.h \
    dialog_alarm_dist.h \
    dialog_alarm_pos.h \
    dialog_csv.h \
    dialog_deviceconfig.h \
    dialog_dist_graph_config.h \
    dialog_mediafilter.h \
    dialog_pos_graph_config.h \
    dialog_stats.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    dialog_about.ui \
    dialog_alarm_dist.ui \
    dialog_alarm_pos.ui \
    dialog_csv.ui \
    dialog_deviceconfig.ui \
    dialog_dist_graph_config.ui \
    dialog_mediafilter.ui \
    dialog_pos_graph_config.ui \
    dialog_stats.ui \
    mainwindow.ui

TRANSLATIONS += \
    UCO_DWM1001_DEV_en_GB.ts \
    UCO_DWM1001_DEV_es_SP.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
LIBS += -lpoppler-cpp -lpoppler-qt5

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cpuusage.cpp \
    filesystem.cpp \
    fsqtablewidget.cpp \
    infosize.cpp \
    infosizeqtablewidgetitem.cpp \
    loadcustomplot.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    modelqtablewidget.cpp \
    percentticker.cpp \
    procinfo.cpp \
    procqtablewidget.cpp \
    qcustomplot.cpp \
    squarelegenditem.cpp \
    worker.cpp

HEADERS += \
    cpuusage.h \
    filesystem.h \
    fsqtablewidget.h \
    infosize.h \
    infosizeqtablewidgetitem.h \
    loadcustomplot.h \
    mainwindow.h \
    model.h \
    modelqtablewidget.h \
    percentticker.h \
    procinfo.h \
    procqtablewidget.h \
    qcustomplot.h \
    squarelegenditem.h \
    worker.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    scandirectory.cpp \
    treeview.cpp \
    widget.cpp

HEADERS += \
    scandirectory.h \
    treeview.h \
    widget.h

INCLUDEPATH +=/home/semen/qtProjects/QtProjects/SumWidget/
INCLUDEPATH +=/home/semen/qtProjects/QtProjects/FileWidget/

# Default rules for deployment.S
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

#unix:!macx: LIBS += -L$$PWD/../build-SumWidget-Desktop_Qt_5_12_10_GCC_64bit-Debug/ -lSumWidget

#INCLUDEPATH += $$PWD/../SumWidget
#DEPENDPATH += $$PWD/../SumWidget

LIBS += -lboost_filesystem
#LIBS += -lboost_system
LIBS += -ldl

FORMS +=


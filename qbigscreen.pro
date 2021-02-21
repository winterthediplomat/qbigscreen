QT       += core gui network gamepad

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -Werror=switch

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bigscreencontroller.cpp \
    bigscreennetwork.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    bigscreen_defines.hpp \
    bigscreencontroller.h \
    bigscreenmodel.h \
    bigscreennetwork.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = $$[PREFIX]
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    dev-support/install-scripts.sh \
    scripts/launch-via-streamlink.py \
    scripts/start-mpv.sh

RESOURCES +=

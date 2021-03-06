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
    dev-support/test-lockfile-scripts.sh \
    scripts/activate-window-of-current-instance.sh \
    scripts/create-lockfile.sh \
    scripts/delete-lockfile.sh \
    scripts/launch-deezer.py \
    scripts/launch-mpv-cdda.py \
    scripts/launch-restart-qbigscreen.py \
    scripts/launch-steam.py \
    scripts/launch-use-pc.py \
    scripts/launch-via-streamlink.py \
    scripts/start-antimicrox.py \
    scripts/start-mpv.sh \
    scripts/start-or-switch-to-current-instance.py

RESOURCES +=

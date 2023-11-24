# **********************************************************
# * Author(作者)     : Qt君
# * 微信公众号        : Qt君
# * Website(网站)    : qthub.com
QT += gui widgets

# HTTP客户端源码下载 Source Code: https://github.com/aeagean/QtNetworkService
include($$PWD/../QtNetworkService/src/QtNetworkService.pri)

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CXXFLAGS += /utf-8
QMAKE_CFLAGS += /utf-8

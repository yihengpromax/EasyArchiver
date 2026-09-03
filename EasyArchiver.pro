QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DESTDIR = $$PWD/build/bin

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/SevenzipArchiver/easyarchiver.cpp

HEADERS += \
    $$PWD/src/mainwindow.h \
    $$PWD/src/SevenzipArchiver/easyarchiver.h \
    $$PWD/src/stdafx.h


FORMS += \
    $$PWD/src/mainwindow.ui


# bit7z
INCLUDEPATH += $$PWD/thrid_party/bit7z/include

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/thrid_party/bit7z/lib/x64/Debug/ -lbit7z   # Debug 版本的库
} else {
    LIBS += -L$$PWD/thrid_party/bit7z/lib/x64/Release/ -lbit7z    # Release 版本的库
}

# 不加上-lOleAut32 -lUser32这段 bit7z无法解析
LIBS += -L$$DESTDIR -lOleAut32 -lUser32


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# 基于7z实现

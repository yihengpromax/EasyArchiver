QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DESTDIR = $$PWD/build/bin

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/SevenzipArchiver/easyarchiver.cpp \
    src/Common/resourceloader.cpp \
    src/SevenzipArchiver/sevenzipcoreexecworker.cpp \
    src/dialog/archiverinfodlg.cpp \
    src/dialog/compressdlg.cpp \
    src/dialog/extractdlg.cpp \
    src/dialog/sevenzipcoreexecdlg.cpp

HEADERS += \
    $$PWD/src/mainwindow.h \
    $$PWD/src/SevenzipArchiver/easyarchiver.h \
    $$PWD/src/stdafx.h \
    src/Common/resourceloader.h \
    src/SevenzipArchiver/sevenzipcoreexecworker.h \
    src/dialog/archiverinfodlg.h \
    src/dialog/compressdlg.h \
    src/dialog/extractdlg.h \
    src/dialog/sevenzipcoreexecdlg.h


FORMS += \
    $$PWD/src/mainwindow.ui \
    src/dialog/archiverinfodlg.ui \
    src/dialog/compressdlg.ui \
    src/dialog/extractdlg.ui \
    src/dialog/sevenzipcoreexecdlg.ui


# bit7z
INCLUDEPATH += $$PWD/thrid_party/bit7z/include

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/thrid_party/bit7z/lib/x64/Debug/ -lbit7z   # Debug 版本的库
} else {
    LIBS += -L$$PWD/thrid_party/bit7z/lib/x64/Release/ -lbit7z    # Release 版本的库
}

# 不加上-lOleAut32 -lUser32这段 bit7z无法解析
LIBS += -L$$PWD/thrid_party/bit7z/lib/x64 -lOleAut32 -lUser32


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc


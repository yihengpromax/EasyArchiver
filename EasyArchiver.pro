QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/mainwindow.cpp

HEADERS += \
    $$PWD/src/mainwindow.h


FORMS += \
    $$PWD/src/mainwindow.ui

INCLUDEPATH += $$PWD/thrid_party/bit7z/include
DEPENDPATH += $$PWD/thrid_party/bit7z/include

# 不加上-lOleAut32 -lUser32这段 bit7z无法解析
LIBS += -L$$DESTDIR -lOleAut32 -lUser32

LIBS += -L$$PWD/thrid_party/bit7z/lib/x86/Debug/ -lbit7z
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thrid_party/bit7z/lib/x86/Release/ -lbit7z
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thrid_party/bit7z/lib/x86/Debug/ -lbit7z





# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# 基于7z实现

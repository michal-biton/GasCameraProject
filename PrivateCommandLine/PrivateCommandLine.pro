QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        HashTable.c \
        PrivateCommandLine.c \
        commandHandler.c \
        gpio.c \
        main.c \
        server.c

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    HashTable.h \
    PrivateCommandLine.h \
    commandHandler.h \
    gas_cam_lib.h \
    gpio.h \
    queue.h \
    server.h

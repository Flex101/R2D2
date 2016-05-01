TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -m64
QMAKE_LDFLAGS += -m64

DEFINES += NO_DEBUG

INCLUDEPATH += \
    Tools \
    Devices \

HEADERS += \
    R2CS.h \
    Tools/Logging.h \
    Tools/File.h \
    Tools/RealTime.h \
    Devices/Gamepad.h \
    Devices/LogitechCRP2.h \
    Devices/Audio.h \
    Devices/Device.h \
    Tools/WavFile.h \
    Tools/AudioLibrary.h \
    Tools/StringEx.h \
    Tools/CommonDefs.h \

SOURCES += \
    main.cpp \
    R2CS.cpp \
    Tools/Logging.cpp \
    Tools/File.cpp \
    Tools/RealTime.cpp \
    Devices/Gamepad.cpp \
    Devices/LogitechCRP2.cpp \
    Devices/Audio.cpp \
    Devices/Device.cpp \
    Tools/WavFile.cpp \
    Tools/AudioLibrary.cpp \
    Tools/StringEx.cpp

OTHER_FILES += \
    Resources/Audio/Positive/*.wav \
    Resources/Audio/Neutral/*.wav \
    Resources/Audio/Negative/*.wav \

LIBS += -lasound

resources.files = $$PWD/Resources/*
resources.path = $$OUT_PWD/Resources
INSTALLS += resources

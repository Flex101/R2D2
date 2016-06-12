TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= c++11

DEFINES += NO_DEBUG

INCLUDEPATH += \
    Tools \
    Devices \
    Controllers \

HEADERS += \
    R2CS.h \
    Tools/Logging.h \
    Tools/File.h \
    Tools/RealTime.h \
    Devices/Gamepad.h \
    Devices/LogitechCRP2.h \
    Devices/APlayAudio.h \
    Devices/Device.h \
    Tools/WavFile.h \
    Tools/AudioLibrary.h \
    Tools/StringEx.h \
    Tools/CommonDefs.h \
    Tools/SerialDeviceLibrary.h \
    Devices/SerialDevice.h \
    Devices/FootDrive.h \
    Tools/Process.h \
    Controllers/ThreeLegDrive.h \
    Controllers/TwoLegDrive.h \
    Controllers/DomeDrive.h \
    Controllers/LegDrive.h \
    Tools/JoystickMix.h \
    Devices/Dome.h

SOURCES += \
    main.cpp \
    R2CS.cpp \
    Tools/Logging.cpp \
    Tools/File.cpp \
    Tools/RealTime.cpp \
    Devices/Gamepad.cpp \
    Devices/LogitechCRP2.cpp \
    Devices/APlayAudio.cpp \
    Devices/Device.cpp \
    Tools/WavFile.cpp \
    Tools/AudioLibrary.cpp \
    Tools/StringEx.cpp \
    Tools/SerialDeviceLibrary.cpp \
    Devices/SerialDevice.cpp \
    Devices/FootDrive.cpp \
    Tools/Process.cpp \
    Controllers/ThreeLegDrive.cpp \
    Controllers/TwoLegDrive.cpp \
    Controllers/DomeDrive.cpp \
    Controllers/LegDrive.cpp \
    Tools/JoystickMix.cpp \
    Devices/Dome.cpp

OTHER_FILES += \
    Resources/Audio/Positive/*.wav \
    Resources/Audio/Neutral/*.wav \
    Resources/Audio/Negative/*.wav \
    build.sh \
    shutdown.sh \

LIBS += -lasound
#LIBS += -lpulse
#LIBS += -lpulse-simple

resources.files = $$PWD/Resources/*
resources.path = $$OUT_PWD/Resources
INSTALLS += resources

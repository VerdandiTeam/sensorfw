CONFIG += link_pkgconfig

TARGET = motoglancesensor

HEADERS += motoglancesensor.h \
           motoglancesensor_a.h \
           motoglanceplugin.h

SOURCES += motoglancesensor.cpp \
           motoglancesensor_a.cpp \
           motoglanceplugin.cpp

include( ../sensor-config.pri )

contextprovider {
    DEFINES += PROVIDE_CONTEXT_INFO
    PKGCONFIG += contextprovider-1.0
}

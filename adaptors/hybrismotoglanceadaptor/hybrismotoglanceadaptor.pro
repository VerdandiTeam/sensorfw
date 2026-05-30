TARGET = hybrismotoglanceadaptor

HEADERS += hybrismotoglanceadaptor.h \
           hybrismotoglanceadaptorplugin.h

SOURCES += hybrismotoglanceadaptor.cpp \
           hybrismotoglanceadaptorplugin.cpp
LIBS += -L../../core -lhybrissensorfw-qt$${QT_MAJOR_VERSION}

include( ../adaptor-config.pri )
config_hybris {
    PKGCONFIG += android-headers
}

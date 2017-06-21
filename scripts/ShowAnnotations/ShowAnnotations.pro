TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

LIBS += `pkg-config opencv --libs`		# so files
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lxml2

INCLUDEPATH += -I "/usr/local/include/opencv2" 	# headers
INCLUDEPATH += -I "/usr/include/libxml2/"


SOURCES += main.cpp \
    OpenXml.cpp \

HEADERS += \
    OpenXml.h \

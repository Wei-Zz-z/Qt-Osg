QT += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += D:/OSG/OSG-VS2017-msvc64/install/include
DEPENDPATH += D:/OSG/OSG-VS2017-msvc64/install/include

win32:CONFIG(release, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgQOpenGL
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgQOpenGLd

win32:CONFIG(release, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgViewer
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgViewerd

win32:CONFIG(release, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgGA
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgGAd


win32:CONFIG(release, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgDB
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgDBd

win32:CONFIG(release, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losg
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OSG/OSG-VS2017-msvc64/install/lib/ -losgd


SOURCES += \
    main.cpp \
    osgtopng.cpp \
    widget.cpp

HEADERS += \
    osgtopng.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

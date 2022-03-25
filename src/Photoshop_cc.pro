QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -L/usr/local/Cellar/opencv/4.5.5/lib \
        -lopencv_highgui \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lopencv_videoio \


INCLUDEPATH += /usr/local/Cellar/opencv/4.5.5/include/opencv4


SOURCES += \
    main.cpp \
    photoshop.cpp \
    scribblearea.cpp \
    text_editor.cpp

HEADERS += \
    photoshop.h \
    scribblearea.h \
    text_editor.h

FORMS += \
    photoshop.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

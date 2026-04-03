QT       += core gui network sql

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

#头文件
INCLUDEPATH +=D:\D2\OpenCv\SeetaFace\include D:\D2\OpenCv\SeetaFace\include\seeta
INCLUDEPATH +=D:\D2\OpenCv\opencv452\include D:\D2\OpenCv\opencv452\include\opencv2
#库文件(编译库)
LIBS += -L"D:/D2/OpenCv/opencv452/x64/mingw/lib" -lopencv_core452 -lopencv_imgproc452 -lopencv_imgcodecs452 -lopencv_highgui452 -lopencv_videoio452
LIBS += -L"D:/D2/OpenCv/SeetaFace/lib" -lSeetaFaceDetector -lSeetaFaceLandmarker -lSeetaFaceRecognizer -lSeetaNet -lSeetaQualityAssessor



SOURCES += \
    adminwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    regauditdialog.cpp

HEADERS += \
    adminwindow.h \
    mainwindow.h \
    regauditdialog.h

FORMS += \
    adminwindow.ui \
    mainwindow.ui \
    regauditdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#Windows
win32{
INCLUDEPATH += c:/usr/include #opencv includepath

Debug : {
LIBS += -lc:/usr/lib/x86/vc15/lib/opencv_world412d
}
Release : {
LIBS += -lc:/usr/lib/x86/vc15/lib/opencv_world412
}
}

#Unix
unix{
INCLUDEPATH += /usr/include/opencv4 #opencv includepath

LIBS += -L/usr/lib #opencv lib directory

#requiered libs
LIBS += -lopencv_core \
        -lopencv_imgproc \
        -lopencv_features2d \
        -lopencv_imgcodecs

}

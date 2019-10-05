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
INCLUDEPATH += /usr/include #opencv includepath

LIBS += -L/usr/lib #opencv lib directory

#requiered libs
LIBS += -l/lib/opencv_core \
        -l/lib/opencv_improc \
        -l/lib/opencv_feature2d
        -l/lib/opencv_imcodec

}

#Windows
win32{
INCLUDEPATH += c:/usr/include #CHANGE ME (Include Directory)

Debug : {
LIBS += -lc:/usr/lib/x86/vc15/lib/opencv_world412d #CHANGE ME (OpenCV Debug lib full path)
}
Release : {
LIBS += -lc:/usr/lib/x86/vc15/lib/opencv_world412 #CHANGE ME (OpenCV Release lib full path)
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

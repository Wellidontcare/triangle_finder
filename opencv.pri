#Windows
win32{
INCLUDEPATH += c:/usr/include/opencv4 #CHANGE ME (Include Directory)

LIBS += -Lc:/usr/lib/opencv4/x86 #CHANGE ME (OpenCV Debug lib full path)
Debug : {
LIBS += -lopencv_world412d
}
Release : {
LIBS += -lopencv_world412 #CHANGE ME (OpenCV Release lib full path)
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

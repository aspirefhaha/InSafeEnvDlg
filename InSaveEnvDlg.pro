######################################################################
# Automatically generated by qmake (2.01a) ?? ?? 12 21:20:45 2020
######################################################################

TEMPLATE = app
TARGET = 
QMAKE_LFLAGS *= /MACHINE:X64
DEFINES += "_MBCS"
DEPENDPATH += . GeneratedFiles
INCLUDEPATH += . \
		../InEnvDirPlugin \
		D:/VBox/VirtualBox-5.0.0/src/VBox/Additions/WINNT/FhahaLib

LIBS += -L../InEnvDirPlugin/debug \
		InEnvDirPlugin.lib

!contains(QMAKE_TARGET.arch, x86_64) {
    message("x86_64 build")
    ## Windows x64 (64bit) specific build here       
} else {
    message("x86 build")
    ## Windows x86 (32bit) specific build here
}
# Input
HEADERS += CFriendViewModel.h HistoryModel.h insaveenvdlg.h qoutenvfsmodel.h
FORMS += insaveenvdlg.ui
SOURCES += CFriendViewModel.cpp \
           HistoryModel.cpp \
           insaveenvdlg.cpp \
           main.cpp \
           qoutenvfsmodel.cpp
RESOURCES += insaveenvdlg.qrc
TRANSLATIONS += zh_CN.ts

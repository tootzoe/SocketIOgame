




#TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#
#
PRJNAMETOOT = SocketIOgame
DEFINES += "SOCKETIOGAME_API="
DEFINES += "SOCKETIOGAME_API(...)="
#
DEFINES += "UCLASS()=SOCKETIOGAME_API"
DEFINES += "UCLASS(...)=SOCKETIOGAME_API"
#
# this is true during development with unreal-editor...

DEFINES += "WITH_EDITORONLY_DATA=1"

## this project only
DEFINES += "WITH_WEBSOCKETS"



INCLUDEPATH += ../Intermediate/Build/Win64/UnrealEditor/Inc/$$PRJNAMETOOT/UHT
#INCLUDEPATH += $$PRJNAMETOOT $$PRJNAMETOOT/Public $$PRJNAMETOOT/Private #don't use this,
# we should follow UE project struct to include files, start from prj.Build.cs folder
#
#  The Thirdparty libs
#    INCLUDEPATH += ../ThirdpartyLibs/FMOD/inc
#
#
include(defs.pri)
include(inc.pri)
#
#

DISTFILES += \
    SocketIOgame.Target.cs \
    SocketIOgame/SocketIOgame.Build.cs \
    SocketIOgameEditor.Target.cs

HEADERS += \
    SocketIOgame/Public/SocketIOManager.h \
    SocketIOgame/SocketIOgame.h

SOURCES += \
    SocketIOgame/Private/SocketIOManager.cpp \
    SocketIOgame/SocketIOgame.cpp









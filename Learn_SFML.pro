TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG -= console
#CONFIG += console

LIBS += -LD:\Biblioteki\SFML-2.4.2\lib

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

#CONFIG(release, debug|release): LIBS += -lsfml-audio-s -lsfml-graphics-s -lsfml-network-s -lsfml-window-s -lsfml-system-s -lopengl32 -lglu32 -lwinmm -ljpeg -lfreetype -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32
#DEFINES += SFML_STATIC


INCLUDEPATH += D:\Biblioteki\SFML-2.4.2\include
DEPENDPATH += D:\Biblioteki\SFML-2.4.2\include


SOURCES += main.cpp \
    window.cpp \
    game.cpp \
    eventmanager.cpp \
    basestate.cpp \
    statemanager.cpp \
    spritesheet.cpp \
    anim_base.cpp \
    anim_directional.cpp \
    gui_element.cpp \
    gui_interface.cpp \
    gui_manager.cpp \
    gui_label.cpp \
    gui_textfield.cpp \
    gui_scrollbar.cpp \
    state_mainmenu.cpp

HEADERS += \
    window.h \
    game.h \
    textbox.h \
    eventmanager.h \
    basestate.h \
    statemanager.h \
    resourcemanager.h \
    utils.h \
    texturemanager.h \
    spritesheet.h \
    anim_base.h \
    anim_directional.h \
    directions.h \
    fontmanager.h \
    gui_element.h \
    gui_event.h \
    gui_interface.h \
    gui_manager.h \
    gui_label.h \
    gui_textfield.h \
    gui_scrollbar.h \
    sharedcontext.h \
    state_mainmenu.h

DISTFILES += \
    Data/keys.cfg \
    Data/MainMenu.interface \
    Data/MainMenu1.style \
    Data/MainMenu2.style \
    Data/MainMenu3.style \
    Data/Textures/Textures.cfg

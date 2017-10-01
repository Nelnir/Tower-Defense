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
    state_mainmenu.cpp \
    state_about.cpp \
    settings.cpp \
    state_settings.cpp \
    state_game.cpp \
    state_selectlevel.cpp \
    level.cpp \
    towermanager.cpp \
    abstractenemy.cpp \
    enemymanager.cpp \
    state_gameover.cpp \
    abstracttower.cpp \
    bulletmanager.cpp \
    bullet.cpp \
    tank_enemy.cpp

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
    state_mainmenu.h \
    state_about.h \
    settings.h \
    state_settings.h \
    state_game.h \
    state_selectlevel.h \
    level.h \
    abstracttower.h \
    towermanager.h \
    abstractenemy.h \
    enemymanager.h \
    state_gameover.h \
    statistics.h \
    bulletmanager.h \
    bullet.h \
    tank_enemy.h

DISTFILES += \
    Data/keys.cfg \
    Data/MainMenu.interface \
    Data/Textures/Textures.cfg \
    Data/Styles/About.style \
    Data/Styles/AboutText.style \
    Data/Styles/AboutText2.style \
    Data/Styles/MainMenu1.style \
    Data/Styles/MainMenu2.style \
    Data/Styles/MainMenu3.style \
    Data/Interfaces/About.interface \
    Data/Interfaces/MainMenu.interface \
    Data/Interfaces/Settings.interface \
    Data/Interfaces/SelectLevel.interface \
    Data/Styles/LeftArrow.style \
    Data/Styles/RightArrow.style \
    Data/Interfaces/SelectLevel-Levels.interface \
    Data/Styles/SelectLevels.style \
    Data/Styles/Level1.style \
    Data/Styles/Level2.style \
    Data/Styles/Level3.style \
    Data/Levels/Level-1.level \
    Data/tiles.cfg \
    Data/Interfaces/Game.interface \
    Data/Styles/Game.style \
    Data/Styles/GameTitle.style \
    Data/Styles/Tower1.style \
    Data/Styles/Start.style \
    Data/Enemies/Enemies.cfg \
    Data/Enemies/Soldier.proporties \
    Data/Styles/GameText.style \
    Data/Enemies/Soldier2.proporties \
    Data/Interfaces/GameOver.interface \
    Data/Styles/GameOver.style \
    Data/Styles/lost.style \
    Data/Interfaces/Statistics.interface \
    Data/Styles/Statistics.style \
    Data/Interfaces/GameOverWin.interface \
    Data/Styles/win.style \
    Data/Styles/GameTextSpeed.style \
    Data/Styles/AboutText2Clickable.style \
    Data/Enemies/Tank.proporties \
    Data/Interfaces/TowerUpgrade.interface \
    Data/Styles/TowerUpgradeInterface.style \
    Data/Styles/UpgradeUpgradeButton.style \
    Data/Styles/UpgradeCloseButton.style \
    Data/Styles/GameTextClickable.style \
    Data/Styles/GameTextNext.style

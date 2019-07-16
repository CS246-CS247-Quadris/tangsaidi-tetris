QT += widgets
QMAKE_CC = gcc
QMAKE_CXX = g++
CONFIG += c++14

HEADERS       = tetrixboard.h \
                tetrixpiece.h \
                tetrixwindow.h \
                game.h \
                board.h \
                block.h \
                row.h \
                pixel.h \
                level.h \
                score.h \
                levelZero.h \
                levelOne.h \
                levelTwo.h \
                levelThree.h \
                levelFour.h \
                settler.h

SOURCES       = main.cc \
                tetrixboard.cpp \
                tetrixpiece.cpp \
                tetrixwindow.cpp \
                game.cc \
                board.cc \
                block.cc \
                row.cc \
                pixel.cc \
                level.cc \
                score.cc \
                levelZero.cc \
                levelOne.cc \
                levelTwo.cc \
                levelThree.cc \
                levelFour.cc \
                settler.cc

# install
target.path = Projects/Qt
INSTALLS += target

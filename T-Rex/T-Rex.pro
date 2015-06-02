#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T19:48:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app

INCLUDEPATH += /usr/include/graphviz

LIBS += -lgvc -lcdt -lcgraph

QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp\
    AutomatParser/TreeNodes.cpp \
    Gui/uvodframe.cpp \
    Gui/editorframe.cpp \
    Gui/diagramframe.cpp \
    Gui/mainwindow.cpp \
    Gui/automatframe.cpp \
    Engine/editorprocess.cpp \
    AutomatParser/automata.cpp \
    AutomatParser/visitor_nodes.cpp \
    Engine/automatprocess.cpp \
    Gui/guibuilder.cpp \
    AutomatParser/parserengine.cpp \
    AutomatParser/graphview.cpp \
    Gui/dragwidget.cpp \
    Gui/acceptwidget.cpp \
    AutomatParser/kretanje.cpp \
    Gui/diagram.cpp

HEADERS  += \
    AutomatParser/TreeNodes.hpp \
    Gui/uvodframe.h \
    Gui/editorframe.h \
    Gui/diagramframe.h \
    Gui/mainwindow.h \
    Gui/automatframe.h \
    Engine/editorprocess.h \
    AutomatParser/automata.h \
    AutomatParser/visitor_nodes.h \
    Engine/automatprocess.h \
    Gui/guibuilder.h \
    AutomatParser/parserengine.h \
    AutomatParser/graphview.h \
    Gui/dragwidget.h \
    Gui/acceptwidget.h \
    AutomatParser/kretanje.h \
    Gui/diagram.h

FORMS    += \
    Gui/uvodframe.ui \
    Gui/editorframe.ui \
    Gui/diagramframe.ui \
    Gui/mainwindow.ui \
    Gui/automatframe.ui

DISTFILES +=

RESOURCES += \
    images.qrc


#ovaj deo make-a odnosi se na lex i bison

LIBS += -lfl -ly

FLEXSOURCES = AutomatParser/lexer.l
BISONSOURCES = AutomatParser/parser.y

flexsource.input = FLEXSOURCES
flexsource.output = ${QMAKE_FILE_BASE}.cpp
flexsource.commands = flex --header-file=${QMAKE_FILE_BASE}.h -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
flexsource.variable_out = SOURCES
flexsource.name = Flex Sources ${QMAKE_FILE_IN}
flexsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += flexsource

flexheader.input = FLEXSOURCES
flexheader.output = ${QMAKE_FILE_BASE}.h
flexheader.commands = @true
flexheader.variable_out = HEADERS
flexheader.name = Flex Headers ${QMAKE_FILE_IN}
flexheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += flexheader

bisonsource.input = BISONSOURCES
bisonsource.output = ${QMAKE_FILE_BASE}.cpp
bisonsource.commands = bison -d --defines=${QMAKE_FILE_BASE}.h -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
bisonsource.variable_out = SOURCES
bisonsource.name = Bison Sources ${QMAKE_FILE_IN}
bisonsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += bisonsource

bisonheader.input = BISONSOURCES
bisonheader.output = ${QMAKE_FILE_BASE}.h
bisonheader.commands = @true
bisonheader.variable_out = HEADERS
bisonheader.name = Bison Headers ${QMAKE_FILE_IN}
bisonheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += bisonheader

# kraj za lex i bison

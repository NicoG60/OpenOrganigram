INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/muParser.h \
    $$PWD/muParserBase.h \

SOURCES += \
    $$PWD/muParserBase.cpp \
    $$PWD/muParserBytecode.cpp \
    $$PWD/muParserCallback.cpp \
    $$PWD/muParserComplex.cpp \
    $$PWD/muParserDLL.cpp \
    $$PWD/muParserError.cpp \
    $$PWD/muParserInt.cpp \
    $$PWD/muParserTest.cpp \
    $$PWD/muParserTokenReader.cpp \
    $$PWD/muParser.cpp

HEADERS += \
    $$PUBLIC_HEADERS \
    $$PWD/muParserBytecode.h \
    $$PWD/muParserCallback.h \
    $$PWD/muParserComplex.h \
    $$PWD/muParserDef.h \
    $$PWD/muParserDLL.h \
    $$PWD/muParserError.h \
    $$PWD/muParserFixes.h \
    $$PWD/muParserInt.h \
    $$PWD/muParserStack.h \
    $$PWD/muParserTest.h \
    $$PWD/muParserToken.h \
    $$PWD/muParserTokenReader.h


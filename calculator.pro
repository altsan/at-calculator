CONFIG += map

os2:QMAKE_CFLAGS   = -Zomf -march=i686 -Wno-pointer-sign
os2:QMAKE_CXXFLAGS = -Zomf -march=i686 -Wno-unused-local-typedefs -Wno-literal-suffix -D OS2EMX_PLAIN_CHAR

TEMPLATE = app
TARGET = atcalc
HEADERS       = button.h \
                calculator.h
SOURCES       = button.cpp \
                calculator.cpp \
                main.cpp
RESOURCES    += calculator.qrc

os2:HEADERS += os2native.h
os2:SOURCES += os2native.cpp
os2:RC_FILE   = calculator.rc
win32:RC_FILE = calculator_win.rc

TRANSLATIONS = 	language/atcalc_de.ts \
		language/atcalc_es.ts \
		language/atcalc_fr.ts \
		language/atcalc_it.ts \
		language/atcalc_ja.ts \
		language/atcalc_nl.ts \
		language/atcalc_ru.ts \
		language/atcalc_zh_cn.ts \
		language/atcalc_zh_tw.ts

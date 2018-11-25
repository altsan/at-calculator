HEADERS       = button.h \
                calculator.h
SOURCES       = button.cpp \
                calculator.cpp \
                main.cpp

os2:QMAKE_CXXFLAGS += -Wno-unused-local-typedefs

os2:RC_FILE   = calculator.rc
win32:RC_FILE = calculator_win.rc



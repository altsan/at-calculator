HEADERS       = button.h \
                calculator.h
SOURCES       = button.cpp \
                calculator.cpp \
                main.cpp

os2:RC_FILE = calculator.rc


os2:QMAKE_CXXFLAGS += -Wno-unused-local-typedefs

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/calculator
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS calculator.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/calculator
INSTALLS += target sources

symbian {
    TARGET.UID3 = 0xA000C602
    include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
}

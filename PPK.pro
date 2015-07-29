TEMPLATE = lib
VERSION = 0.1
CONFIG -= qt
CONFIG += c++11
CONFIG += release

DESTDIR = Build
OBJECTS_DIR = $$DESTDIR

VPATH += src

SOURCES += \
    Node.cpp \
    Node.tpp \
    NodeIterators.tpp \
    IFileIterator.cpp \
    FS.cpp

HEADERS += \
    Node.hpp \
    utility.hpp \
    NodeIterators.hpp \
    StandardConverters.hpp \
    IFileIterator.hpp \
    FS.hpp

LIBS += \
    -lboost_filesystem \
    -lboost_system


public_headers.files =  \
    Node.hpp \
    FS.hpp \
    NodeIterators.hpp \
    StandardConverters.hpp \
    Node.tpp \
    NodeIterators.tpp


public_headers.path = Built/include/PPK
target.path = Built/lib

INSTALLS +=  target public_headers

DISTFILES += \
    README.md
    
dox.target = doc
dox.commands = (cat Doxyfile) | doxygen -

QMAKE_EXTRA_TARGETS += dox

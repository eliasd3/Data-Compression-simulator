QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

LIBS += -lz
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -march=haswell

INCLUDEPATH += /path/to/zstd/include
LIBS += -L/path/to/zstd/lib -lzstd

SOURCES += \
    ../../../Documents/sprintz/delta.cpp \
    ../../../Documents/sprintz/entropy.cpp \
    ../../../Documents/sprintz/format.cpp \
    ../../../Documents/sprintz/online.cpp \
    ../../../Documents/sprintz/predict.cpp \
    ../../../Documents/sprintz/sprintz.cpp \
    ../../../Documents/sprintz/sprintz_delta.cpp \
    ../../../Documents/sprintz/sprintz_delta_lowdim.cpp \
    ../../../Documents/sprintz/sprintz_delta_rle.cpp \
    ../../../Documents/sprintz/sprintz_delta_rle_query.cpp \
    ../../../Documents/sprintz/sprintz_xff.cpp \
    ../../../Documents/sprintz/sprintz_xff_lowdim.cpp \
    ../../../Documents/sprintz/sprintz_xff_rle.cpp \
    ../../../Documents/sprintz/sprintz_xff_rle_query.cpp \
    ../../../Documents/sprintz/univariate_8b.cpp \
    compressor.cpp \
    exportwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mytabpage.cpp \
    mythread.cpp \
    plotpertab.cpp \
    qcustomplot.cpp

HEADERS += \
    ../../../Documents/sprintz/bitpack.h \
    ../../../Documents/sprintz/debug_utils.hpp \
    ../../../Documents/sprintz/delta.h \
    ../../../Documents/sprintz/entropy.hpp \
    ../../../Documents/sprintz/format.h \
    ../../../Documents/sprintz/macros.h \
    ../../../Documents/sprintz/nn_search.hpp \
    ../../../Documents/sprintz/nn_utils.hpp \
    ../../../Documents/sprintz/online.hpp \
    ../../../Documents/sprintz/predict.h \
    ../../../Documents/sprintz/query.hpp \
    ../../../Documents/sprintz/search.hpp \
    ../../../Documents/sprintz/sprintz.h \
    ../../../Documents/sprintz/sprintz_delta.h \
    ../../../Documents/sprintz/sprintz_delta_rle_query.hpp \
    ../../../Documents/sprintz/sprintz_xff.h \
    ../../../Documents/sprintz/sprintz_xff_rle_query.hpp \
    ../../../Documents/sprintz/traits.hpp \
    ../../../Documents/sprintz/transpose.h \
    ../../../Documents/sprintz/univariate_8b.h \
    ../../../Documents/sprintz/util.h \
    compressor.h \
    exportwindow.h \
    mainwindow.h \
    mytabpage.h \
    mythread.h \
    plotpertab.h \
    qcustomplot.h

FORMS += \
    exportwindow.ui \
    mainwindow.ui \
    mytabpage.ui \
    plotpertab.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

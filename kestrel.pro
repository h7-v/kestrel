QT       += core gui

INCLUDEPATH += /usr/local/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# LIBS += -stdlib=libstdc++

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ext/sha256/sha256.cc \
    src/block.cc \
    src/blockchain.cc \
    src/checkerthread.cc \
    src/dbaccess.cc \
    src/kestrel.cc \
    src/main.cc \
    src/merkle.cc \
    src/minerthread.cc \
    src/tempblock.cc \
    src/transaction.cc \
    src/wallet.cc

HEADERS += \
    ext/sha256/sha256.h \
    src/block.h \
    src/blockchain.h \
    src/checkerthread.h \
    src/dbaccess.h \
    src/kestrel.h \
    src/merkle.h \
    src/minerthread.h \
    src/tempblock.h \
    src/transaction.h \
    src/wallet.h

FORMS += \
    kestrel.ui

# QMAKE_CFLAGS += -fno-rtti

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ext/leveldb/1.23/lib/release/ -lleveldb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ext/leveldb/1.23/lib/debug/ -lleveldb
else:unix: LIBS += -L$$PWD/ext/leveldb/1.23/lib/ -lleveldb

INCLUDEPATH += $$PWD/ext/leveldb/1.23/include
DEPENDPATH += $$PWD/ext/leveldb/1.23/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ext/leveldb/1.23/lib/release/libleveldb.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ext/leveldb/1.23/lib/debug/libleveldb.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ext/leveldb/1.23/lib/release/leveldb.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ext/leveldb/1.23/lib/debug/leveldb.lib
else:unix: PRE_TARGETDEPS += $$PWD/ext/leveldb/1.23/lib/libleveldb.a

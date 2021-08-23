#include "src/minerthread.h"
#include <QtCore>
#include <string>

MinerThread::MinerThread(QObject *parent)
    : QThread(parent) {}

MinerThread::~MinerThread() {}

void MinerThread::setProcessChain(Blockchain *b) {
    processChain = b;
}

void MinerThread::run() {
    while (processChain->getIsMining()) {
        if (this->stop) break;
        usleep(100000);  // TODO(matt): wait 0.1s for debugging. COMMENT OUT FOR RELEASE.
        if (processChain->bufferBlockContainsData()) {
            processChain->createBlock();

            emit mineFinished();
        }
    }
}

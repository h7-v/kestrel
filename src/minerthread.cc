#include "src/minerthread.h"
#include <QtCore>
#include <string>

MinerThread::MinerThread(QObject *parent)
    : QThread(parent) {}

MinerThread::~MinerThread() {}

void MinerThread::setProcessChain(Blockchain *b) {
    process_chain_ = b;
}

void MinerThread::run() {
    while (process_chain_->getIsMining()) {
        if (this->stop_) break;
        usleep(100000);  // TODO(matt): wait 0.1s for debugging. COMMENT OUT FOR RELEASE.
        if (process_chain_->bufferBlockContainsData()) {
            process_chain_->createBlock();

            emit mineFinished();
        }
    }
}

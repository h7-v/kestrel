#include "checkerthread.h"
#include <iostream>

CheckerThread::CheckerThread(QObject *parent)
    : QThread(parent) {}

CheckerThread::~CheckerThread() {}

void CheckerThread::setFilePath(std::string dbFilePath) {
    cthread_block_db_file_path = dbFilePath;
}

void CheckerThread::run() {
    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = false;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, cthread_block_db_file_path, &blockdb);

    assert(blockdbStatus.ok());

    leveldb::Iterator *it = blockdb->NewIterator(leveldb::ReadOptions());
    it->SeekToLast();

    std::string valueStore;
    while (valueStore[0] != '1') {
        valueStore = it->value().ToString();
        usleep(100000);  // Wait for 0.1s.
    }

    delete blockdb;

    emit previousBlockInMinedState();
}

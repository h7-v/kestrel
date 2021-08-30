#include "src/kestrel.h"
#include "ui_kestrel.h"
#include "src/dbaccess.h"
#include <string>
#include <iomanip>
// #include <QPixmap>
// #include <QFile>
#include "leveldb/db.h"

Kestrel::Kestrel(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Kestrel) {
    ui->setupUi(this);

//    QPixmap logoPix(":/files/images/k-1k1k.png");
//    int w = ui->logoLabel->width();
//    int h = ui->logoLabel->height();
//    ui->logoLabel->setPixmap(logoPix.scaled(w,h,Qt::KeepAspectRatio));

//    QFile file(":/files/logForTextWin.txt");
}

Kestrel::~Kestrel() {
    delete ui;
}

void Kestrel::closeEvent(QCloseEvent *event) {
    if (mthread_ != nullptr) {
        mthread_->stop_ = true;
        mthread_->wait();
        delete mthread_;
        bchain_->stopBlockchainMining();
    }

    event->accept();
}

void Kestrel::on_startButton_clicked() {
    Blockchain *b = new Blockchain("/tmp/kestrelBLOCKDB", "/tmp/kestrelTRANSACTIONSDB");
    bchain_ = b;

    if (bchain_->getBChainObjectExists()) {
        ui->textBrowser->append("Blockchain Created");

        ui->textBrowser->append(
                    "Block Database Status: " +
                    QString::fromStdString(bchain_->getChainBlockDBStatus()));

        ui->textBrowser->append(
                    "Transaction Database Status: " +
                    QString::fromStdString(bchain_->getChainTransactionDBStatus() + "\n"));

        ui->textBrowser->append("Genesis Block: ");
        ui->textBrowser->append(QString::fromStdString(
                                    bchain_->getBlockDBContents("00000000")));

        ui->textBrowser->append("LAST BLOCK ON LOCAL SYSTEM: ");
        ui->textBrowser->append(QString::fromStdString(
                                    bchain_->getBlockDBContents()));

        ui->startButton->setEnabled(false);
        ui->addTxButton->setEnabled(true);
        ui->mineButton->setEnabled(true);
    } else {
        ui->textBrowser->append("Blockchain Creation Failed...\n");
    }
}

void Kestrel::on_addTxButton_clicked() {
//    bchain_->executeTransaction("fromMae", "toHuisi", 50);
    bchain_->executeTransaction("fromLola", "toSydney", 120);
    bchain_->transactionsToBlockBuffer();

    ui->textBrowser->append(
                QString::fromStdString(bchain_->getBlockDataOnly() + "\n"));
}

void Kestrel::onMineFinished() {
    ui->textBrowser->append(
                QString::fromStdString(bchain_->getBlockDBContents()));
}

void Kestrel::on_mineButton_toggled(bool checked) {
    if (checked) {
        bchain_->runBlockchainMining();
        mthread_ = new MinerThread(this);
        mthread_->setProcessChain(bchain_);

        ui->mineButton->setText("Stop Mining");

        connect(mthread_, &MinerThread::mineFinished,
                this, &Kestrel::onMineFinished);

        mthread_->start();
    } else if (!checked) {
        mthread_->stop_ = true;
        mthread_->wait();
        delete mthread_;  // Deletes the object.
        bchain_->stopBlockchainMining();
        mthread_ = nullptr;  // Sets the object pointer to null so that the program can close properly with closeEvent().

        ui->mineButton->setText("Start Mining");
    }
}


void Kestrel::on_debugButton_clicked() {

    /*
    std::string latest_key;
    std::string latest_value;

    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = false;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, "/tmp/kestrelBLOCKDB", &blockdb);

    assert(blockdbStatus.ok());
    leveldb::Iterator *it = blockdb->NewIterator(leveldb::ReadOptions());

    for (it->SeekToFirst(); it->Valid(); it->Next()) {
      ui->textBrowser->append(
                  QString::fromStdString(it->key().ToString() + ": "  + it->value().ToString()));
    }
    delete it;
    delete blockdb;
    */

//    bchain_->getLatestTXInVectorAndDB();
    bchain_->getTxCount();

}

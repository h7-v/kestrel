#include "src/kestrel.h"
#include "ui_kestrel.h"
#include "src/dbaccess.h"
#include <string>
#include <iomanip>
// #include <QPixmap>
// #include <QFile>
#include "leveldb/db.h"

#include "src/wallet.h"

Kestrel::Kestrel(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Kestrel) {
    ui->setupUi(this);

//    QPixmap logoPix(":/files/images/k-1k1k.png");
//    int w = ui->logoLabel->width();
//    int h = ui->logoLabel->height();
//    ui->logoLabel->setPixmap(logoPix.scaled(w,h,Qt::KeepAspectRatio));

//    QFile file(":/files/logForTextWin.txt");

    Wallet *w = new Wallet();
    wallet_ = w;
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

    delete wallet_;
    wallet_ = nullptr;
    delete bchain_;
    bchain_ = nullptr;

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
//    bchain_->transactionsToBlockBuffer();

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

}

void Kestrel::on_generateKButton_clicked()
{
    std::string private_key = wallet_->generatePrivateKey();
    ui->pkeyTextBrowser->setText(QString::fromStdString(private_key));

    ui->adrTextBrowser->setText(QString::fromStdString(wallet_->
                                       computeWalletAddress(private_key)));

    private_key = "";
}


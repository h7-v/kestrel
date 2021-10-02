#include "src/kestrel.h"
#include "ui_kestrel.h"
#include "src/dbaccess.h"
#include <unistd.h>
#include <string>
#include <iomanip>
// #include <QPixmap>
// #include <QFile>
#include "leveldb/db.h"

#include "src/wallet.h"

Kestrel::Kestrel(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Kestrel) {
    ui->setupUi(this);

    this->setFixedSize(QSize(1280, 1000));

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
        ui->textBrowser->append("Blockchain Process Created");

        ui->textBrowser->append(
                    "Block Database Status: " +
                    QString::fromStdString(bchain_->getChainBlockDBStatus()));

        ui->textBrowser->append(
                    "Transaction Database Status: " +
                    QString::fromStdString(bchain_->getChainTransactionDBStatus() + "\n"));

//        ui->textBrowser->append("Genesis Block: ");
//        ui->textBrowser->append(QString::fromStdString(
//                                    bchain_->getBlockDBContents("00000000")));

        ui->textBrowser->append("LAST BLOCK ON LOCAL SYSTEM: ");
        ui->textBrowser->append(QString::fromStdString(
                                    bchain_->getBlockDBContents()));

        // TODO(matt): Check what the latest transactions are and catch up.
        if (bchain_->updateBChainWithLatestTx()) {
            ui->textBrowser->append("LAST BLOCK ON LOCAL SYSTEM WITH UPDATED TRANSACTIONS: ");
            ui->textBrowser->append(QString::fromStdString(
                                        bchain_->getBlockDBContents()));
        }

        ui->startButton->setEnabled(false);
        ui->addTxButton->setEnabled(true);
        ui->mineButton->setEnabled(true);
    } else {
        ui->textBrowser->append("Blockchain Creation Failed...\n");
    }
}

void Kestrel::on_addTxButton_clicked() {
    if (ui->sndAdrCheckBox->isChecked() && ui->recipAdrCheckBox->isChecked()) {
//            bchain_->executeTransaction("fromLola", "toSydney", 120);
        std::string result = wallet_->getWalletAddrFromPrivateKey(
                    ui->sndPkeyLineEdit->text().toStdString());

        if (result == ui->sndAdrLineEdit->text().toStdString()) {
            bchain_->executeTransaction(ui->sndAdrLineEdit->text().toStdString(),
                                        ui->recipAdrLineEdit->text().toStdString(),
                                        ui->amtToSendSpinBox->text().toInt());

            ui->textBrowser->append("Transaction submitted successfully.");
            ui->textBrowser->append(
                        QString::fromStdString("Current Block Transactions: " +
                                               bchain_->getBlockDataOnly() +
                                               "\n"));
            ui->txStatusInfoLabel->setText("Transaction Submitted");
        } else if (result == "ERROR: Key or address incorrect length after process.") {
            ui->txStatusInfoLabel->setText("Error");
            ui->textBrowser->append("Error: Authentication unsuccessful.");
        } else {
            ui->txStatusInfoLabel->setText("Error");
            ui->textBrowser->append("Error: Incorrect private key.");
        }

    } else {
        ui->txStatusInfoLabel->setText("Error");
        ui->textBrowser->append("Error: Confirm wallet addresses are correct.");
    }
    // Protection against accidentally executing more than one transaction.
    ui->sndAdrCheckBox->setChecked(false);
    ui->recipAdrCheckBox->setChecked(false);
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
    bchain_->executeTransaction("fromLola", "toSydney", 120);

    ui->textBrowser->append(
                    QString::fromStdString(bchain_->getBlockDataOnly() + "\n"));
}

void Kestrel::on_generateKButton_clicked()
{
    std::string private_key = wallet_->generatePrivateKey();
    ui->prvKeyTextBrowser->setText(QString::fromStdString(private_key));

    if (wallet_->computePkeyAndWalletAddress(private_key) == 1) {
        ui->pubKeyTextBrowser->setText(QString::fromStdString(wallet_->
                                                              getPublicKey()));

        ui->textBrowser->append("Key pair generated!");

        ui->adrTextBrowser->setText(QString::fromStdString(wallet_->
                                                           getWalletAddress()));
        ui->textBrowser->append("Wallet address generated!");
    } else {
        ui->textBrowser->append("ERROR: Public Key/Address generation unsuccessful.");
    }
    private_key = "";
}


void Kestrel::on_sndAdrLineEdit_textEdited(const QString &arg1)
{
    ui->typedSndAdrLabel->setText(arg1);
    if (ui->sndAdrLineEdit->text() == "") {
        ui->typedSndAdrLabel->setText("------------------------------------------------------");
    }
}


void Kestrel::on_recipAdrLineEdit_textEdited(const QString &arg1)
{
    ui->typedRecipAdrLabel->setText(arg1);
    if (ui->recipAdrLineEdit->text() == "") {
        ui->typedRecipAdrLabel->setText("------------------------------------------------------");
    }
}

void Kestrel::on_sndAdrCheckBox_clicked(bool checked)
{
    if (checked == 0) {
        ui->sndAdrLineEdit->setEnabled(true);
    } else if (checked == 1) {
        ui->sndAdrLineEdit->setEnabled(false);
    }
}

void Kestrel::on_recipAdrCheckBox_clicked(bool checked)
{
    if (checked == 0) {
        ui->recipAdrLineEdit->setEnabled(true);
    } else if (checked == 1) {
        ui->recipAdrLineEdit->setEnabled(false);
    }
}

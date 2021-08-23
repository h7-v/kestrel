#ifndef KESTREL_H
#define KESTREL_H

// File created by QT mostly for handling of the UI.

#include <QMainWindow>
#include <QCloseEvent>
#include <string>
#include "src/blockchain.h"
#include "src/minerthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Kestrel; }
QT_END_NAMESPACE

class Kestrel : public QMainWindow {
  Q_OBJECT

 public:
  // Handles creation of the UI. May also be used to create a logging file in
  // the future.
  explicit Kestrel(QWidget *parent = nullptr);

  // Deletes the UI.
  ~Kestrel();

  void closeEvent(QCloseEvent *event);

  // Used to keep access to the MinerThread when toggling the Mine button.
  MinerThread *mThread{nullptr};

 private slots:
  void on_startButton_clicked();

  void on_addTxButton_clicked();

  void on_mineButton_toggled(bool checked);

  void on_debugButton_clicked();

 public slots:
  // Appends mined block data to the console text box in the UI when a block
  // has been mined. SIGNAL for this function is in minerthread.h -
  // mineFinished().
  void onMineFinished();

 private:
  // UI pointer created by QT.
  Ui::Kestrel *ui;

  // Pointer is given an address when the blockchain object is created on
  // start. Using this pointer allows for access to the blockchain object in
  // all scopes in the translation unit.
  Blockchain *bChain{nullptr};
};
#endif  // KESTREL_H

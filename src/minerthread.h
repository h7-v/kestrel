#ifndef MINERTHREAD_H
#define MINERTHREAD_H

#include <QThread>
#include <string>
#include "src/blockchain.h"

// QThread subclass used to offload mining to another thread so that the UI
// doesn't lock up.
class MinerThread : public QThread {
  Q_OBJECT

 private:
  // Used to access the Blockchain object directly.
  Blockchain *processChain{nullptr};

 public:
  // Constructor empty.
  explicit MinerThread(QObject *parent = 0);

  // Destructor empty.
  ~MinerThread();

  // Bridges the processChain pointer and the Blockchain object.
  void setProcessChain(Blockchain *b);

  // Override from QThread. Performs the task stated at the top of this class.
  void run() override;

  bool exists = true;  // TODO(matt): Do I even need this?

  // Used by run() to check when to stop the process. This variable is set by
  // the Mine button in the UI when it is toggled off.
  bool stop;

 signals:
  // Signals to the SLOT onMineFinished() in the UI to display the latest block
  // data.
  void mineFinished();
};

#endif  // MINERTHREAD_H

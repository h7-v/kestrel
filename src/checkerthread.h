#ifndef CHECKERTHREAD_H
#define CHECKERTHREAD_H

#include <QThread>
#include <QtCore>
#include <string>
#include <leveldb/db.h>

// QThread subclass used to make sure that the last block in the chain is mined
// before trying to add any new blocks. This may be used for consensus in the
// future as well.
class CheckerThread : public QThread {
  Q_OBJECT

 private:
  // Stores the file path set by BlockDBAccess for opening the database.
  std::string cthread_block_db_file_path_;

 public:
  // Construtor empty.
  explicit CheckerThread(QObject *parent = nullptr);

  // Destructor empty.
  ~CheckerThread();

  // BlockDBAccess sets cthread_block_db_file_path_ before starting run().
  void setFilePath(std::string dbFilePath);

  // Override from QThread. Performs the task stated at the top of this class.
  void run() override;

 signals:
  // Emits a QT SIGNAL from this thread class to the QT SLOT in BlockDBAccess
  // that changes the previousDBBlockIsMined variable in BlockDBAccess from
  // false to true.
  void previousBlockInMinedState();
};
#endif  // CHECKERTHREAD_H

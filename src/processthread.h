
#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QtGui>
#include <QMutex>
#include <QThread>

class PThread : public QThread
{
    Q_OBJECT

public:
    PThread(QObject *parent = 0);
    ~PThread();

    void pigExecute(const QString javaHome, const QString cmdPath, QListWidget *lst, QString timeout);
    void stopProcess();
    QString coprocessStartErrorMsg;

public slots:

protected:
    void run();

private:
    bool m_abort;
    QMutex mutex;
    QString javaHome;
    QString commandBatchFilePath;
    QListWidget *listWidgetStatus;
    QString processExecutionTimeoutSeconds;
};

#endif

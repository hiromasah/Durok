
#include "processthread.h"

//------------------------------------------------------------------------------
PThread::PThread(QObject *parent)
    : QThread(parent)
{
    m_abort = false;
}

//------------------------------------------------------------------------------
PThread::~PThread()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();

    wait();
}

//------------------------------------------------------------------------------
void PThread::pigExecute(const QString jvHome, const QString cmdPath, QListWidget *lst, QString timeout)
{
    QString str = "";
    
    QStringList list = jvHome.split("/", QString::KeepEmptyParts, Qt::CaseSensitive);
    for(int ii = 0; ii < list.count(); ii++){
        if(ii > 0){
            str += "\\";
        }
        str += list.at(ii);
    }
    javaHome = str;
    //javaHome = "c:\\Program Files (x86)\\Java\\jre6";
    commandBatchFilePath = cmdPath;
    listWidgetStatus = lst;
    processExecutionTimeoutSeconds = timeout;
    coprocessStartErrorMsg = "";
    m_abort = false;
    start();
}

//------------------------------------------------------------------------------
void PThread::run()
{
    QProcess coprocess;
    bool stat = false;
    bool contf = true;
    QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
    QTextDecoder *decoder = codec->makeDecoder();

    coprocess.setProcessChannelMode(QProcess::MergedChannels);

    QProcess coprocess1;
    coprocess1.setStandardOutputProcess(&coprocess);
    coprocess1.start("cmd.exe", QStringList() << "/c" << "echo" << "y");
    QStringList env = QProcess::systemEnvironment();
    //env << "JAVA_HOME=c:\\Program Files (x86)\\Java\\jre6\\";
    env << "JAVA_HOME=" + javaHome;
    env.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive), "PATH=\\1;C:\\Bin");
    coprocess.setEnvironment(env);
    coprocess.start(commandBatchFilePath);

    // Clear the status
    //listWidgetStatus->clear();

    if (coprocess.waitForStarted()){
        int timeoutSec = 600;
        if(! processExecutionTimeoutSeconds.isEmpty()){
            bool ok;
            timeoutSec = processExecutionTimeoutSeconds.toInt(&ok, 10);
            if(timeoutSec == 0){
                timeoutSec = 600;
            }
        }

        while(contf){
            QString ret;
            if (!coprocess.waitForReadyRead(timeoutSec * 1000)){
                //QMessageBox::information(this, tr("Confirm"), decoder->toUnicode(coprocess.readAll()));
                stat = true;
                break;
            }
            else{
                //ret = QString(coprocess.readAll());
                ret = decoder->toUnicode(coprocess.readAll());
                if(ret.endsWith("\r\n")){
                    ret = ret.left(ret.length() - 2);
                }
                QStringList list = ret.split("\r\n", QString::KeepEmptyParts, Qt::CaseSensitive);
                for(int ii = 0; ii < list.count(); ii++){
                    listWidgetStatus->addItem(list.at(ii));
                }
                listWidgetStatus->setCurrentItem(listWidgetStatus->item(listWidgetStatus->count() - 1), 0);

                // 2009-10-16 Change judgement of result.
                if(coprocess.state() != QProcess::Running){
                    stat = true;
                    contf = false;
                }
            }
            if (m_abort){
                return;
            }
            msleep(1000);
            if(coprocess.state() != QProcess::Running){
                stat = true;
                contf = false;
            }
        }
    }
    else{
        coprocessStartErrorMsg = decoder->toUnicode(coprocess.errorString().toAscii());
        listWidgetStatus->addItem(coprocessStartErrorMsg);
    }
}

//NOT USED
//------------------------------------------------------------------------------
void PThread::stopProcess()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();
}

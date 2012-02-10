#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "formhelp.h"
#include "processthread.h"
#include "forminput.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString processExecutionTimeoutSeconds;
    QString formalProgramName, programName, programVersion, pigFolder;
    QString javaHome_x86, javaHome, udf;
    QString inputFilesPath;

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    FormHelp *formHelp;
    PThread *pthread;
    FormInput *ff1_input, *ff3_input, *ff4_input, *d_input, *ef_input;

    bool CheckJavaHome(QSettings *ini_settings);
    void getTargetFiles(QListWidget *listWidget, QLineEdit *labelFileStatus, QStringList files);
    bool getTargetFolder(QListWidget *listWidget, QString folder);
    bool getTargetFile(QListWidget *listWidget, QString fpath, int cnt);
    void rewriteFileStatus(QListWidget *listWidget, QLineEdit *labelFileStatus);
    int isDup(QListWidget *listWidget, QString fpath);
    void eraseFilesInFolder(QString folder);
    bool checkFileTailInFolder(QString folder, QString tail);
    bool checkMasterUsed(QString scriptFilePath);
    bool pigExecute(QString cmdPath, QString *result);
    bool pigComplete(QString pigOutputFolder, QString targetPath, QString targetFolder);
    bool makeScriptFile(QString orgScriptPath, QString targetScriptPath);
    QString makeScriptLoad(QListWidget *list, QString key);
    bool checkOverWrite(QString oldPath);
    void iniSet(QString section, QString key, QString value);
    void resetGridLayout();
    void setGridLayout();


    QString outputFilesPath;

private slots:
    void on_action_FF4_triggered(bool checked);
    void on_action_FF3_triggered(bool checked);
    void on_action_D_triggered(bool checked);
    void on_action_EF_triggered(bool checked);
    void on_action_FF1_triggered(bool checked);
    void on_action_UdfDelete_triggered();
    void on_action_UdfAdd_triggered();
    void on_actionHelp_triggered();
    void on_action_About_triggered();
    void on_action_Exit_triggered();
    void on_pushButton_Exec_clicked();
    void on_pushButton_Exit_clicked();
    void on_pushButton_master_clicked();
    void on_pushButton_Ref_clicked();
    void on_pushButton_script_clicked();
    void timerProc();
    void pthreadDone();
};

#endif // MAINWINDOW_H

#ifndef FORMINPUT_H
#define FORMINPUT_H

#include <QtGui>
#include <QWidget>

namespace Ui {
    class FormInput;
}

class FormInput : public QWidget
{
    Q_OBJECT

public:
    explicit FormInput(QString *winputFilesPath, QWidget *parent = 0);
    ~FormInput();

    void setTitle(QString fileKind);
    QString copyFiles(QString targetFolder);
    QListWidget *getListWidget();
    int getListWidgetCount();

private slots:
    void on_pushButton_Add_clicked();
    void on_pushButton_Delete_clicked();

private:
    Ui::FormInput *ui;
    QString fileKind;
    QString *pinputFilesPath;

    void getTargetFiles(QListWidget *listWidget, QLineEdit *labelFileStatus, QStringList files);
    bool getTargetFolder(QListWidget *listWidget, QString folder);
    bool getTargetFile(QListWidget *listWidget, QString fpath, int cnt);
    void rewriteFileStatus(QListWidget *listWidget, QLineEdit *labelFileStatus);
    int isDup(QListWidget *listWidget, QString fpath);
};

#endif // FORMINPUT_H

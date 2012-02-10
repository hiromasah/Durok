#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formhelp.h"
#include "processthread.h"

//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName(programName); //Avoid QSettings trouble in Vista x64
    QCoreApplication::addLibraryPath("plugins");

    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerProc()));

    programVersion = "2.1";
    programName = "Durok";
    formalProgramName = "DPC Data Sampling Program";
    pigFolder = "pig";

    setWindowTitle(tr(formalProgramName.toAscii()) + "(" + programName + ")");

    // Prepare help widget
    formHelp = new FormHelp(0, tr(formalProgramName.toAscii()) + "(" + programName + ")");

    pthread = new PThread();
    connect(pthread, SIGNAL(finished()), this, SLOT(pthreadDone()));

    QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
    QTextDecoder *decoder = codec->makeDecoder();

    QString iniFname = programName + ".ini";
    QSettings *ini_settings;
    ini_settings = new QSettings(iniFname, QSettings::IniFormat);

    // Get JAVA_HOME
    javaHome = decoder->toUnicode(ini_settings->value("JAVA/JAVA_HOME").toByteArray());
    if(javaHome == ""){
        javaHome = "c:/Program Files (x86)/Java/jre6/";
    }

    // Get Process Execution Timeout (default 600sec)
    processExecutionTimeoutSeconds = ini_settings->value("Timeout/ProcessExecution").toString();
    if(processExecutionTimeoutSeconds == ""){
        processExecutionTimeoutSeconds = "600";
    }

    // Get UDF
    udf = "";
    udf = ini_settings->value("UDF/FileName").toString();

    // Check the path to GnuPG
    if(! CheckJavaHome(ini_settings)){
        timer->start(1000);
        return;
    }

    ui->listWidgetStatus->addItem(tr("Processing status."));

    ff1_input = new FormInput(&inputFilesPath);
    ff3_input = 0;
    ff4_input = 0;
    d_input = 0;
    ef_input = new FormInput(&inputFilesPath);

    ef_input->setTitle("EF");
    ff1_input->setTitle("FF1");

    ui->gridLayoutInput->addWidget(ef_input, 0, 0, 1, 3, 0);
    ui->gridLayoutInput->addWidget(ff1_input, 0, 3, 1, 3, 0);
    
    inputFilesPath = "";
}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------------------------------------
void MainWindow::timerProc()
{
    timer->stop();
    close();
}

//------------------------------------------------------------------------------
bool MainWindow::CheckJavaHome(QSettings *ini_settings)
{
    bool stat = true;
    QString javaStdDir, javaStdDir_x86;

    QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
    QTextEncoder *encoder = codec->makeEncoder();

    QDir dir(javaHome);
    if(! dir.exists()){
        javaStdDir_x86 = "c:/Program Files (x86)/Java/jre6/";
        QDir dir_x86(javaStdDir_x86);
        javaStdDir = "c:/Program Files/Java/jre6/";
        QDir dir(javaStdDir);
        if(dir_x86.exists()){
            javaHome = javaStdDir_x86;
        }
        else if(dir.exists()){
            javaHome = javaStdDir;
        }
        else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(this, tr("ERROR"),
                tr("JAVA is not installed in the folder below.") + "\n" +
                " " + javaStdDir + "\n" +
                " " + tr("nor") + "\n" +
                " " + javaStdDir_x86 + "\n\n" +
                tr("Install JAVA(JRE) from the URL below.") + "\n" +
                "http://www.java.com/ja/download/" + "\n\n" +
                tr("If JAVA is installed in another folder, Change ini file.") + "\n" +
                " " + tr("ini file name") + ": " + programName + ".ini\n" +
                " " + tr("section") + ": [JAVA]\n" +
                " " + tr("name") + ": JAVA_HOME\n" +
                " " + tr("ex.") + " JAVA_HOME=C:/Program Files (x86)/Java/jre6/\n"
                ,QMessageBox::Abort);
            javaHome = "";
        }
        if(javaHome != ""){
            ini_settings->setValue("JAVA/JAVA_HOME", QString(encoder->fromUnicode(javaHome)));
        }
        else{
            stat = false;
        }
    }
    return stat;
}

//------------------------------------------------------------------------------
void MainWindow::eraseFilesInFolder(QString folder)
{
    QDir dir(folder);
    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
    for( int ii = 0; ii < files.count(); ii++){
        QFile::remove(folder + "/" + files[ii]);
    }
}

//------------------------------------------------------------------------------
bool MainWindow::checkFileTailInFolder(QString folder, QString tail)
{
    bool stat = false;
    
    QDir dir(folder);
    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
    for( int ii = 0; ii < files.count(); ii++){
        if(files[ii].endsWith(tail)){
            stat = true;
            break;
        }
    }
    return stat;
}

//------------------------------------------------------------------------------
bool MainWindow::checkMasterUsed(QString scriptFilePath)
{
    bool stat = false;
    QString line;
    
    QFile rfile(scriptFilePath);
    if (rfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        while (!rfile.atEnd()) {
            line = rfile.readLine();
            if(line.contains("MASTER =", Qt::CaseInsensitive)){
                stat = true;
                break;
            }
            if(line.contains("MASTER=", Qt::CaseInsensitive)){
                stat = true;
                break;
            }
        }
        rfile.close();
    }
    else{
        // Can't open script file.
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this, tr("ERROR"),
            tr("Can't open the script file.") + "(" + scriptFilePath + ")",
            QMessageBox::Abort);
        stat = false;
    }
    return stat;
}

//------------------------------------------------------------------------------
bool MainWindow::makeScriptFile(QString orgScriptPath, QString targetScriptPath)
{
    bool stat = true;
    QString line;
    
    QFile wfile(targetScriptPath);
    if (wfile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QByteArray line;
        line += "cd ../../tmp\n";
        line += "rmf output\n";
        //if(udf != ""){
        //    line += "REGISTER " + udf + ";\n";
        //}
        
        if(ff1_input){
            QListWidget *list = ff1_input->getListWidget();
            line += makeScriptLoad(list, "FF1");
        }

        if(ff3_input){
            QListWidget *list = ff3_input->getListWidget();
            line += makeScriptLoad(list, "FF3");
        }

        if(ff4_input){
            QListWidget *list = ff4_input->getListWidget();
            line += makeScriptLoad(list, "FF4");
        }

        if(d_input){
            QListWidget *list = d_input->getListWidget();
            line += makeScriptLoad(list, "D");
        }

        if(ef_input){
            QListWidget *list = ef_input->getListWidget();
            line += makeScriptLoad(list, "EF");
        }

        line += "\n\n";
        wfile.write(line);
        
        QFile rfile(orgScriptPath);
        if (rfile.open(QIODevice::ReadOnly | QIODevice::Text)){
            while (!rfile.atEnd()) {
                line = rfile.readLine();
                wfile.write(line);
            }
            rfile.close();
            
            if(udf == ""){
                wfile.write("\nStore FINALDATA into 'output' USING PigStorage();\n");
            }
            else{
                wfile.write("\nStore FINALDATA into 'output' USING jp.ac.u.tokyo.m.dpc.pig.udf.store.StoreDataWithSchema();\n");
            }
        }
        else{
            // Can't open script file.
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(this, tr("ERROR"),
                tr("Can't open the script file.") + "(" + orgScriptPath + ")",
                QMessageBox::Abort);
            if (reply == QMessageBox::Abort){
                wfile.close();
                close();
            }
            stat = false;
        }
        
        wfile.close();
    }
    else{
        // Can't open target script file.
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this, tr("ERROR"),
            tr("Can't generate the target script file.") + "(" + targetScriptPath + ")",
            QMessageBox::Abort);
        if (reply == QMessageBox::Abort){
            close();
        }
        stat = false;
    }
    
    return stat;
}

//------------------------------------------------------------------------------
QString MainWindow::makeScriptLoad(QListWidget *list, QString key)
{
    QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
    QTextEncoder *encoder = codec->makeEncoder();
    QString line = "";
    QString wstr;
    char wbuf[32];
    
    if(list->count() > 0){
        if(list->count() == 1){
            QString fpath = list->item(0)->text();
            QFileInfo *fileinfo = new QFileInfo(fpath);
            line += key + " = load '" + QString(encoder->fromUnicode(fileinfo->fileName())) + "';\n";
        }
        else{
            wstr = "";
            for (int ii = 0; ii < list->count(); ii++){
                QString fpath = list->item(ii)->text();
                QFileInfo *fileinfo = new QFileInfo(fpath);
                ::snprintf(wbuf, 32, "_%d", ii + 1);
                QString itemName = key + QString::fromAscii(wbuf);
                line += itemName + " = load '" + QString(encoder->fromUnicode(fileinfo->fileName())) + "';\n";
                if(wstr != ""){
                    wstr += ",";
                }
                wstr += itemName;
            }
            line += key + " = Union " + wstr + ";\n";
        }
    }
    return line;
}


//------------------------------------------------------------------------------
void MainWindow::on_pushButton_script_clicked()
{
    QFileDialog::Options options;
//        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QStringList files = QFileDialog::getOpenFileNames(
                                this, tr("Select the script file."),
                                inputFilesPath,
                                tr("All Files (*)"),
                                &selectedFilter,
                                options);
    if (files.count() == 1) {
        QFileInfo *fileinfo = new QFileInfo(files[0]);
        if(fileinfo->isFile()){
            ui->lineEdit_script->setText(files[0]);
            inputFilesPath = fileinfo->dir().absolutePath();
        }
    }
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButton_Ref_clicked()
{
    QFileDialog::Options options;
    QString selectedFilter;
    options |= QFileDialog::ShowDirsOnly;
    QString folderName = QFileDialog::getExistingDirectory(this,
                                tr("Select the output folder."),
                                outputFilesPath,
                                options);
    if (!folderName.isEmpty()){
        ui->lineEdit_OutputFolder->setText(folderName);
        outputFilesPath = folderName;
    }
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButton_master_clicked()
{
    QFileDialog::Options options;
//        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QStringList files = QFileDialog::getOpenFileNames(
                                this, tr("Select the master file."),
                                inputFilesPath,
                                tr("All Files (*)"),
                                &selectedFilter,
                                options);
    if (files.count() == 1) {
        QFileInfo *fileinfo = new QFileInfo(files[0]);
        if(fileinfo->isFile()){
            ui->lineEdit_master->setText(files[0]);
            inputFilesPath = fileinfo->dir().absolutePath();
        }
    }
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButton_Exit_clicked()
{
    close();
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButton_Exec_clicked()
{
    bool stat = true;
    bool isMasterUsed = false;
    QString copyErrorPath;
    //QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
    //QTextDecoder *decoder = codec->makeDecoder();
    QString errmsg = "";
    QString workingFolder = "cygwin/tmp";
    QString pigOutputFolder = "cygwin/tmp/output";
    QString scriptPath = "cygwin/tmp/script_generated.txt";
    QString masterPath = "cygwin/tmp/master.txt";
    QString cmdPath = "cygwin/PigExecute.bat";
    QMessageBox::StandardButton reply;

    // Check if the file is selected.
    int fcnt = 0;
    if(ff1_input){
        fcnt += ff1_input->getListWidgetCount();
    }
    if(ff3_input){
        fcnt += ff3_input->getListWidgetCount();
    }
    if(ff4_input){
        fcnt += ff4_input->getListWidgetCount();
    }
    if(d_input){
        fcnt += d_input->getListWidgetCount();
    }
    if(ef_input){
        fcnt += ef_input->getListWidgetCount();
    }
    if(fcnt == 0){
        errmsg += tr("Select the input file(s).") + "\n";
    }

    // Check if the script file is selected.
    if(ui->lineEdit_script->text() == ""){
        errmsg += tr("Select the script file.") + "\n";
    }
    else{
        isMasterUsed = checkMasterUsed(ui->lineEdit_script->text());
    }

    if(isMasterUsed){
        // Check if the master file is selected.
        if(ui->lineEdit_master->text() == ""){
            errmsg += tr("Select the master file.") + "\n";
        }
    }
    else{
        // Check if the master file is selected.
        if(ui->lineEdit_master->text() != ""){
            errmsg += tr("Master file is not used.") + "\n";
        }
    }

    // Check if the output folder is selected.
    if(ui->lineEdit_OutputFolder->text() == ""){
        errmsg += tr("Select the output folder.") + "\n";
    }

    if(errmsg == ""){
        reply = QMessageBox::information(this, tr("Confirm"),
            tr("Are you ready?") + "\n",
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No){
            return;
        }
    }
    else{
        QMessageBox::information(this, tr("Confirm"), errmsg);
        return;
    }

    // Clear the status
    ui->listWidgetStatus->clear();

    // Clear the working folder.
    eraseFilesInFolder(workingFolder);
    QDir dir(pigOutputFolder);
    if(dir.exists()){
        eraseFilesInFolder(pigOutputFolder);
        dir.rmdir(pigOutputFolder);
    }

    setCursor(Qt::WaitCursor);

    if(stat){
        if(ef_input){
            // Copy target EF files into the working folder.
            copyErrorPath = ef_input->copyFiles(workingFolder);
            if(copyErrorPath != ""){
                stat = false;
            }
        }
    }

    if(stat){
        if(d_input){
            // Copy target D files into the working folder.
            copyErrorPath = d_input->copyFiles(workingFolder);
            if(copyErrorPath != ""){
                stat = false;
            }
        }
    }

    if(stat){
        if(ff4_input){
            // Copy target FF4 files into the working folder.
            copyErrorPath = ff4_input->copyFiles(workingFolder);
            if(copyErrorPath != ""){
                stat = false;
            }
        }
    }

    if(stat){
        if(ff3_input){
            // Copy target FF3 files into the working folder.
            copyErrorPath = ff3_input->copyFiles(workingFolder);
            if(copyErrorPath != ""){
                stat = false;
            }
        }
    }

    if(stat){
        if(ff1_input){
            // Copy target FF1 files into the working folder.
            copyErrorPath = ff1_input->copyFiles(workingFolder);
            if(copyErrorPath != ""){
                stat = false;
            }
        }
    }

    if(stat){
        if(isMasterUsed){
            // Copy master file into the working folder.
            QString fpath = ui->lineEdit_master->text();
            QFile *file = new QFile(fpath);

            // Check result of copy function
            if(! file->copy(workingFolder + "/master.txt")){
                copyErrorPath = fpath;
                stat = false;
            }
        }
    }

    if(stat){
        // Make script file into the working folder.
        stat = makeScriptFile(ui->lineEdit_script->text(), scriptPath);

    }
    
    if(stat){
        // Execute Pig.
        ui->listWidgetStatus->addItem(tr("Started processing."));

        pthread->pigExecute(javaHome, cmdPath, ui->listWidgetStatus, processExecutionTimeoutSeconds);
    }
    else{
        QMessageBox::information(this, tr("Confirm"), tr("Can't copy the target file into the working folder.")
            + "(" + copyErrorPath + ")\n\n"
            + tr("Aborted.") + "\n"
            + tr("Check free space of the disk drive C.") + "\n"
            + tr("About twice the free space of the target file is needed."));
    }
}

//NOT USED
//------------------------------------------------------------------------------
bool MainWindow::pigExecute(QString cmdPath, QString *result)
{
    //QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
    //QTextDecoder *decoder = codec->makeDecoder();

    QProcess coprocess;
    bool stat = false;
    bool contf = true;

    coprocess.setProcessChannelMode(QProcess::MergedChannels);

    QProcess coprocess1;
    coprocess1.setStandardOutputProcess(&coprocess);
    coprocess1.start("cmd.exe", QStringList() << "/c" << "echo" << "y");
    coprocess.start(cmdPath);

    if (coprocess.waitForStarted()){
        int timeoutSec = 600;
        if(! processExecutionTimeoutSeconds.isEmpty()){
            bool ok;
            timeoutSec = processExecutionTimeoutSeconds.toInt(&ok, 10);
            if(timeoutSec == 0){
                timeoutSec = 600;
            }
        }
        *result = "";
        while(contf){
            QString ret;
            if (!coprocess.waitForReadyRead(timeoutSec * 1000)){
                //QMessageBox::information(this, tr("Confirm"), decoder->toUnicode(coprocess.readAll()));
                stat = true;
                break;
            }
            else{
                ret = QString(coprocess.readAll());
                if(ret.endsWith("\r\n")){
                    ret = ret.left(ret.length() - 2);
                }
                ui->listWidgetStatus->addItem(ret);
                // 2009-10-16 Change judgement of result.
                if(coprocess.state() != QProcess::Running){
                    stat = true;
                    contf = false;
                }
            }
        }
        if(ui->listWidgetStatus->count()){
            ui->listWidgetStatus->setCurrentItem(ui->listWidgetStatus->item(ui->listWidgetStatus->count() - 1), 0);
        }
    }
    else{
        QString errmsg = coprocess.errorString();
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this, tr("ERROR"),
                                        tr("Can't start PigExecute.bat."),
                                        QMessageBox::Abort);
        if (reply == QMessageBox::Abort){
            close();
        }
    }
    return stat;
}

//------------------------------------------------------------------------------
bool MainWindow::pigComplete(QString pigOutputFolder, QString targetPath, QString targetFolder)
{
    bool stat = true;
    QString resultPath;
    QString headerPath, schemaPath;
    
    QDir dir(pigOutputFolder);
    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Readable);
    stat = checkOverWrite(targetPath);
    if(stat){
        QFile wfile(targetPath);
        if (wfile.open(QIODevice::WriteOnly | QIODevice::Text)){
            schemaPath = pigOutputFolder + "/.schema";
            QFile sfile(schemaPath);
            if(sfile.exists()){
                stat = checkOverWrite(targetFolder + "/.schema");
                if(stat){
                    if(! sfile.copy(targetFolder + "/.schema")){
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::critical(this, tr("ERROR"),
                            tr("Can't copy the schema file.") + "(" + schemaPath + ")",
                            QMessageBox::Abort);
                        if (reply == QMessageBox::Abort){
                        }
                        stat = false;
                    }
                }
            }
            
            if(stat){
                headerPath = pigOutputFolder + "/.header";
                QFile hfile(headerPath);
                if(hfile.exists()){
                    if (hfile.open(QIODevice::ReadOnly | QIODevice::Text)){
                        if (!hfile.atEnd()) {
                            QByteArray line = hfile.readLine();
                            wfile.write(line);
                        }
                        hfile.close();
                        stat = checkOverWrite(targetFolder + "/.header");
                        if(stat){
                            if(! hfile.copy(targetFolder + "/.header")){
                                QMessageBox::StandardButton reply;
                                reply = QMessageBox::critical(this, tr("ERROR"),
                                    tr("Can't copy the header file.") + "(" + headerPath + ")",
                                    QMessageBox::Abort);
                                if (reply == QMessageBox::Abort){
                                }
                                stat = false;
                            }
                        }
                    }
                    else{
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::critical(this, tr("ERROR"),
                            tr("Can't open the header file.") + "(" + headerPath + ")",
                            QMessageBox::Abort);
                        if (reply == QMessageBox::Abort){
                        }
                        stat = false;
                    }
                }
            }
            
            if(stat){
                for( int ii = 0; ii < files.count(); ii++){
                    if(files[ii].mid(0, 5) == "part-"){
                        resultPath = pigOutputFolder + "/" + files[ii];
                        QFile rfile(resultPath);
                        if (rfile.open(QIODevice::ReadOnly | QIODevice::Text)){
                            while (!rfile.atEnd()) {
                                QByteArray line = rfile.readLine();
                                wfile.write(line);
                            }
                            rfile.close();
                        }
                        else{
                            // Can't open pig result file.
                            QMessageBox::StandardButton reply;
                            reply = QMessageBox::critical(this, tr("ERROR"),
                                tr("Can't open the pig result file.") + "(" + resultPath + ")",
                                QMessageBox::Abort);
                            if (reply == QMessageBox::Abort){
                            }
                            stat = false;
                            break;
                        }
                    }
                }
            }
            wfile.close();
        }
        else{
            // Can't open target output file.
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(this, tr("ERROR"),
                tr("Can't open the target output file.") + "(" + targetPath + ")",
                QMessageBox::Abort);
            if (reply == QMessageBox::Abort){
            }
            stat = false;
        }
    }
    
    return stat;
}

//------------------------------------------------------------------------------
void MainWindow::pthreadDone()
{
    bool stat;
    QString targetPath = ui->lineEdit_OutputFolder->text() + "/output.txt";
    QString workingFolder = "cygwin/tmp";
    QString pigOutputFolder = "cygwin/tmp/output";
    
    if(pthread->coprocessStartErrorMsg == ""){
        stat = true;
    }
    else{
        stat = false;
    }

    if(ui->listWidgetStatus->count()){
        ui->listWidgetStatus->setCurrentItem(ui->listWidgetStatus->item(ui->listWidgetStatus->count() - 1), 0);
    }

    if(stat){
        stat = pigComplete(pigOutputFolder, targetPath, ui->lineEdit_OutputFolder->text());
        if(stat){
            if(checkFileTailInFolder(workingFolder, ".log")){
                stat = false;
                
                QMessageBox::information(this, tr("ERROR"), tr("Check detail by the display status or Error logfile in cygwin/tmp/pig_xxxxxxxxxxxxx.log."));
            }
            else{
                QMessageBox::information(this, tr("Confirm"), tr("Completed."));
                
                // Clear the main form
                //ui->lineEdit_OutputFolder->setText("");
                //ui->lineEdit_script->setText("");
                //ui->lineEdit_master->setText("");
                //rewriteFileStatus(ui->listWidget_EF, ui->labelFileStatus_EF);
                //rewriteFileStatus(ui->listWidget_FF1, ui->labelFileStatus_FF1);
            }
        }
    }
    else{
        // returned some error.
        QMessageBox::information(this, tr("ERROR"), tr("Check detail by the display status."));
    }

    // Clear working folder.
    if(stat){
        eraseFilesInFolder(workingFolder);
        QDir odir(pigOutputFolder);
        if(odir.exists()){
            eraseFilesInFolder(pigOutputFolder);
            odir.rmdir(pigOutputFolder);
        }
    }
    setCursor(Qt::ArrowCursor);
}

void MainWindow::on_action_Exit_triggered()
{
    close();
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, tr(formalProgramName.toAscii()) + "(" + programName + ") " + " Ver." + programVersion, tr(formalProgramName.toAscii()) + tr(" is a helper program for DPC data sampling using Pig."));
}

void MainWindow::on_actionHelp_triggered()
{
    formHelp->show();
    formHelp->activateWindow();
}


void MainWindow::on_action_UdfAdd_triggered()
{
    QString pigLibFolder = "cygwin/" + pigFolder + "/lib";
    QFileDialog::Options options;
//        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString wFilesPath;
    bool stat;
    
    if(udf == ""){
        QStringList files = QFileDialog::getOpenFileNames(
                                    this, tr("Select the UDF file."),
                                    wFilesPath,
                                    tr("UDF Files (*.jar)"),
                                    &selectedFilter,
                                    options);
        if (files.count() == 1) {
            QFileInfo *fileinfo = new QFileInfo(files[0]);
            if(fileinfo->isFile()){
                QString fname = fileinfo->fileName();
                QString oldPath = pigLibFolder + "/" + fname;
                
                stat = checkOverWrite(oldPath);
                
                if(stat){
                    QFile *newFile = new QFile(files[0]);
                    if(newFile->copy(oldPath)){
                        udf = fname;
                        iniSet("UDF", "FileName", udf);

                        QMessageBox::information(this, tr("Confirm"), tr("Completed."));
                    }
                    else{
                        QMessageBox::information(this, tr("ERROR"), tr("Can't copy the file.")  + "(" + oldPath + ")");
                    }
                }
            }
        }
    }
    else{
        QMessageBox::information(this, tr("Confirm"), tr("The UDF is already entried. Delete it first."));
    }
}

//------------------------------------------------------------------------------
bool MainWindow::checkOverWrite(QString oldPath)
{
    bool stat;
    
    QFile oldFile(oldPath);
    stat = true;
    if(oldFile.exists()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, tr("Confirm"),
                tr("The file is already exists. overwrite?")  + "(" + oldPath + ")",
                QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes){
            if(! oldFile.remove()){
                QMessageBox::information(this, tr("ERROR"), tr("Can't delete the file.")  + "(" + oldPath + ")");
                stat = false;
            }
        }
        else if(reply == QMessageBox::No){
            stat = false;
        }
    }
    
    return stat;
}

//------------------------------------------------------------------------------
void MainWindow::iniSet(QString section, QString key, QString value)
{
    QString iniFname = programName + ".ini";
    QSettings *ini_settings;
    ini_settings = new QSettings(iniFname, QSettings::IniFormat);

    ini_settings->setValue(section + "/" + key, value);
}

void MainWindow::on_action_UdfDelete_triggered()
{
    QString pigLibFolder = "cygwin/" + pigFolder + "/lib";
    QFileDialog::Options options;
//        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString wFilesPath;
    bool stat;
    
    if(udf == ""){
        QMessageBox::information(this, tr("Confirm"), tr("The UDF is not entried."));
    }
    else{
        QString oldPath = pigLibFolder + "/" + udf;
        QFile oldFile(oldPath);
        stat = true;
        if(oldFile.exists()){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::information(this, tr("Confirm"),
                    tr("Are you sure to delete the UDF?")  + "(" + udf + ")",
                    QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes){
                if(! oldFile.remove()){
                    QMessageBox::information(this, tr("ERROR"), tr("Can't delete the file.")  + "(" + oldPath + ")");
                    stat = false;
                }
            }
            else if(reply == QMessageBox::No){
                stat = false;
            }
        }
        
        if(stat){
            udf = "";
            iniSet("UDF", "FileName", udf);
            QMessageBox::information(this, tr("Confirm"), tr("Completed."));
        }
    }
}

void MainWindow::resetGridLayout()
{
    if(ef_input){
        ui->gridLayoutInput->removeWidget(ef_input);
    }
    if(d_input){
        ui->gridLayoutInput->removeWidget(d_input);
    }
    if(ff4_input){
        ui->gridLayoutInput->removeWidget(ff4_input);
    }
    if(ff3_input){
        ui->gridLayoutInput->removeWidget(ff3_input);
    }
    if(ff1_input){
        ui->gridLayoutInput->removeWidget(ff1_input);
    }
}

void MainWindow::setGridLayout()
{
    int cnt = 0;
    int colspan;

    if(ef_input){
        if((cnt % 2 == 0) && (d_input == 0) && (ff4_input == 0) && (ff3_input == 0) && (ff1_input == 0)){
            colspan = 6;
        }
        else{
            colspan = 3;
        }
        ui->gridLayoutInput->addWidget(ef_input, (int)(cnt / 2), (cnt % 2) * 3, 1, colspan, 0);
        cnt++;
    }
    if(d_input){
        if((cnt % 2 == 0) && (ff4_input == 0) && (ff3_input == 0) && (ff1_input == 0)){
            colspan = 6;
        }
        else{
            colspan = 3;
        }
        ui->gridLayoutInput->addWidget(d_input, (int)(cnt / 2), (cnt % 2) * 3, 1, colspan, 0);
        cnt++;
    }
    if(ff4_input){
        if((cnt % 2 == 0) && (ff3_input == 0) && (ff1_input == 0)){
            colspan = 6;
        }
        else{
            colspan = 3;
        }
        ui->gridLayoutInput->addWidget(ff4_input, (int)(cnt / 2), (cnt % 2) * 3, 1, colspan, 0);
        cnt++;
    }
    if(ff3_input){
        if((cnt % 2 == 0) && (ff1_input == 0)){
            colspan = 6;
        }
        else{
            colspan = 3;
        }
        ui->gridLayoutInput->addWidget(ff3_input, (int)(cnt / 2), (cnt % 2) * 3, 1, colspan, 0);
        cnt++;
     }
    if(ff1_input){
        if(cnt % 2 == 0){
            colspan = 6;
        }
        else{
            colspan = 3;
        }
        ui->gridLayoutInput->addWidget(ff1_input, (int)(cnt / 2), (cnt % 2) * 3, 1, colspan, 0);
        cnt++;
    }
}

void MainWindow::on_action_FF1_triggered(bool checked)
{
    resetGridLayout();
    if(checked){
        ff1_input = new FormInput(&inputFilesPath);
        ff1_input->setTitle("FF1");
    }
    else{
        ff1_input->close();
        ff1_input = 0;
    }
    setGridLayout();
}

void MainWindow::on_action_EF_triggered(bool checked)
{
    resetGridLayout();
    if(checked){
        ef_input = new FormInput(&inputFilesPath);
        ef_input->setTitle("EF");
    }
    else{
        ef_input->close();
        ef_input = 0;
    }
    setGridLayout();
}

void MainWindow::on_action_D_triggered(bool checked)
{
    resetGridLayout();
    if(checked){
        d_input = new FormInput(&inputFilesPath);
        d_input->setTitle("D");
    }
    else{
        d_input->close();
        d_input = 0;
    }
    setGridLayout();
}

void MainWindow::on_action_FF3_triggered(bool checked)
{
    resetGridLayout();
    if(checked){
        ff3_input = new FormInput(&inputFilesPath);
        ff3_input->setTitle("FF3");
    }
    else{
        ff3_input->close();
        ff3_input = 0;
    }
    setGridLayout();
}

void MainWindow::on_action_FF4_triggered(bool checked)
{
    resetGridLayout();
    if(checked){
        ff4_input = new FormInput(&inputFilesPath);
        ff4_input->setTitle("FF4");
    }
    else{
        ff4_input->close();
        ff4_input = 0;
    }
    setGridLayout();
}

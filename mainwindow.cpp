#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "chatgptconfig.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QJsonArray>
#include <QListWidget>
#include <QThread>
#include <qmarkdowntextedit.h>

#include "updatetextworker.h"

ProgressDialog::ProgressDialog(const QUrl &url, QWidget *parent)
  : QProgressDialog(parent)
{
    setWindowTitle(tr("ChatGpt Progress"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setLabelText(tr("ChatGpt %1.").arg(url.toDisplayString()));
    setMinimum(0);
    setValue(0);
    setMinimumDuration(0);
    setMinimumSize(QSize(400, 75));
}

void ProgressDialog::networkReplyProgress(qint64 bytesRead, qint64 totalBytes)
{
    setMaximum(totalBytes);
    setValue(bytesRead);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),_reply(nullptr),_worker(new UpdateTextWorker())
{
    ui->setupUi(this);
    InitLayout();
    InitSigSlot();
}

MainWindow::~MainWindow()
{
    ChatGptConfig::Instance()->SaveConfig();
    _worker->Stop();
    while(!_worker->isFinished()){
        QThread::msleep(1);
    }
    delete ui;
}

void MainWindow::StartRequest(const QString &question)
{
    QNetworkRequest request(QUrl::fromUserInput("https://api.openai.com/v1/chat/completions"));

    if(ChatGptConfig::Instance()->_apiKey.isEmpty()){
        qDebug() << "api key is null";
        return;
    }
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer " + ChatGptConfig::Instance()->_apiKey).toUtf8());

    QJsonObject prompt;
    prompt.insert("role", "user");
    prompt.insert("content", question);
    QJsonArray array;
    array.append(prompt);
    QJsonObject parameters;
    parameters.insert("model", "gpt-3.5-turbo");
    parameters.insert("messages", array);

    qDebug() << "==>" << question;

    QJsonDocument requestBody(parameters);
    _reply = _qnam.post(request, requestBody.toJson());
    QObject::connect(_reply, &QNetworkReply::finished, [&]{
        if (_reply->error() == QNetworkReply::NoError) {
//            QString retmsg = "";
            QByteArray responseData = _reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
//            qDebug() << "all:" << responseData;
            QJsonObject responseObject = responseDoc.object();
            QJsonValue choicesValue = responseObject.value("choices");
            if (choicesValue.isArray() && choicesValue.toArray().size() > 0) {
                QJsonValue textValue = choicesValue.toArray().at(0).toObject().value("message").toObject().value("content");
                if (textValue.isString()) {
                    _items.append(new QListWidgetItem());
                    _items.at(_items.length()-1)->setIcon(QIcon(":/res/openai_logo.svg"));
                    _ctxList->addItem(_items.at(_items.length()-1));
                    emit appendEditTextSignal(textValue.toString());
                }
            }
        } else {
            qDebug() << "Error:" << _reply->errorString();
        }
        _reply->deleteLater();
        _btnQuery->setEnabled(true);
    });

    ProgressDialog *progressDialog = new ProgressDialog(QUrl::fromUserInput("https://api.openai.com/v1/chat/completions"), this);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(progressDialog, &QProgressDialog::canceled, [&]{
        _reply->abort();
        _btnQuery->setEnabled(true);
        qDebug() << "udpate text ok";
    });
    connect(_reply, &QNetworkReply::downloadProgress, progressDialog, &ProgressDialog::networkReplyProgress);
    connect(_reply, &QNetworkReply::finished, progressDialog, &ProgressDialog::hide);
    progressDialog->show();
}

void MainWindow::updateEditTextSlot(const QString& tx)
{
    QMarkdownTextEdit* editReply = new QMarkdownTextEdit();
    _ctxList->setItemWidget(_items.at(_items.length()-1), editReply);
    editReply->setText(tx);
    _items.at(_items.length()-1)->setSizeHint(QSize(editReply->width(), 20*editReply->blockCount()));
    _ctxList->scrollToBottom();
    editReply->update();
}

void MainWindow::httpQuery(){
    _btnQuery->setEnabled(false);
    QListWidgetItem *item = new QListWidgetItem();
    item->setIcon(QIcon(":/res/user.svg"));
    item->setText(_editQuery->text());
    _ctxList->addItem(item);
    StartRequest(_editQuery->text());
    _editQuery->clear();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        httpQuery();
    }
}

void MainWindow::InitSigSlot()
{
    QObject::connect(ui->actionSetApiKey, &QAction::triggered, [=]{
        _dialog.show();
    });
    QObject::connect(ui->actionQuit, &QAction::triggered, [=]{
        QCoreApplication::quit();
    });
    QObject::connect(ui->actionAbout, &QAction::triggered, [=]{
        qDebug() << "about qchatgpt";
    });

    QObject::connect(_worker, SIGNAL(updateEditTextSignal(QString)), this, SLOT(updateEditTextSlot(QString)));
    QObject::connect(this, SIGNAL(appendEditTextSignal(QString)), _worker, SLOT(appendEditTextSlot(QString)));

    _worker->start();
}


void MainWindow::InitLayout()
{
    QVBoxLayout *mLayout = new QVBoxLayout();

    QHBoxLayout *questionLayout = new QHBoxLayout();
    _editQuery = new QLineEdit();
    _btnQuery = new QPushButton();
    _btnQuery->setText(tr("Send"));
    questionLayout->addWidget(_editQuery);
    questionLayout->addWidget(_btnQuery);

    QObject::connect(_btnQuery, &QPushButton::clicked, [=]{ 
        httpQuery();
    });

    _ctxList = new QListWidget();
    mLayout->addWidget(_ctxList);
    _ctxList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    mLayout->addLayout(questionLayout);

    ui->centralwidget->setLayout(mLayout);
}

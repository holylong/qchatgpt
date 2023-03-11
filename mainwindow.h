#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressDialog>

#include <QVector>
#include <QString>
#include <QMap>
//#include <tuple>

#include "settingdialog.h"

class QMarkdownTextEdit;
class QListWidget;
class QLineEdit;
class QListWidgetItem;
class UpdateTextWorker;

class ProgressDialog : public QProgressDialog {
    Q_OBJECT

public:
    explicit ProgressDialog(const QUrl &url, QWidget *parent = nullptr);

public slots:
   void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitLayout();
    void InitSigSlot();
    void StartRequest(const QString &question);

protected:
    void keyPressEvent(QKeyEvent *event);
    void httpQuery();

private slots:
    void updateEditTextSlot(const QString& tx);

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager _qnam;
    QListWidget             *_ctxList;
    QNetworkReply         *_reply;
    QVector<QMarkdownTextEdit*> _editReplys;
    QVector<QListWidgetItem*> _items;
    QPushButton           *_btnQuery;
    QLineEdit             *_editQuery;
    UpdateTextWorker      *_worker;

    SettingDialog  _dialog;

    QVector<QMap<QString, QString >> _arrRequest;
//    QVector<std::tuple<const QString&, const QString &>> _arrRequest;

signals:
    void appendEditTextSignal(const QString& tx);
};
#endif // MAINWINDOW_H

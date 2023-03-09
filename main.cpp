#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QStringListModel>
#include <QListView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QStringList>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>
#include <QUrl>
#include <QApplication>

#include "httprequest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/res/openai_logo.svg"));
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "chatgpt_" + QLocale(locale).name();
        if (translator.load(":/res/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

#if 1
    MainWindow w;
    w.show();

#else
    // Create the main window
    QWidget window;
    window.setWindowTitle("Chat Window");

    // Create the message list view
    QListView messageListView;
    QStringListModel messageListModel;
    QStringList slis{"John: Hello", "Jane: Hi there"};
    messageListModel.setStringList(slis);
    messageListView.setModel(&messageListModel);

    // Create the message input box and send button
    QTextEdit messageInputBox;
    QPushButton sendButton("Send");
    QObject::connect(&sendButton, &QPushButton::clicked, [&]{
        int row = messageListView.currentIndex().row();
        messageListModel.insertRow(row);
        messageListModel.setData(messageListModel.index(row),
                                 messageInputBox.toPlainText());
        messageInputBox.clear();
    });

    // Create the layout
    QVBoxLayout mainLayout;
    mainLayout.addWidget(&messageListView);

    QHBoxLayout inputLayout;
    inputLayout.addWidget(&messageInputBox);
    inputLayout.addWidget(&sendButton);
    mainLayout.addLayout(&inputLayout);

    // Set the layout to the main window
    window.setLayout(&mainLayout);

    // Show the window
    window.show();
#endif
    return a.exec();
}

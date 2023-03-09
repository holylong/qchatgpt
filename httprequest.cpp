#include "httprequest.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>
#include <QUrl>
#include <QApplication>

namespace chatgpt{
void RequestChatGPT(const QString &url, const QString &key, const QString &question){
    QNetworkAccessManager manager;

    QNetworkRequest request(QUrl::fromUserInput(url));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer " + key).toUtf8());

    QJsonObject prompt;
    prompt.insert("role", "user");
    prompt.insert("content", question);
    QJsonArray array;
    array.append(prompt);
    QJsonObject parameters;
    parameters.insert("model", "gpt-3.5-turbo");
    parameters.insert("messages", array);

    QJsonDocument requestBody(parameters);
    QNetworkReply *reply = manager.post(request, requestBody.toJson());
    QObject::connect(reply, &QNetworkReply::finished, [&]{
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            qDebug() << "all:" << responseData;
            QJsonObject responseObject = responseDoc.object();
            QJsonValue choicesValue = responseObject.value("choices");
            if (choicesValue.isArray() && choicesValue.toArray().size() > 0) {
                QJsonValue textValue = choicesValue.toArray().at(0).toObject().value("message").toObject().value("content");
                if (textValue.isString()) {
                    qDebug() << textValue.toString();
                }
            }
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
        QCoreApplication::quit();
    });
}
}

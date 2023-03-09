#ifndef CHATGPTCONFIG_H
#define CHATGPTCONFIG_H

#include <QObject>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include "fileutil.h"

class ChatGptConfig : public QObject
{
    Q_OBJECT
public:
    explicit ChatGptConfig(QObject *parent = nullptr);

    static ChatGptConfig* Instance(){
        static ChatGptConfig config;
        return &config;
    }

    void LoadConfig(){
        QString path = QDir::currentPath();
        path += "/config.json";
        if(FileUtil::Exist(path)){
            qDebug() << "file exist";
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen()){
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                QJsonObject jobj = doc.object();
                if(jobj.contains("apikey")){
                    _apiKey = jobj["apikey"].toString();
                }
                file.close();
            }else{
                qDebug() << "file open failed!";
            }
        }
    }

    void SaveConfig(){
        QJsonObject jobj;
        jobj["apikey"] = _apiKey;
        QJsonDocument doc(jobj);

        QFile file("config.json");
        file.open(QIODevice::WriteOnly);
        if(file.isOpen()){
            file.write(doc.toJson());
            file.close();
        }
    }

public:
    QString _apiKey{""};

signals:

};

#endif // CHATGPTCONFIG_H

#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QObject>
#include <QFile>

class FileUtil : public QObject
{
    Q_OBJECT
public:
    explicit FileUtil(QObject *parent = nullptr);

    static bool Exist(const QString& filepath){
        return QFile(filepath).exists();
    }

    static QString ReadFile(const QString &filepath){
        if(Exist(filepath)){
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen()){
                return file.readAll();
            }
        }
    }

signals:

};

#endif // FILEUTIL_H

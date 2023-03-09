#ifndef UPDATETEXTWORKER_H
#define UPDATETEXTWORKER_H

#include <QThread>
#include <QVector>

class UpdateTextWorker : public QThread
{
    Q_OBJECT
public:
    explicit UpdateTextWorker(QObject *parent = nullptr);

    void Stop(){
        _bStop = true;
    }

protected:
    void run() override;


private slots:
    void appendEditTextSlot(const QString&);

signals:
    void updateEditTextSignal(const QString&);

private:
    QVector<QString> _arrAssistent;
    bool             _bStop{false};
};

#endif // UPDATETEXTWORKER_H

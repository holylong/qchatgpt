#include "updatetextworker.h"

UpdateTextWorker::UpdateTextWorker(QObject *parent) : QThread(parent)
{

}


void UpdateTextWorker::run(){
    while(!_bStop){
        if(!_arrAssistent.empty()){
            emit updateEditTextSignal(_arrAssistent.at(0));
            _arrAssistent.pop_front();
        }
    }
}

void UpdateTextWorker::appendEditTextSlot(const QString& ass)
{
    _arrAssistent.append(ass);
}

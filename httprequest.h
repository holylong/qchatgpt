#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QString>


namespace chatgpt{
    void RequestChatGPT(const QString &url, const QString &key, const QString &question);
}

#endif // HTTPREQUEST_H

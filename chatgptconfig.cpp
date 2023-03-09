#include "chatgptconfig.h"

ChatGptConfig::ChatGptConfig(QObject *parent) : QObject(parent)
{
    LoadConfig();
}

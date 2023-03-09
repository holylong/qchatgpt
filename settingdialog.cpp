#include "settingdialog.h"
#include "chatgptconfig.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>


SettingDialog::SettingDialog(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QFormLayout *fLayout = new QFormLayout();
    QLineEdit *edit = new QLineEdit();
    fLayout->addRow("ApiKey:", edit);
    edit->setText(ChatGptConfig::Instance()->_apiKey);
    QPushButton *btnSave =new QPushButton(tr("Save"));
    mainLayout->addLayout(fLayout);
    mainLayout->addWidget(btnSave, Qt::AlignRight);

    QObject::connect(btnSave, &QPushButton::clicked, [=]{
        ChatGptConfig::Instance()->_apiKey = edit->text();
        // qDebug() << ChatGptConfig::Instance()->_apiKey;
        hide();
    });
    setLayout(mainLayout);
}

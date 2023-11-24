/**********************************************************
 * Author(作者)     : Qt君
 * 微信公众号        : Qt君
 * Website(网站)    : qthub.com
 * QQ交流群         : 1039852727
 * Email(邮箱)      : 2088201923@qq.com
 *********************************************************/
#include <QApplication>
#include <QDateTime>
#include <QCryptographicHash>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include "HttpClient.h"

using namespace AeaQt;

static QByteArray createSign(const QByteArray &text, const QByteArray &appid, const QByteArray &key, const QByteArray &salt)
{
    return QCryptographicHash::hash(appid+text+salt+key, QCryptographicHash::Md5).toHex();
}

static QString trans(const QByteArray &text, bool isEn = true)
{
    QString output;
    QByteArray appid = APP_ID; // 修改为你的app-id
    QByteArray key   = APP_KEY; // 修改为你的密钥
    QByteArray salt  = QByteArray::number(QDateTime::currentSecsSinceEpoch());
    QByteArray sign  = createSign(text, appid, key, salt);

    static HttpClient client;
    client.get("http://api.fanyi.baidu.com/api/trans/vip/translate")
          .queryParam("from",  isEn ? "en" : "zh")
          .queryParam("to",    isEn ? "zh" : "en")
          .queryParam("q",     text)
          .queryParam("appid", appid)
          .queryParam("salt",  salt)
          .queryParam("sign",  sign)
          .onSuccess([&](QJsonObject result) {
              qDebug().noquote()<<result;
              auto arr = result.value("trans_result").toArray();
              output = text;
              int index = 0;
              // 转换翻译,可将换行和空格字符一并转换
              for (int i = 0; i < arr.size(); i++) {
                  QString src = arr.at(i).toObject().value("src").toString();
                  QString dst = arr.at(i).toObject().value("dst").toString();
                  index = output.indexOf(src, index);
                  output.replace(index, src.size(), dst);
              }
           })
          .onFailed([](QByteArray error) { qDebug()<<error; })
          .logLevel(HttpRequest::All)
          .block()
          .exec();

    return output;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPushButton *okBtn = new QPushButton("翻译");
    QPushButton *switchBtn = new QPushButton("英->中");
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(okBtn, 5);
    hLayout->addWidget(switchBtn, 1);

    QTextEdit *textInput = new QTextEdit;
    QTextEdit *textOutput = new QTextEdit;
    textOutput->setReadOnly(true);
    QWidget w;
    w.setWindowTitle("翻译小工具");
    w.resize(320, 240);
    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(textInput, 2);
    l->addLayout(hLayout, 1);
    l->addWidget(textOutput, 2);
    w.setLayout(l);
    w.show();

    QObject::connect(okBtn, &QPushButton::clicked, okBtn, [&](){
        qDebug().noquote() << textInput->toPlainText().toUtf8();
        QString result = trans(textInput->toPlainText().toUtf8(), switchBtn->text() == "英->中" ? true : false);
        textOutput->setPlainText(result);
    });

    QObject::connect(switchBtn, &QPushButton::clicked, switchBtn, [&](){
        if (switchBtn->text() == "中->英") {
            switchBtn->setText("英->中");
        }
        else {
            switchBtn->setText("中->英");
        }
    });

    return a.exec();
}

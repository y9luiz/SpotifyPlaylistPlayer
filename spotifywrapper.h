#ifndef SPOTIFYWRAPPER_H
#define SPOTIFYWRAPPER_H

#include <QObject>
#include <QtNetwork>
#include <QJsonObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QString>
#include <memory>
#include <QtNetworkAuth>

class SpotifyWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SpotifyWrapper(QObject *parent = nullptr);
    QNetworkReply  * me();
signals:
    void authenticated();
public slots:
    void grant();
    void granted();
private:

    std::shared_ptr<QOAuth2AuthorizationCodeFlow> oauth2;
    QOAuthHttpServerReplyHandler *replyHandler = nullptr;

    QString state;
    QString code;

    // Constants
    static constexpr uint16_t port = 8080;
    QString authorizationBaseUrlString = "https://accounts.spotify.com/authorize";
    const QString responseType = "code";
    const QString clientId = "20087250e7fb4414a94189aea4f714a4";
    const QString clientSecret = "207ce7f1040b44d39ae7d45494a8655d";
    const QString scope = "user-top-read";//"user-read-private user-read-email";
    const QString redirectUri = "http://localhost:" + QString::number(port) + "/cb/";
};

#endif // SPOTIFYWRAPPER_H

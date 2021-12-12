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

signals:
    void authenticated();
public slots:
    void grant();
private:

    std::shared_ptr<QOAuth2AuthorizationCodeFlow> oauth2;
    QOAuthHttpServerReplyHandler *replyHandler = nullptr;


    // Constants
    static constexpr uint16_t port = 8080;
    QString authorizationBaseUrlString = "https://accounts.spotify.com/authorize?";
    const QString responseType = "code";
    const QString clientId = "20087250e7fb4414a94189aea4f714a4";
    const QString scope = "user-read-private user-read-email";
    const QString redirectUri = "https://localhost:" + QString::number(port) + "/callback";
};

#endif // SPOTIFYWRAPPER_H

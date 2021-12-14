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
    QString inline getUserId(){return userId;};
    QString searchTrack(QString  phrase,  uint8_t limit);

signals:
    void authenticated();
    void userIdReplied();
public slots:
    void grant();
    void granted();
private:

    void requestUserId();
    std::shared_ptr<QOAuth2AuthorizationCodeFlow> oauth2;
    QOAuthHttpServerReplyHandler *replyHandler = nullptr;

    QString userId;

    QString state;
    QString code;

};

#endif // SPOTIFYWRAPPER_H

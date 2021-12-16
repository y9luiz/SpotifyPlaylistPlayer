#ifndef SPOTIFYWRAPPER_H
#define SPOTIFYWRAPPER_H

#include <QObject>
#include <QtNetwork>
#include <QJsonObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QString>
#include <memory>
#include <QtNetworkAuth>
#include <DataAccessObjects/dataaccessobject.h>

class SpotifyWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SpotifyWrapper(QObject *parent = nullptr);
    void requestUser();
    void requestSearchTrack(QString  phrase, uint8_t limit);
signals:
    void authenticated();
    void userReplied(SpotifyUser user);
    void trackReplied(QList<SpotifyTrack> tracks);
public slots:
    void grant();
    void granted();

private:
    QNetworkReply  * me();
    std::shared_ptr<QOAuth2AuthorizationCodeFlow> oauth2;
    QOAuthHttpServerReplyHandler *replyHandler = nullptr;
    QString state;
    QString code;

};

#endif // SPOTIFYWRAPPER_H

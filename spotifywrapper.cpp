#include "spotifywrapper.h"
#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <unordered_map>
#include <string>
QString JsonObjectToQString(QJsonObject & jsonObject)
{
    return QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
}

SpotifyWrapper::SpotifyWrapper(QObject *parent)
    : QObject{parent}
{
    QUrl authorizationUrl(authorizationBaseUrlString);
    QUrl acessTokenUrl("https://accounts.spotify.com/api/token");
    oauth2 = std::make_shared<QOAuth2AuthorizationCodeFlow>(clientId,authorizationUrl,acessTokenUrl,nullptr, parent);
    replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    replyHandler->setCallbackPath("callback");
    oauth2->setReplyHandler(replyHandler);
    oauth2->setScope(scope);

    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::statusChanged, [=](
            QAbstractOAuth::Status status) {
        if (status == QAbstractOAuth::Status::Granted)
        {
            qDebug() << "Granted";
            emit authenticated();
        }
        else{
            qDebug() << "Not granted";
        }
    });
    oauth2->setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QVariantMap *parameters) {
        if (stage == QAbstractOAuth::Stage::RequestingAuthorization)// && isPermanent())
        {
            parameters->insert("duration", "permanent");
            parameters->insert("client_id",clientId);
            parameters->insert("redirect_uri",redirectUri);
            parameters->insert("state","xxxxxxxxxxxxxxx");
        }
    });
    // open the web desktop browser to perform the authentication
    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

}
void SpotifyWrapper::grant()
{
    oauth2->grant();

}


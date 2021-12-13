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
    oauth2 = std::make_shared<QOAuth2AuthorizationCodeFlow>();
    oauth2->setAuthorizationUrl(authorizationUrl);
    oauth2->setAccessTokenUrl(acessTokenUrl);
    oauth2->setClientIdentifier(clientId);
    oauth2->setClientIdentifierSharedKey(clientSecret);
    replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    oauth2->setReplyHandler(replyHandler);
    oauth2->setScope(scope);

    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::statusChanged, [=](
            QAbstractOAuth::Status status) {

        if (status == QAbstractOAuth::Status::Granted)
        {
            qDebug() << "Granted";
            emit authenticated();
        }
        else if (status == QAbstractOAuth::Status::NotAuthenticated){
            qDebug() << "Not NotAuthenticated";
        }

        else if(status == QAbstractOAuth::Status::TemporaryCredentialsReceived)
        {
            qDebug() << "TemporaryCredentialsReceived";
            qDebug()<<oauth2->token();

        }
        else if(status == QAbstractOAuth::Status::RefreshingToken)
        {
            qDebug() << "RefreshingToken";

        }
    });

    oauth2->setModifyParametersFunction([&](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
            if(stage == QAbstractOAuth::Stage::RequestingAuthorization) {
                qDebug() << * parameters;

                parameters->insert("duration", "permanent");
                parameters->insert("redirect_uri",redirectUri);
                parameters->insert("response_type",responseType);
                parameters->insert("client_id" ,oauth2->clientIdentifier());
                parameters->insert("client_secret", oauth2->clientIdentifierSharedKey());
                parameters->insert("scope",scope);
            }
            if(stage == QAbstractOAuth::Stage::RequestingAccessToken)
            {
                parameters->insert("redirect_uri",redirectUri);

                qDebug() << * parameters;
                parameters->insert("state",state);
                parameters->insert("grant_type","authorization_code");
                parameters->insert("code",code);
            }
        });

    // open the web desktop browser to perform the authentication
    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);
    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::granted,
                this, &SpotifyWrapper::granted);
    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::error,
                this, [&](const QString &error, const QString &errorDescription, const QUrl &uri)
    {
        qDebug()<<"erro porra\n";
    });
    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::authorizationCallbackReceived,
                this, [this](const QVariantMap &data)
    {
           code = data["code"].toString();
           state = data["state"].toString();
           qDebug() <<"code " << code;
           qDebug() <<"state "<< oauth2->state();
           /*emit oauth2->stateChanged(state);
           auto url = QUrl("https://accounts.spotify.com/api/token");
           auto reply = oauth2->post(url,data);
           connect(reply, &QNetworkReply::finished, [=]() {
                   if (reply->error() != QNetworkReply::NoError) {
                        qWarning () << "Erro Encontrado";
                        qWarning() << reply->error();
                       return;//https://accounts.spotify.com/api/token
                   }

                   const auto data = reply->readAll();

                   const auto document = QJsonDocument::fromJson(data);
                   const auto root = document.object();

                   qDebug()<<data;
                   reply->deleteLater();
               });*/


    });

}
void SpotifyWrapper::grant()
{
    oauth2->grant();
}
void SpotifyWrapper::granted()
{
    qDebug()<<"entrou\n";
}
QNetworkReply  * SpotifyWrapper::me()
{
    return oauth2->get(QUrl("https://api.spotify.com/v1/me"));
}



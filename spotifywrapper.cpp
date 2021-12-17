#include "spotifywrapper.h"
#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <unordered_map>
#include <string>
#include "utils.h"
QString JsonObjectToQString(QJsonObject & jsonObject)
{
    return QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
}

SpotifyWrapper::SpotifyWrapper(QObject *parent)
    : QObject{parent}
{
    // initialize the QoAuth2Authorization Object
    oauth2 = std::make_shared<QOAuth2AuthorizationCodeFlow>();
    oauth2->setAuthorizationUrl(QUrl(Constants::SpotifyWrapper::authorizationUrlString));
    oauth2->setAccessTokenUrl(QUrl(Constants::SpotifyWrapper::acessTokenUrlString));
    oauth2->setClientIdentifier(Constants::SpotifyWrapper::clientId);
    oauth2->setClientIdentifierSharedKey(Constants::SpotifyWrapper::clientSecret);
    oauth2->setScope(Constants::SpotifyWrapper::scope);

    // Create an http to deal with the replies
    replyHandler = new QOAuthHttpServerReplyHandler(Constants::SpotifyWrapper::port, this);
    oauth2->setReplyHandler(replyHandler);

    // deal with authorization status changes
    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::statusChanged,
            [=](QAbstractOAuth::Status status) {
                if (status == QAbstractOAuth::Status::Granted){
                    qDebug() << "[SpotifyWrapper::SpotifyWrapper()][QOAuth2AuthorizationCodeFlow::statusChanged][INFO] Granted";
                    emit authenticated();
                 }
                else if(status == QAbstractOAuth::Status::TemporaryCredentialsReceived){
                    qDebug() << "[SpotifyWrapper::SpotifyWrapper()][QOAuth2AuthorizationCodeFlow::statusChanged][INFO] TemporaryCredentialsReceived";
                }
                else if(status == QAbstractOAuth::Status::RefreshingToken){
                    qDebug() << "[SpotifyWrapper::SpotifyWrapper()][QOAuth2AuthorizationCodeFlow::statusChanged][INFO] RefreshingToken";
                }
                else if(status == QAbstractOAuth::Status::NotAuthenticated){
                    qDebug() << "[SpotifyWrapper::SpotifyWrapper()][QOAuth2AuthorizationCodeFlow::statusChanged][INFO] NotAuthenticated";
                }
            }
        );

    // In this callback we configure the response as defined in the documentation
    // see https://developer.spotify.com/documentation/general/guides/authorization/code-flow/
    oauth2->setModifyParametersFunction(
            [&](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
                qDebug() << "[SpotifyWrapper::SpotifyWrapper()][QAbstractOAuth::Stage] Status changed";
                if(stage == QAbstractOAuth::Stage::RequestingAuthorization) {
                    parameters->insert("duration", "permanent");
                    parameters->insert("redirect_uri",Constants::SpotifyWrapper::redirectUri);
                    parameters->insert("response_type",Constants::SpotifyWrapper::responseType);
                }
                if(stage == QAbstractOAuth::Stage::RequestingAccessToken){
                    parameters->insert("redirect_uri",Constants::SpotifyWrapper::redirectUri);
                    parameters->insert("state",state);
                    parameters->insert("grant_type", Constants::SpotifyWrapper::grantType);
                    parameters->insert("code",code);
                }
            }
    );

    // open the web desktop browser to perform the authentication
    connect(&(*oauth2),&QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    connect(&(*oauth2),&QOAuth2AuthorizationCodeFlow::granted,this,
            &SpotifyWrapper::granted);

    connect(&(*oauth2),&QOAuth2AuthorizationCodeFlow::error,this,
            [&](const QString &error, const QString &errorDescription, const QUrl &uri){
                qDebug() <<"[SpotifyWrapper::SpotifyWrapper()][QOAuth2AuthorizationCodeFlow::error][ERROR] "<< error << " " << errorDescription;
            }
        );

    connect(&(*oauth2), &QOAuth2AuthorizationCodeFlow::authorizationCallbackReceived,
                this, [this](const QVariantMap &data){
           code = data["code"].toString();
           state = data["state"].toString();
    });

}
void SpotifyWrapper::grant()
{
    oauth2->grant();
}
void SpotifyWrapper::granted()
{
    // when granted, request the user id from the spotify server
    requestUser();
}
void SpotifyWrapper::requestUser()
{

    QNetworkReply *reply = me();
    QObject::connect(reply,
                     &QNetworkReply::finished,
                     [=]() {
                        reply->deleteLater();

                        if (reply->error() != QNetworkReply::NoError) {
                            qDebug() << "[SpotifyWrapper::requestUser()][QNetworkReply::finished][ERROR] " << reply->errorString();
                            return;
                        }

                        const auto data = reply->readAll();
                        const auto document = QJsonDocument::fromJson(data);
                        const auto userJson = document.object();

                        //SpotifyUser user(userJson.value("display_name").toString(),userJson.value("email").toString(), userJson.value("id").toString()) ;
                        SpotifyUser user(userJson);
                        // after receive the user id emit a signal to notify the application
                        emit userReplied(user);
                    }
               );
}

QNetworkReply  * SpotifyWrapper::me()
{
    return oauth2->get(QUrl("https://api.spotify.com/v1/me"));
}
void SpotifyWrapper::requestSearchTrack(QString  phrase, uint8_t limit)
{

    QString query = "https://api.spotify.com/v1/search?q=" + phrase + "&type=track" + "&limit=" + QString::number(limit);
    auto reply = oauth2->get(QUrl(query));
    QObject::connect(reply,
                     &QNetworkReply::finished,
                     [=]() {
                        reply->deleteLater();

                        if (reply->error() != QNetworkReply::NoError) {
                            qDebug() << "[SpotifyWrapper::searchTrack()][QNetworkReply::finished][ERROR] " << reply->errorString();
                            return;
                        }

                        // as specified at documentation,
                        // each search returns a reply that contains
                        // an array with the desired objects called by
                        // \"items\"
                        const auto data = reply->readAll();
                        const auto document = QJsonDocument::fromJson(data);
                        const auto root = document.object();
                        const auto tracks = root["tracks"].toObject();
                        const auto jsonItemsArray = tracks["items"].toArray();

                        // Our object answer
                        QList<SpotifyTrack> tracksList;

                        foreach (const QJsonValue & item, jsonItemsArray) {
                            QString music;
                            const auto track = item.toObject();
                            // create our DAO Spotify Track to communicate with the UI
                            SpotifyTrack spotifyTrack(track);
                            // add our track to list
                            tracksList.push_back(spotifyTrack);

                        }
                        // return out list of tracks to UI
                        emit trackReplied(tracksList);
                    }
                );
}


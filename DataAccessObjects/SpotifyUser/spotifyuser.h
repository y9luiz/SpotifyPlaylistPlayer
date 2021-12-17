#ifndef SPOTIFYUSER_H
#define SPOTIFYUSER_H

#include <QString>
#include "../abstractdaoobject.h"
class SpotifyUser : public AbstractDAOObject
{
private:
     void setupSpotifyUser(const QString & name, const QString & email,const QString & id)
     {
         map_["name"] = name;
         map_["email"] = email;
         map_["id"] = id;
     }
     virtual void fromJsonObject(const QJsonObject & jsonObject) override
     {
         QString name = jsonObject.value("display_name").toString();
         QString email = jsonObject.value("email").toString();
         QString id = jsonObject.value("id").toString();

         setupSpotifyUser(name,email,id);
     }

public:

    SpotifyUser() : SpotifyUser(QString(""),QString(""),QString("")){
    };

    SpotifyUser(QString && name, QString && email, QString && id)
    {
       setupSpotifyUser(std::move(name),std::move(email),std::move(id));
    }

    SpotifyUser(const QJsonObject & jsonObject)
    {
       fromJsonObject(jsonObject);
    }

    inline QString name(){return map_["name"].toString();};
    inline QString email(){return map_["email"].toString();};
    inline QString id(){return map_["id"].toString();};
};

#endif // SPOTIFYUSER_H

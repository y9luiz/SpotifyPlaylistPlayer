#include "localplaylistfilemanager.h"
#include <cassert>
LocalPlaylistFileManager::LocalPlaylistFileManager(QString directoryName,QObject *parent)
    : QObject{parent}, directoryName_(directoryName), dir_(directoryName_)
{
    qDebug () << QDir::currentPath();
    createDirectory();
    dir_.setFilter(QDir::Files | QDir::Readable | QDir::Writable);
}
void LocalPlaylistFileManager::createDirectory() const
{
    if(!QDir().exists(directoryName_))
         if(!QDir().mkdir(directoryName_));
            qDebug() << "[LocalPlaylistFileManager::createDirectory()][INFO] could not create directory " << directoryName_;
}
QJsonDocument LocalPlaylistFileManager::loadJsonFile(QString fileName) const {
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void LocalPlaylistFileManager::saveJsonFile(QJsonDocument document, QString fileName) const {
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}
QStringList LocalPlaylistFileManager::listJsonFilesInDirectory() const
{
    QStringList jsonFilesName;
    jsonFilesName = dir_.entryList( QStringList() << "*.json");
    return jsonFilesName;
}
QList<LocalPlaylist> LocalPlaylistFileManager::loadLocalPlaylistsFromDisk() const
{
    QStringList jsonFileNames = listJsonFilesInDirectory();
    QList<LocalPlaylist> localPlaylists;
    foreach(const auto & jsonFilename, jsonFileNames)
    {
        auto jsonDocument = loadJsonFile(jsonFilename);
        auto jsonLocalPlayList = jsonDocument.object();
        localPlaylists.push_back(LocalPlaylist(jsonLocalPlayList));
    }
    return localPlaylists;
}
void LocalPlaylistFileManager::saveLocalPlaylistToDisk(const LocalPlaylist & localPlayList) const
{
    assert(("[LocalPlaylistFileManager::saveLocalPlaylistToDisk] not implemented yet",false));
}

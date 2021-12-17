# SpotifyPlaylistPlayer

This project is compatible with Windows and Linux.

### Test enviroment

#### Linux

  * OS: Ubuntu 18.04 and Ubuntu 20.04
  * Compiler: gcc 9.3
  * Qt Version: 5.12
  
#### Windows

  * OS Version: Windows 10 x64
  * Compiler: MS BUILD 2017
  * Qt Version: 6.3

# How build

## Linux 
### Dependencies:
All these dependencies could be installed by apt get if you are using 
ubuntu 20.04
- libqt5widgets5
- libqt5network5
- libqt5networkauth (only available via apt-get on ubuntu 20.04)
- libqt5multimedia
- libqt5gui5
- cmake
- gcc 9 or higher

`You can also install these dependencies using the Qt Creator.`

### Compiling
After install the dependencies, do:

```
mkdir build
cd build
cmake ..
make
```
## Windows
You must install the Qt packages

- Widgets
- Network
- NetworkOauth
- Multimedia
- GUI

### Compiling

Open the project with Qt Creator and have fun

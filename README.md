# TInvestor

[![CI](https://github.com/Gris87/TInvestor/actions/workflows/ci.yaml/badge.svg)](https://github.com/Gris87/TInvestor/actions/workflows/ci.yaml)
[![Windows coverage](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/Gris87/c6ca4b78912b06400bb99a569cc9fccd/raw/TInvestor-Windows-cobertura-coverage.json&label=Windows%20coverage)](https://github.com/Gris87/TInvestor/actions/workflows/ci.yaml)
[![Linux coverage](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/Gris87/c6ca4b78912b06400bb99a569cc9fccd/raw/TInvestor-Linux-lcov-coverage.json&label=Linux%20coverage)](https://github.com/Gris87/TInvestor/actions/workflows/ci.yaml)

TInvestor is a T-Bank Investment trading robot

---

## How to build

You have to install prerequisites in order to build TInvestor.

### Install prerequisites on Windows

1. Install MSVC 2022

- Please visit [Microsoft Visual Studio free downloads](https://visualstudio.microsoft.com/ru/free-developer-offers) and install Visual Studio Community with VC compiler

2. Install Qt 6.9.0

- Just go to [official Qt download page](https://www.qt.io/download-qt-installer-oss) and install Qt 6.9.0. Please also install addons qt5compat and qtcharts

3. Install zlib 1.3.1

- Run commands below
```
ZLIB_PATH=SET_PATH_TO_ZLIB (for example: C:\Qt\zlib)

git clone git@github.com:madler/zlib.git %ZLIB_PATH%
cd %ZLIB_PATH%
git checkout v1.3.1 -b temp
mkdir build
cd build
cmake ^
  -S .. ^
  -B . ^
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON ^
  -DCMAKE_BUILD_TYPE=Debug ^
  -DCMAKE_INSTALL_PREFIX="%ZLIB_PATH%\install"
cmake --build . --config Debug
cmake --install . --config Debug
cmake ^
  -S .. ^
  -B . ^
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX="%ZLIB_PATH%\install"
cmake --build . --config Release
cmake --install . --config Release
```

4. Install quazip 1.5

- Run commands below
```
QTDIR=SET_PATH_TO_QT (for example: C:\Qt\6.9.0\msvc2022_64)
ZLIB_PATH=SET_PATH_TO_ZLIB (for example: C:\Qt\zlib)
QUAZIP_PATH=SET_PATH_TO_QUAZIP (for example: C:\Qt\quazip)

git clone git@github.com:stachenov/quazip.git %QUAZIP_PATH%
cd %QUAZIP_PATH%
git checkout v1.5 -b temp
mkdir build
cd build
cmake ^
  -S .. ^
  -B . ^
  -DCMAKE_BUILD_TYPE=Debug ^
  -DCMAKE_INSTALL_PREFIX="%QUAZIP_PATH%\install" ^
  -DCMAKE_PREFIX_PATH="%QTDIR%\lib\cmake" ^
  -DZLIB_LIBRARY:FILEPATH="%ZLIB_PATH%\install\lib\zlibd.lib" ^
  -DZLIB_INCLUDE_DIR:PATH="%ZLIB_PATH%\install\include"
cmake --build . --config Debug
cmake --install . --config Debug
cmake ^
  -S .. ^
  -B . ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX="%QUAZIP_PATH%\install" ^
  -DCMAKE_PREFIX_PATH="%QTDIR%\lib\cmake" ^
  -DZLIB_LIBRARY:FILEPATH="%ZLIB_PATH%\install\lib\zlib.lib" ^
  -DZLIB_INCLUDE_DIR:PATH="%ZLIB_PATH%\install\include"
cmake --build . --config Release
cmake --install . --config Release
```

### Install prerequisites on Ubuntu

1. Install Qt 6.9.0

- Just go to [official Qt download page](https://www.qt.io/download-qt-installer-oss) and install Qt 6.9.0. Please also install addons qt5compat and qtcharts

2. Install zlib 1.3.1

- Run commands below
```
ZLIB_PATH=SET_PATH_TO_ZLIB (for example: /home/user/Qt/zlib)

git clone git@github.com:madler/zlib.git ${ZLIB_PATH}
cd ${ZLIB_PATH}
git checkout v1.3.1 -b temp
mkdir build
cd build
cmake \
  -S .. \
  -B . \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX="${ZLIB_PATH}/install"
cmake --build . --config Debug
cmake --install . --config Debug
cmake \
  -S .. \
  -B . \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="${ZLIB_PATH}/install"
cmake --build . --config Release
cmake --install . --config Release
```

3. Install quazip 1.5

- Run commands below
```
QTDIR=SET_PATH_TO_QT (for example: /home/user/Qt/6.9.0/gcc_64)
ZLIB_PATH=SET_PATH_TO_ZLIB (for example: /home/user/Qt/zlib)
QUAZIP_PATH=SET_PATH_TO_QUAZIP (for example: /home/user/Qt/quazip)

git clone git@github.com:stachenov/quazip.git ${QUAZIP_PATH}
cd ${QUAZIP_PATH}
git checkout v1.5 -b temp
mkdir build
cd build
cmake \
  -S .. \
  -B . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX="${QUAZIP_PATH}/install" \
  -DCMAKE_PREFIX_PATH="${QTDIR}/lib/cmake" \
  -DZLIB_LIBRARY:FILEPATH="${ZLIB_PATH}/install/lib/libz.a" \
  -DZLIB_INCLUDE_DIR:PATH="${ZLIB_PATH}/install/include"
cmake --build . --config Debug
cmake --install . --config Debug
cmake \
  -S .. \
  -B . \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="${QUAZIP_PATH}/install" \
  -DCMAKE_PREFIX_PATH="${QTDIR}/lib/cmake" \
  -DZLIB_LIBRARY:FILEPATH="${ZLIB_PATH}/install/lib/libz.a" \
  -DZLIB_INCLUDE_DIR:PATH="${ZLIB_PATH}/install/include"
cmake --build . --config Release
cmake --install . --config Release
```

---

## How to install

### Installing on Windows

- Please go to [release page](https://github.com/Gris87/TInvestor/releases)
- Download and extract latest release
- Run TInvestor.exe

### Installing on Ubuntu

- Please go to [release page](https://github.com/Gris87/TInvestor/releases)
- Download and extract latest release
- Execute `post_install.sh` script (It will create desktop file for application in order to let you start TInvestor from launcher menu and will create desktop file on your desktop)
- Run TInvestor from launcher menu

**!!!Please note that development is ongoing. So, it doesn't work yet!!!**

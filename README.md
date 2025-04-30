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

2. Install CMake

- Please visit [CMake download page](https://cmake.org/download) and install CMake 4.0.1

3. Install Qt 6.9.0

- Just go to [official Qt download page](https://www.qt.io/download-qt-installer-oss) and install Qt 6.9.0. Please also install addons qt5compat and qtcharts

4. Install zlib 1.3.1

- Run commands below

```
set ZLIB_PATH=SET_PATH_TO_ZLIB (for example: C:\Qt\zlib)

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

5. Install quazip 1.5

- Run commands below

```
set QTDIR=SET_PATH_TO_QT (for example: C:\Qt\6.9.0\msvc2022_64)
set ZLIB_PATH=SET_PATH_TO_ZLIB (for example: C:\Qt\zlib)
set QUAZIP_PATH=SET_PATH_TO_QUAZIP (for example: C:\Qt\quazip)

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

6. Install grpc via vcpkg

- Run commands below

```
set VCPKG_PATH=SET_PATH_TO_VCPKG (for example: C:\Qt\vcpkg)

git clone git@github.com:microsoft/vcpkg.git %VCPKG_PATH%
cd %VCPKG_PATH%
git checkout 2025.04.09 -b temp
bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install grpc
```

7. Install googletest 1.16.0

- Run commands below

```
set GOOGLETEST_PATH=SET_PATH_TO_GOOGLETEST (for example: C:\Qt\googletest)

git clone git@github.com:google/googletest.git %GOOGLETEST_PATH%
cd %GOOGLETEST_PATH%
git checkout v1.16.0 -b temp
```

8. Setup environment variables

- Add environment variables to your system

```
ZLIB_PATH=SET_PATH_TO_ZLIB (for example: C:\Qt\zlib)
QUAZIP_PATH=SET_PATH_TO_QUAZIP (for example: C:\Qt\quazip)
VCPKG_PATH=SET_PATH_TO_VCPKG (for example: C:\Qt\vcpkg)
GOOGLETEST_PATH=SET_PATH_TO_GOOGLETEST (for example: C:\Qt\googletest)
```

9. Run QT Creator and compile project

### Install prerequisites on Ubuntu

1. Install packages

- Run commands below

```
sudo apt-get update
sudo apt-get install -y \
    bzip2 curl gcc g++ perl cmake make pkg-config \
    libgl1-mesa-dev libxkbcommon-dev libxkbcommon-x11-dev \
    libxcb-cursor0 libxcb-icccm4 libxcb-image0 libxcb-keysyms1 \
    libxcb-randr0 libxcb-render-util0 libxcb-shape0 libxcb-shm0 \
    libxcb-sync1 libxcb-util1 libxcb-xfixes0 libxcb-xkb1 libxcb1
```

2. Install Qt 6.9.0

- Just go to [official Qt download page](https://www.qt.io/download-qt-installer-oss) and install Qt 6.9.0. Please also install addons qt5compat and qtcharts

3. Install zlib 1.3.1

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

4. Install quazip 1.5

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

5. Install grpc via vcpkg

- Run commands below

```
VCPKG_PATH=SET_PATH_TO_VCPKG (for example: /home/user/Qt/vcpkg)

git clone git@github.com:microsoft/vcpkg.git ${VCPKG_PATH}
cd ${VCPKG_PATH}
git checkout 2025.04.09 -b temp
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install grpc
```

6. Install googletest 1.16.0

- Run commands below

```
GOOGLETEST_PATH=SET_PATH_TO_GOOGLETEST (for example: /home/user/Qt/googletest)

git clone git@github.com:google/googletest.git ${GOOGLETEST_PATH}
cd ${GOOGLETEST_PATH}
git checkout v1.16.0 -b temp
```

7. Setup environment variables

- Add environment variables to your /etc/environment

```
ZLIB_PATH=SET_PATH_TO_ZLIB (for example: /home/user/Qt/zlib)
QUAZIP_PATH=SET_PATH_TO_QUAZIP (for example: /home/user/Qt/quazip)
VCPKG_PATH=SET_PATH_TO_VCPKG (for example: /home/user/Qt/vcpkg)
GOOGLETEST_PATH=SET_PATH_TO_GOOGLETEST (for example: /home/user/Qt/googletest)
```

8. Run QT Creator and compile project

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

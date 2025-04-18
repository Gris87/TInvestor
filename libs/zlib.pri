isEmpty(ZLIB_PATH):ZLIB_PATH=$$(ZLIB_PATH)

isEmpty(ZLIB_PATH) {
    win32-msvc* {
        ZLIB_PATH = E:/Qt/zlib-1.3.1
    } else {
        ZLIB_PATH = /home/gris/Qt/zlib-1.3.1
    }
}

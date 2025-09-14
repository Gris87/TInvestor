isEmpty(ZLIB_PATH):ZLIB_PATH=$$(ZLIB_PATH)

isEmpty(ZLIB_PATH) {
    win32* {
        ZLIB_PATH = E:/Qt/zlib
    } else {
        ZLIB_PATH = /home/gris/Qt/zlib
    }
}

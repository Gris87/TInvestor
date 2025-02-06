import sys
import tornado.web
import tornado.websocket
import tornado.ioloop


class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        print("WebSocket opened")

    def on_message(self, message):
        self.write_message(f"You said: {message}")

    def on_close(self):
        print("WebSocket closed")


def main():
    app = tornado.web.Application([(r'/ws', WebSocketHandler)])

    app.listen(8765)
    tornado.ioloop.IOLoop.current().start()

    sys.exit(0)

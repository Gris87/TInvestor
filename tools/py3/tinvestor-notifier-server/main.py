import argparse
import json
import http.server
import ssl
import sys

from functools import partial
from loguru import logger
from pathlib import Path
from urllib.parse import urlparse, parse_qs


DEFAULT_PORT = 8041

MS_IN_SECOND = 1000


class MyHandler(http.server.BaseHTTPRequestHandler):
    def __init__(self, path_to_notifications, *args, **kwargs):
        self.path_to_notifications = path_to_notifications

        super().__init__(*args, **kwargs)


    def do_GET(self):
        parsed_path = urlparse(self.path)

        if parsed_path.path == "/notifications":
            self._handle_notifications_request(parse_qs(parsed_path.query))
        else:
            self._handle_unknown_request()


    def _handle_unknown_request(self):
        body = {
            "description": "Unknown URL"
        }

        self.send_response(404)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.end_headers()
        self.wfile.write(json.dumps(body, ensure_ascii=False).encode("utf-8"))


    def _handle_invalid_request(self, description):
        body = {
            "description": description
        }

        self.send_response(400)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.end_headers()
        self.wfile.write(json.dumps(body, ensure_ascii=False).encode("utf-8"))


    def _handle_notifications_request(self, params):
        from_timestamp_str = params.get("from", ["0"])[0]

        try:
            from_timestamp = int(from_timestamp_str) / MS_IN_SECOND
        except ValueError:
            self._handle_invalid_request(f"from argument {from_timestamp_str} is invalid")

            return

        notifications = []

        for record_path in sorted(Path(self.path_to_notifications).rglob("*.json"), key=lambda x: x.stat().st_mtime):
            if record_path.stat().st_mtime >= from_timestamp:
                with open(record_path, "r", encoding="utf-8") as f:
                    notifications.append(json.loads(f.read()))

        body = {
            "notifications": notifications
        }

        self.send_response(200)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.end_headers()
        self.wfile.write(json.dumps(body, ensure_ascii=False).encode("utf-8"))


def notifier(args):
    server_address = ("", args.port)

    httpd = http.server.HTTPServer(server_address, partial(MyHandler, args.path_to_notifications))

    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.load_cert_chain(certfile="cert.pem", keyfile="key.pem")

    httpd.socket = context.wrap_socket(httpd.socket, server_side=True)

    logger.info(f"Listening at port {args.port}")
    httpd.serve_forever()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--port",
        dest="port",
        type=int,
        default=DEFAULT_PORT,
        help="TCP port for listening"
    )
    parser.add_argument(
        "--path-to-notifications-path",
        dest="path_to_notifications",
        type=str,
        default="",
        help="Path to notifications folder"
    )
    args = parser.parse_args()

    if args.port <= 0 or args.port >= 65636:
        logger.error("Please specify valid port")

        sys.exit(1)

    if args.path_to_notifications == "":
        logger.error("Please specify path to notifications folder with --path-to-notifications-path")

        sys.exit(1)

    sys.exit(0 if notifier(args) else 1)

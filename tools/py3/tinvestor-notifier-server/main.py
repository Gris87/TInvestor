import argparse
import json
import sys

from loguru import logger


DEFAULT_PORT = 8041


def notifier(args):
    print(args)


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

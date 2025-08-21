import argparse
import sys


def telegram_bot(args):
    return True


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--chat-id",
        dest="chat_id",
        type=str,
        default="",
        help="Telegram chat ID"
    )
    args = parser.parse_args()

    sys.exit(0 if telegram_bot(args) else 1)

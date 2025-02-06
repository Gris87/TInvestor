import sys


def check_style():
    print("Hello")

    return True


def main():
     sys.exit(0 if check_style() else 1)

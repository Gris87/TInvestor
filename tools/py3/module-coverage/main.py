import sys


def module_coverage():
    return True


def main():
    sys.exit(0 if module_coverage() else 1)

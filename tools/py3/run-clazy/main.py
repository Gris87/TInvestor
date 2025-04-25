import sys

def run_clazy():
    print("run_clazy")

    return True


def main():
    sys.exit(0 if run_clazy() else 1)

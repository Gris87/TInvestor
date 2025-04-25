def run_clang_tidy():
    print("run_clang_tidy")

    return True


def main():
    sys.exit(0 if run_clang_tidy() else 1)

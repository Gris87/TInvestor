import os
import shutil
import subprocess
import sys
import time


TIMEOUT = 15 * 60 # 15 minutes

LIGHT_MAGENTA_COLOR = "\033[95m"
END_COLOR = "\033[0m"


def generate_doc():
    result_code = 0

    while True:
        shutil.rmtree("build/doxygen")
        process = subprocess.Popen("doxygen", stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

        os.set_blocking(process.stdout.fileno(), False)

        timeout_expired = False
        start_time = int(time.time())

        while True:
            return_code = process.poll()

            bytes = process.stdout.read()
            if bytes is not None:
                sys.stdout.buffer.write(bytes)
                sys.stdout.buffer.flush()
            else:
                time.sleep(0.1)

            if return_code is not None:
                result_code = return_code

                break
            else:
                time_elapsed = int(time.time()) - start_time

                if time_elapsed >= TIMEOUT:
                    timeout_expired = True
                    process.terminate()

        if not timeout_expired:
            break

        timeout_str = "\n"
        timeout_str += f"{LIGHT_MAGENTA_COLOR}======================================={END_COLOR}\n"
        timeout_str += f"{LIGHT_MAGENTA_COLOR}Timeout expired! Restarting...{END_COLOR}\n"
        timeout_str += f"{LIGHT_MAGENTA_COLOR}======================================={END_COLOR}\n"
        timeout_str += "\n"

        sys.stdout.buffer.write(timeout_str.encode("utf-8"))
        sys.stdout.buffer.flush()

    return result_code == 0


def main():
    sys.exit(0 if generate_doc() else 1)

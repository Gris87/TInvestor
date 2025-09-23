import argparse
import json
import sys


def operations_stats(args):
    return True


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--path-to-operations",
        dest="path_to_operations",
        type=str,
        default="build/Desktop-Debug/app/build/data/autopilot/4fa4713d40ede79c630b609d3a95c5ed/operations.json",
        help="Path to operations.json file"
    )
    parser.add_argument(
        "--path-to-logs",
        dest="path_to_logs",
        type=str,
        default="build/Desktop-Debug/app/build/data/autopilot/4fa4713d40ede79c630b609d3a95c5ed/logs.json",
        help="Path to logs.json file"
    )
    args = parser.parse_args()

    sys.exit(0 if operations_stats(args) else 1)

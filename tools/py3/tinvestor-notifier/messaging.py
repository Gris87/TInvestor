import json
import time
import uuid

from loguru import logger
from pathlib import Path


MS_IN_SECOND = 1000


def store_message(args, message_type, msg, data):
    logger.info(f"Store message: {msg}")

    timestamp = int(time.time() * MS_IN_SECOND)
    record_uuid = uuid.uuid4()

    record = {
        "timestamp": timestamp,
        "type": message_type,
        "text": msg,
        "data": data
    }

    output_path = Path(args.output)
    output_path.mkdir(parents=True, exist_ok=True)

    record_path = output_path / f"{timestamp}_{record_uuid}.json"

    with open(record_path, "w", encoding="utf-8") as f:
        json.dump(record, f, ensure_ascii=False)

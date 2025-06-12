#!/usr/bin/env python3
"""
Combine all JSON and JSONL files in a directory into a single JSONL output file.
Usage:
    python combine_json_dir.py --input-dir <raw_data_dir> --output-dir <processed_data_dir>
"""
import argparse
import json
from pathlib import Path
from typing import List, Dict, Any

def load_json_file(path: Path) -> List[Dict[str, Any]]:
    """
    Load a regular JSON file.
    If it contains a list at top-level, return that list.
    If it's a single object (dict), return a one-element list.
    """
    with path.open('r', encoding='utf-8') as f:
        data = json.load(f)
    if isinstance(data, list):
        return data
    elif isinstance(data, dict):
        return [data]
    else:
        raise ValueError(f"Unsupported JSON top-level type in {path}")

def load_jsonl_file(path: Path) -> List[Dict[str, Any]]:
    """
    Load a JSON Lines file: one JSON object per line.
    """
    records: List[Dict[str, Any]] = []
    with path.open('r', encoding='utf-8') as f:
        for i, line in enumerate(f, 1):
            line = line.strip()
            if not line:
                continue
            try:
                records.append(json.loads(line))
            except json.JSONDecodeError as e:
                raise ValueError(f"Invalid JSON on line {i} of {path}: {e}")
    return records

def combine_json(dirpath: Path) -> List[Dict[str, Any]]:
    """
    Find all .jsonl and .json files in dirpath (non-recursively),
    load them, and return the concatenated list of records.
    """
    all_records: List[Dict[str, Any]] = []
    for ext, loader in (("jsonl", load_jsonl_file), ("json", load_json_file)):
        for p in dirpath.glob(f"*.{ext}"):
            print(f"Loading {p.name} ({ext})...")
            recs = loader(p)
            all_records.extend(recs)
    return all_records

def write_jsonl(records: List[Dict[str, Any]], outpath: Path):
    """
    Write out a list of dicts as JSON Lines.
    """
    outpath.parent.mkdir(parents=True, exist_ok=True)
    with outpath.open('w', encoding='utf-8') as f:
        for rec in records:
            f.write(json.dumps(rec, ensure_ascii=False) + "\n")
    print(f"Wrote {len(records)} records to {outpath}")

def main():
    parser = argparse.ArgumentParser(
        description="Combine all JSON/JSONL files in a directory into one JSONL file"
    )
    parser.add_argument(
        '--input-dir', '-i',
        type=Path,
        default=Path('../data/raw'),
        help='Directory containing .json and/or .jsonl files (default: ../data/raw)'
    )
    parser.add_argument(
        '--output-dir', '-o',
        type=Path,
        default=Path('../data/processed'),
        help='Directory where combined JSONL should be saved (default: ../data/processed)'
    )
    parser.add_argument(
        '--output-filename', '-f',
        type=str,
        default='master_data.jsonl',
        help='Filename for the combined JSONL (default: combined.jsonl)'
    )
    args = parser.parse_args()

    input_dir: Path = args.input_dir
    output_dir: Path = args.output_dir
    output_file: Path = output_dir / args.output_filename

    if not input_dir.is_dir():
        parser.error(f"Input directory '{input_dir}' does not exist or is not a directory.")

    records = combine_json(input_dir)
    write_jsonl(records, output_file)

if __name__ == '__main__':
    main()

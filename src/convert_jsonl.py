import json

def json_to_jsonl(input_path: str, output_path: str):
    with open(input_path, 'r') as fin:
        data = json.load(fin)

    with open(output_path, 'w') as fout:
        # If the JSON is an array of objects
        if isinstance(data, list):
            for obj in data:
                fout.write(json.dumps(obj) + '\n')
        # If it’s a single object and you still want one-per-line
        elif isinstance(data, dict):
            fout.write(json.dumps(data) + '\n')
        else:
            raise ValueError("Unsupported JSON top-level type")

if __name__ == "__main__":
    json_to_jsonl('../data/raw/cwe_119.json', '../data/processed/cwe_119.jsonl')

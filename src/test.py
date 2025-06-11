import pandas as pd

def json_to_csv(json_path: str, csv_path: str):
    """
    Reads a JSON file into a pandas DataFrame and writes it out as CSV.

    :param json_path: Path to the input JSON file.
    :param csv_path: Path where the output CSV should be saved.
    """
    # Read the JSON file. If it’s a line-delimited JSON, use lines=True.
    df = pd.read_json(json_path, lines=False)

    # Write to CSV (index=False to omit row numbers)
    df.to_csv(csv_path, index=False)
    print(f"Successfully wrote CSV to {csv_path}")

if __name__ == "__main__":
    json_path = "C:/Users/nagar/Downloads/MSR_data_cleaned_json/MSR_data_cleaned.json"
    csv_path = "data.csv"
    json_to_csv(json_path, csv_path)



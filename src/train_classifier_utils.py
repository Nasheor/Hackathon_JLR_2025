"""
Utility functions for the classifier notebook / script.
────────────────────────────────────────────────────────
Keeps the notebook tidy and re-usable by other teammates.
"""

import json, random, pathlib, os
from typing import Dict

import numpy as np
import torch
from datasets import load_dataset, DatasetDict


# ------------------------------------------------------------------ #
#  Reproducibility helpers                                            #
# ------------------------------------------------------------------ #
def seed_everything(seed: int = 42) -> None:
    """Seed Python, NumPy, and PyTorch RNGs."""
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    os.environ["PYTHONHASHSEED"] = str(seed)


# ------------------------------------------------------------------ #
#  Dataset loader                                                    #
# ------------------------------------------------------------------ #
def load_splits(train_file: pathlib.Path,
                valid_file: pathlib.Path) -> DatasetDict:
    """
    Turns two JSONL files into a HuggingFace DatasetDict so the HF
    Trainer API works out-of-the-box.
    Expected JSON schema per line:
        {
          "Function before": "...",
          "is_vuln": 1
        }
    """
    ds = load_dataset(
        "json",
        data_files={
            "train": str(train_file),
            "validation": str(valid_file)
        }
    )
    return ds

"""
src/eval_utils.py
─────────────────
Shared helpers for Task 5 evaluation.
"""

import json, pathlib, time
from typing import List, Dict
import numpy as np
from tqdm.auto import tqdm
from transformers import AutoTokenizer, AutoModelForSequenceClassification, Trainer
import torch


def load_adapter(model_dir: pathlib.Path, device: str = "cuda"):
    """
    Re-attach the LoRA adapter to the frozen CodeBERT base and return a HF
    `Trainer`-style model ready for inference (no gradients).
    """
    from peft import PeftModel
    base = AutoModelForSequenceClassification.from_pretrained(
        "microsoft/codebert-base",
        num_labels=2
    )
    model = PeftModel.from_pretrained(base, model_dir)
    model = model.merge_and_unload()   # merge LoRA → full weights
    model.eval().to(device)
    return model


def batched_logits(model, tokenizer, texts: List[str],
                   batch_size: int = 16, device: str = "cuda") -> np.ndarray:
    """
    Tokenise & run the classifier in batches → returns logits array (N, 2)
    """
    all_logits = []
    with torch.no_grad():
        for i in range(0, len(texts), batch_size):
            batch = tokenizer(
                texts[i:i + batch_size],
                truncation=True,
                padding="longest",
                max_length=512,
                return_tensors="pt"
            ).to(device)
            logits = model(**batch).logits
            all_logits.append(logits.cpu().numpy())
    return np.vstack(all_logits)

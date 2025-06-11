"""
Small helpers shared by notebooks / scripts for Task 4.
"""

from __future__ import annotations
import json, pathlib
from typing import List, Dict

import numpy as np
import faiss, torch
from sklearn.preprocessing import normalize
from transformers import AutoTokenizer, AutoModel


class CodeBERTEmbedder:
    """
    Wraps microsoft/codebert-base and returns a 768-d CLS embedding.
    Usage:
        embedder = CodeBERTEmbedder("cuda")
        vec = embedder("int main(){…}")
    """
    def __init__(self, device: str = "cuda"):
        self.device = device
        self.tokenizer = AutoTokenizer.from_pretrained(
            "microsoft/codebert-base",
            trust_remote_code=True
        )
        self.model = (
            AutoModel.from_pretrained("microsoft/codebert-base")
            .to(device)
            .eval()
        )
        torch.set_grad_enabled(False)

    def __call__(self, text: str) -> np.ndarray:
        ids = self.tokenizer(
            text,
            truncation=True,
            max_length=512,
            return_tensors="pt"
        ).to(self.device)
        with torch.no_grad():
            cls = self.model(**ids).last_hidden_state[:, 0, :].cpu().numpy()[0]
        return cls.astype(np.float32)      # (768,)


def write_faiss_and_meta(
    vectors: np.ndarray,
    meta_records: List[Dict],
    out_index: pathlib.Path,
    out_meta:  pathlib.Path
) -> None:
    """
    * L2-normalises vectors
    * Builds IndexFlatIP
    * Writes .faiss and .jsonl side-car
    """
    vectors = normalize(vectors)          # cosine ≈ inner product
    index = faiss.IndexFlatIP(vectors.shape[1])
    index.add(vectors.astype(np.float32))
    faiss.write_index(index, str(out_index))

    with out_meta.open("w", encoding="utf-8") as w:
        for rec in meta_records:
            w.write(json.dumps(rec, ensure_ascii=False) + "\n")

    # ---------------------------------------------
    # Final user-friendly message (no relative_to)
    # ---------------------------------------------
    try:
        # Nice relative path if possible
        rel = out_index.relative_to(pathlib.Path.cwd())
    except ValueError:
        # Different drive / parent directory → use absolute
        rel = out_index
    print(f"💾  Saved {len(vectors)} × {vectors.shape[1]} index → {rel}")
    # plain absolute path; guaranteed not to throw
    # print(f"💾  Saved index: {out_index}")

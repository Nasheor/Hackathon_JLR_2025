import numpy as np, sklearn.metrics as sk

def regression_metrics(preds, labels):
    preds  = preds.flatten()
    labels = labels.flatten()
    return {
        "mae":  sk.mean_absolute_error(labels, preds),
        "rmse": sk.mean_squared_error(labels, preds, squared=False),
        "r2":   sk.r2_score(labels, preds),
    }

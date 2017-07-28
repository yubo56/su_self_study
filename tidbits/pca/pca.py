#!/usr/bin/env python
""" test out PCA """
import numpy as np

NUM_ENTRIES = 1000
NUM_FEATURES = 20 # total num features for each entry
NUM_PRINCIPALS = 3 # number of principal components (PC)
NUM_PRINCIPAL_FEATURES = 3 # num features each PC increments
PRINCIPAL_MAGNITUDE = 3 # how much to increment each PC feature
NOISE_SIZE = PRINCIPAL_MAGNITUDE # stdev of noise to add

def generate_data(num_entries):
    """
    generates some fake data for PCA
    """
    entries = []
    nonzero_indexes = {
        i: np.random.choice(NUM_FEATURES, NUM_PRINCIPAL_FEATURES, replace=False)
        for i in range(NUM_PRINCIPALS)
    }
    print("The feature vectors are:")
    for indicies in nonzero_indexes:
        print(nonzero_indexes[indicies])

    for _ in range(num_entries):
        entry = np.zeros(NUM_FEATURES)
        for j in nonzero_indexes[np.random.choice(NUM_PRINCIPALS)]:
            entry[j] += PRINCIPAL_MAGNITUDE
        entry += np.random.randn(NUM_FEATURES) * NOISE_SIZE
        entries.append(entry)
    return entries

def get_pca(data):
    """
    gets eigenvalues, eigenvectors + labels of each entry in data
    """
    _, s, v = np.linalg.svd(data)
    ret = []
    for idx in range(len(s)):
        if s[idx] > 1:
            normed_vector = v[idx] / v[idx][np.argmax(abs(v[idx]))]
            ret.append({s[idx]: normed_vector})
    return ret

if __name__ == '__main__':
    generated_data = generate_data(NUM_ENTRIES)
    eigen_objs = get_pca(generated_data)
    for entry_dict in eigen_objs:
        print(entry_dict)

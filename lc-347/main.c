/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

typedef struct HashNode {
    int key;
    int freq;
} HashNode;

int compare(const void *a, const void *b) {
    HashNode* hA = *(HashNode**)a;
    HashNode* hB = *(HashNode**)b;

    if (hA == NULL && hB == NULL) return 0;
    if (hA == NULL) return 1;
    if (hB == NULL) return -1;
  
    return (hB->freq - hA->freq);
}

int get_hash_idx(int key, int length) {
    int idx = key % length;
    if (idx < 0) idx += length;
    return idx;
}

int* topKFrequent(int* nums, int numsSize, int k, int* returnSize) {
    *returnSize = k;

    int hash_table_size = numsSize;
    int* result = (int*)calloc(k, sizeof(int));
    HashNode** ht = (HashNode**)calloc(hash_table_size + 1, sizeof(HashNode*));

    for (int idx = 0; idx < numsSize; idx++) {
        int num = nums[idx];
        int hash_idx = get_hash_idx(num, hash_table_size);

        HashNode* hashNode = ht[hash_idx];
        if (hashNode == NULL) {
            HashNode* hNode = (HashNode*)calloc(1, sizeof(HashNode));
            hNode -> key = num;
            hNode -> freq = 1;
            ht[hash_idx] = hNode;
        } else {
            if (ht[hash_idx] -> key != num) {
                while(hash_idx < hash_table_size && ht[hash_idx] != NULL && ht[hash_idx] -> key != num) {
                    hash_idx++;
                }

                if (ht[hash_idx] != NULL && ht[hash_idx] -> key == num) {
                    ht[hash_idx] -> freq++;
                } else {
                    if (hash_idx >= hash_table_size) {
                        hash_table_size *= 2;
                        ht = (HashNode**)realloc(ht, sizeof(HashNode*) * (hash_table_size));
                    }

                    HashNode* hNode = (HashNode*)calloc(1, sizeof(HashNode));
                    hNode -> key = num;
                    hNode -> freq = 1;
                    ht[hash_idx] = hNode;
                }
                 
                
            } else {
                ht[hash_idx] -> freq++;
            }
        }
    }

    qsort(ht, numsSize + 1, sizeof(HashNode*), compare);

    int i = 0;
    while(k > 0) {
        HashNode* hashNode = ht[i];
        result[i] = hashNode -> key;
        i++;
        k--;
    }

    free(ht);
    return result;
}

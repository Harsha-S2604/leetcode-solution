/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

typedef struct AnagramsNode {
    unsigned long size;
    unsigned long key;
    char** values;
    struct AnagramsNode* next;
} AnagramsNode;

typedef struct HashNode {
    unsigned long hashKey;
    unsigned long hashIdx;
} HashNode;

// int string_compare(const void* a, const void* b) {
//     return (*(char*)a - *(char*)b);
// }

HashNode getHashNode(char* key, int size) {
    HashNode hashNode = {.hashKey = -1, .hashIdx = -1};
    unsigned long hash = 0;
    while (*key) {
        hash += (unsigned long)(*key);
        key++;
    }

    hashNode.hashKey = hash;
    hashNode.hashIdx = hash % size;

    return hashNode;
}

HashNode getHashNodeV2(char* key, int size) {
    HashNode hashNode = {.hashKey = -1, .hashIdx = -1};
    int freq[26] = {0};
    unsigned long hash = 5381;

    for (int i = 0; key[i] != '\0'; i++) {
        if (key[i] >= 'a' && key[i] <= 'z') {
            freq[key[i] - 'a']++;
        }
    }

    for (int i = 0; i < 26; i++) {
        hash = ((hash << 5) + hash) + freq[i];  // Hash * 33 + frequency
    }

    hashNode.hashKey = hash;
    hashNode.hashIdx = hash % size;

    return hashNode;
}

char*** groupAnagrams(char** strs, int strsSize, int* returnSize, int** returnColumnSizes) {
    AnagramsNode** htAnagrams = (AnagramsNode**)calloc(strsSize, sizeof(AnagramsNode*));
    *returnSize = 0;

    for (int idx = 0; idx < strsSize; idx++) {
        char* str = strs[idx];
        HashNode hashNode = getHashNodeV2(str, strsSize);
        AnagramsNode* htAnagramNode = htAnagrams[hashNode.hashIdx];

        if (htAnagrams[hashNode.hashIdx] == NULL) {
            AnagramsNode* anagramNode = (AnagramsNode*)malloc(sizeof(AnagramsNode));
            anagramNode -> size = 1;
            anagramNode -> key = hashNode.hashKey;
            anagramNode -> values = (char**)malloc(sizeof(char*));
            anagramNode -> values[0] = (char*)malloc(strlen(str) * sizeof(char));
            anagramNode -> values[0] = str;
            anagramNode -> next = NULL;

            htAnagrams[hashNode.hashIdx] = anagramNode;
            *returnSize += 1;
        } else {
            while (htAnagramNode != NULL) {
                if (htAnagramNode->key == hashNode.hashKey) {
                    htAnagramNode->size++;
                    htAnagramNode->values = (char**)realloc(htAnagramNode->values, htAnagramNode->size * sizeof(char*));
                    htAnagramNode->values[htAnagramNode->size - 1] = (char*)malloc(strlen(str) * sizeof(char));
                    htAnagramNode->values[htAnagramNode->size - 1] = str;
                    break;
                }
                if (htAnagramNode->next == NULL) {
                    AnagramsNode* anagramNode = (AnagramsNode*)malloc(sizeof(AnagramsNode));
                    anagramNode->size = 1;
                    anagramNode->key = hashNode.hashKey;
                    anagramNode->values = (char**)malloc(sizeof(char*));
                    anagramNode->values[0] = str;
                    anagramNode->next = NULL;
                    htAnagramNode->next = anagramNode;
                    *returnSize += 1;
                    break;
                }
                htAnagramNode = htAnagramNode->next;
            }
        }

    }

    char*** results = (char***)malloc((*returnSize) * sizeof(char**));
    *returnColumnSizes = (int*)malloc((*returnSize) * sizeof(int));
    int resultIdx = -1;
    for (int idx = 0; idx < strsSize; idx++) {
        AnagramsNode* htAnagram = htAnagrams[idx];
        while (htAnagram != NULL) {
            resultIdx++;
            results[resultIdx] = (char**)malloc((htAnagram -> size) * sizeof(char*));
            results[resultIdx] = htAnagram -> values;

            (*returnColumnSizes)[resultIdx] = htAnagram -> size;

            htAnagram = htAnagram -> next;
        }
    }

    free(htAnagrams);
    return results;
}

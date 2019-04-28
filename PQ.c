// PQ ADT interface for Ass2 (COMP2521)
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


struct PQRep {
    ItemPQ **item;
    int      size;
};


PQ newPQ() {
    PQ new = malloc(sizeof(struct PQRep));
    assert(new != NULL);

    new->item = malloc(sizeof(ItemPQ*));
    assert(new->item != NULL);

    new->item[0] = malloc(sizeof(ItemPQ));
    assert(new->item[0] != NULL);

    new->item[0]->key = new->item[0]->value = -1;
    new->size = 0;

    return new;
}

int PQEmpty(PQ p) {
    return (!p->size);
}

void addPQ(PQ pq, ItemPQ element) {
    if (pq->size == 0) {
        pq->item[0]->key = element.key;
        pq->item[0]->value = element.value;
        pq->size++;
    }
    else {
        int exist = 0;
        // we want to find if the value has existed
        for (int i = 0; i < pq->size; i++) {
            if (element.key == i) {
                pq->item[i]->value = element.value;
                exist = 1;
                break;
            }
        }
        // if the key does not exist, we want to alloc new memories for the new "ItemPQ"
        if (!exist) {
            pq->item = realloc(pq->item, (++pq->size) * sizeof(*pq->item));
            assert(pq->item != NULL);
            pq->item[pq->size - 1] = malloc(sizeof(ItemPQ));
            assert(pq->item[pq->size - 1] != NULL);
            pq->item[pq->size - 1]->key = element.key;
            pq->item[pq->size - 1]->value = element.value;
        }
    }
}

ItemPQ dequeuePQ(PQ pq) {
    ItemPQ *throwAway = malloc(sizeof(ItemPQ));
    assert(throwAway != NULL);
    int minValue = pq->item[0]->value;
    int minAt = 0;
    int i = 0;
    for (; i < pq->size; i++) {
        if (pq->item[i]->value < minValue) {
            minValue = pq->item[i]->value;
            minAt = i;
        }
    }
    throwAway->key = pq->item[minAt]->key;
    throwAway->value = minValue;
    // move the rest ItemPQs from current index forward one index, then free the last index
    for (i = minAt ; i < pq->size - 1; i++) {
        pq->item[i]->key = pq->item[i + 1]->key;
        pq->item[i]->value = pq->item[i + 1]->value;
    }
    pq->size--;
    free(pq->item[pq->size]);
    return *throwAway;
}

void updatePQ(PQ pq, ItemPQ element) {
    for (int i = 0; i < pq->size; i++) {
        if (pq->item[i]->key == element.key) {
            pq->item[i]->value = element.value;
            break;
        }
    }
}

void showPQ(PQ pq) {
    printf("\033[32m  Key \033[33mValue\n\033[0m");
    for (int i = 0; i < pq->size; i++) {
        printf("\033[32m%5d \033[33m%-5d\n\033[0m",
            pq->item[i]->key, pq->item[i]->value);
    }
}

void freePQ(PQ pq) {
    int i = 0;
    while(i < pq->size) {
        free(pq->item[i]);
        i++;
    }
    free(pq->item);
    free(pq);
}

/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by
  COMP2521 2019 T1
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include "LanceWilliamsHAC.h"
#include "Graph.h"


/*
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs)
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods.

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 *
 */
static void setDist(Graph g, float **dist);

static void modify_da(Dendrogram *da, int ci, int cj, int N);
static void modify_newdist(float *newdist, int ci, int cj, int N, int method, float **dist);
static void modify_dist(float **dist, int ci, int cj, int N, float *newdist);

Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int N = numVerticies(g);
    // Dendrogram Array
    Dendrogram *da = calloc(N, sizeof(Dendrogram));
    assert(da != NULL);
    float **dist = calloc(N, sizeof(float*) * N);
    for (int i = 0; i < N; i++) {
        dist[i] = malloc(sizeof(float) * N);
        da[i] = malloc(sizeof(Dendrogram));
        da[i]->vertex = i;
        da[i]->left = NULL;
        da[i]->right = NULL;
    }
    setDist(g, dist);

    for (; N > 1; N--) {
        // find two closest cluster, ci and cj
        int ci = 0;
        int cj = 0;
        float mindist = INFINITY;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < i; j++) {
                if (dist[i][j] <= mindist) {
                    mindist = dist[i][j];
                    ci = i; cj = j;
                }
            }
        }

        float *newdist = malloc(N * sizeof(float));
        modify_newdist(newdist, ci, cj, N, method, dist);
        modify_dist(dist, ci, cj, N, newdist);
        modify_da(da, ci, cj, N);
    }
    return da[0];
}


void freeDendrogram(Dendrogram d) {
    if (d != NULL) return;
    free(d->left);
    free(d->right);
    free(d);
}

// three steps to set the start **dist with a given graph
static void setDist(Graph g, float **dist) {
    int N = numVerticies(g);

    // step 1 : set all distances to infinity
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = INFINITY;
        }
    }

    // step 2 : if there is a link between two vertexes,
    //          set the distance to the reciprocal of the distance
    for (int i = 0; i < N; i++) {
        for (AdjList out = outIncident(g, i); out != NULL; out = out->next) {
            dist[i][out->w] = dist[out->w][i] = pow(out->weight, -1.0);

            // step 3 : if there are two links between two vertexes,
            //          set the distance to the reciprocal of the
            //          maximum value of two distances
            for (AdjList in = inIncident(g, i); in != NULL; in = in->next) {
                if (out->w == in->w) {
                    float tmp = fmaxf(out->weight, in->weight);
                    dist[i][out->w] = dist[out->w][i] = pow(tmp, -1.0);
                }
            }
        }
    }
}

// remove ci and cj, and add cij
static void modify_da(Dendrogram *da, int ci, int cj, int N) {
    Dendrogram dt = malloc(sizeof(Dendrogram));
    assert(dt != NULL);
    dt->left = da[cj];
    dt->right = da[ci];

    // delete da @ index ci
    for (int i = ci; i < N - 1; i++) {
        da[i] = da[i + 1];
    }

    // delete da @ index cj
    for (int i = cj; i < N - 2; i++) {
        da[i] = da[i + 1];
    }

    da[N - 2] = dt;
}

// set *newdist with **dist and given method, then delete at index ci and cj
static void modify_newdist(float *newdist, int ci, int cj, int N, int method, float **dist) {
    for (int n = 0; n < N; n++) {
        if (method == 1)
        newdist[n] = fminf(dist[n][ci], dist[n][cj]);
        if (method == 2)
        newdist[n] = fmaxf(dist[n][ci], dist[n][cj]);
    }

    // delete newdist @ index ci
    for (int i = ci; i < N - 1; i++) {
        newdist[i] = newdist[i + 1];
    }

    // delete newdist @ index cj
    for (int i = cj; i < N - 2; i++) {
        newdist[i] = newdist[i + 1];
    }
}

// delete row and column at index ci and cj,
// and add newdist at the last row and column
static void modify_dist(float **dist, int ci, int cj, int N, float *newdist) {
    // delete dist @ column index ci
    for (int i = 0; i < N; i++) {
        for (int j = ci; j < N - 1; j++) {
            dist[i][j] = dist[i][j + 1];
        }
    }

    // delete dist @ column index cj
    for (int i = 0; i < N; i++) {
        for (int j = cj; j < N - 2; j++) {
            dist[i][j] = dist[i][j + 1];
        }
    }

    // delete dist @ row index ci
    for (int i = ci; i < N - 1; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = dist[i + 1][j];
        }
    }

    // delete dist @ row index cj
    for (int i = cj; i < N - 2; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = dist[i + 1][j];
        }
    }

    // add new distance to the column index (N-2) and row index (N-2)
    int k = N - 2;
    for (int i = 0; i < N - 1; i++) {
        dist[k][i] = dist[i][k] = newdist[i];
    }
    dist[k][k] = INFINITY;
}

// multithread_sort.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10

// the original list
int arr[N] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
// the array where the merged, fully sorted result goes
int sorted[N];

// midpoint
const int mid = N / 2;

// parameters for each sorting thread
typedef struct {
    int start;  // starting index in arr[]
    int len;    // length of the subarray
} SortParams;

// simple insertion sort on arr[start .. start+len-1]
void* sort_half(void* args) {
    SortParams* p = (SortParams*)args;
    int s = p->start;
    int e = s + p->len;
    for (int i = s + 1; i < e; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= s && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    pthread_exit(NULL);
}

// merge the two sorted halves into sorted[]
void* merge_halves(void* args) {
    int i = 0, j = mid, k = 0;
    // merge until one half is exhausted
    while (i < mid && j < N) {
        if (arr[i] <= arr[j])
            sorted[k++] = arr[i++];
        else
            sorted[k++] = arr[j++];
    }
    // copy any remaining elements
    while (i < mid)
        sorted[k++] = arr[i++];
    while (j < N)
        sorted[k++] = arr[j++];
    pthread_exit(NULL);
}

int main() {
    pthread_t sort_tid[2], merge_tid;
    SortParams params[2];

    // prepare parameters for the two sorting threads
    params[0].start = 0;
    params[0].len   = mid;       // first half: indices 0..mid-1
    params[1].start = mid;
    params[1].len   = N - mid;   // second half: indices mid..N-1

    // 1) spawn the two sort threads
    pthread_create(&sort_tid[0], NULL, sort_half,   &params[0]);
    pthread_create(&sort_tid[1], NULL, sort_half,   &params[1]);

    // 2) wait for both sort threads to finish
    pthread_join(sort_tid[0], NULL);
    pthread_join(sort_tid[1], NULL);

    // 3) spawn the merge thread
    pthread_create(&merge_tid, NULL, merge_halves, NULL);

    // 4) wait for merge to finish
    pthread_join(merge_tid, NULL);

    // 5) print the fully sorted list
    printf("Final sorted list:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", sorted[i]);
    }
    printf("\n");

    return 0;
}

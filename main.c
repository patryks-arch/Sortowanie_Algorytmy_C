#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --- ALGORYTMY SORTOWANIA BEZ PORÓWNYWANIA ---

// 1. Counting Sort
void counting_sort(int* arr, int n) {
    if (n <= 0) return;
    int max = arr[0];
    for (int i = 1; i < n; i++) if (arr[i] > max) max = arr[i];
    int* count = (int*)calloc(max + 1, sizeof(int));
    for (int i = 0; i < n; i++) count[arr[i]]++;
    int k = 0;
    for (int i = 0; i <= max; i++) {
        while (count[i] > 0) {
            arr[k++] = i;
            count[i]--;
        }
    }
    free(count);
}

// 2. Radix Sort
void count_for_radix(int* arr, int n, int exp) {
    int* output = (int*)malloc(n * sizeof(int));
    int count[10] = { 0 };
    for (int i = 0; i < n; i++) count[(arr[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
    free(output);
}
void radix_sort(int* arr, int n) {
    if (n <= 0) return;
    int max = arr[0];
    for (int i = 1; i < n; i++) if (arr[i] > max) max = arr[i];
    for (int exp = 1; max / exp > 0; exp *= 10) count_for_radix(arr, n, exp);
}

// 3. Pigeonhole Sort
void pigeonhole_sort(int* arr, int n) {
    if (n <= 0) return;
    int min = arr[0], max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }
    int range = max - min + 1;
    int* holes = (int*)calloc(range, sizeof(int));
    for (int i = 0; i < n; i++) holes[arr[i] - min]++;
    int k = 0;
    for (int i = 0; i < range; i++) {
        while (holes[i]-- > 0) arr[k++] = i + min;
    }
    free(holes);
}

// 4. Bucket Sort
void bucket_sort(int* arr, int n) {
    if (n <= 0) return;
    int max = arr[0];
    for (int i = 1; i < n; i++) if (arr[i] > max) max = arr[i];
    int num_buckets = 100; // liczba kubełków
    int* buckets = (int*)calloc(max + 1, sizeof(int));
    for (int i = 0; i < n; i++) buckets[arr[i]]++;
    int k = 0;
    for (int i = 0; i <= max; i++) {
        while (buckets[i]-- > 0) arr[k++] = i;
    }
    free(buckets);
}

// Funkcja do mierzenia czasu i testów
void testuj_wydajnosc(int n) {
    int* tab = (int*)malloc(n * sizeof(int));
    int* kopia = (int*)malloc(n * sizeof(int));
    clock_t start, end;

    for (int i = 0; i < n; i++) tab[i] = rand() % 100000;

    printf("\nTest dla N = %d:\n", n);

    // 1. Test Counting
    memcpy(kopia, tab, n * sizeof(int));
    start = clock();
    counting_sort(kopia, n);
    end = clock();
    printf("- Counting Sort:   %.4f s\n", (double)(end - start) / CLOCKS_PER_SEC);

    // 2. Test Radix
    memcpy(kopia, tab, n * sizeof(int));
    start = clock();
    radix_sort(kopia, n);
    end = clock();
    printf("- Radix Sort:      %.4f s\n", (double)(end - start) / CLOCKS_PER_SEC);

    // 3. Test Pigeonhole
    memcpy(kopia, tab, n * sizeof(int));
    start = clock();
    pigeonhole_sort(kopia, n);
    end = clock();
    printf("- Pigeonhole Sort: %.4f s\n", (double)(end - start) / CLOCKS_PER_SEC);

    // 4. Test Bucket
    memcpy(kopia, tab, n * sizeof(int));
    start = clock();
    bucket_sort(kopia, n);
    end = clock();
    printf("- Bucket Sort:     %.4f s\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(tab);
    free(kopia);
}
int main() {
    srand((unsigned int)time(NULL));
    int opcja;

    printf("=== PROJEKT 13: ALGORYTMY SORTOWANIA BEZ POROWNYWAN ===\n");
    printf("1. Test wydajnosci (tablice 10 - 1 000 000)\n");
    printf("2. Wyjscie\n");
    printf("Wybor: ");
    scanf_s("%d", &opcja);

    if (opcja == 1) {
        int wielkosci[] = { 100, 1000, 10000, 100000, 1000000 };
        for (int i = 0; i < 5; i++) {
            testuj_wydajnosc(wielkosci[i]);
        }
    }

    printf("\nKoniec. Nacisnij Enter...");
    getchar(); getchar();
    return 0;
}
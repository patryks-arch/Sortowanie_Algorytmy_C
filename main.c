#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --- ALGORYTMY SORTOWANIA BEZ PORÓWNYWANIA ---

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

void bucket_sort(int* arr, int n) {
    if (n <= 0) return;
    int max = arr[0];
    for (int i = 1; i < n; i++) if (arr[i] > max) max = arr[i];
    int* buckets = (int*)calloc(max + 1, sizeof(int));
    for (int i = 0; i < n; i++) buckets[arr[i]]++;
    int k = 0;
    for (int i = 0; i <= max; i++) {
        while (buckets[i]-- > 0) arr[k++] = i;
    }
    free(buckets);
}

// --- POMIARY I OBSŁUGA PLIKÓW ---

void testuj_wydajnosc(int n, FILE* plik_raportu) {
    int* tab = (int*)malloc(n * sizeof(int));
    int* kopia = (int*)malloc(n * sizeof(int));
    clock_t start, end;

    for (int i = 0; i < n; i++) tab[i] = rand() % 100000;

    printf("\nTest dla N = %d:\n", n);
    fprintf(plik_raportu, "Test dla N = %d:\n", n);

    // 1. Counting Sort
    memcpy(kopia, tab, n * sizeof(int));
    start = clock(); counting_sort(kopia, n); end = clock();
    double t1 = (double)(end - start) / CLOCKS_PER_SEC;
    printf("- Counting Sort:   %.4f s\n", t1);
    fprintf(plik_raportu, "- Counting Sort:   %.4f s\n", t1);

    // 2. Radix Sort
    memcpy(kopia, tab, n * sizeof(int));
    start = clock(); radix_sort(kopia, n); end = clock();
    double t2 = (double)(end - start) / CLOCKS_PER_SEC;
    printf("- Radix Sort:      %.4f s\n", t2);
    fprintf(plik_raportu, "- Radix Sort:      %.4f s\n", t2);

    // 3. Pigeonhole Sort
    memcpy(kopia, tab, n * sizeof(int));
    start = clock(); pigeonhole_sort(kopia, n); end = clock();
    double t3 = (double)(end - start) / CLOCKS_PER_SEC;
    printf("- Pigeonhole Sort: %.4f s\n", t3);
    fprintf(plik_raportu, "- Pigeonhole Sort: %.4f s\n", t3);

    // 4. Bucket Sort
    memcpy(kopia, tab, n * sizeof(int));
    start = clock(); bucket_sort(kopia, n); end = clock();
    double t4 = (double)(end - start) / CLOCKS_PER_SEC;
    printf("- Bucket Sort:     %.4f s\n", t4);
    fprintf(plik_raportu, "- Bucket Sort:     %.4f s\n", t4);

    fprintf(plik_raportu, "\n");
    free(tab);
    free(kopia);
}

int main() {
    srand((unsigned int)time(NULL));
    int opcja;

    while (1) {
        printf("\n=== PROJEKT 13: MENU ===\n");
        printf("1. Wczytaj tablice z pliku 'dane.txt' i posortuj\n");
        printf("2. Generuj losowe tablice (100 - 1 000 000) i zapisz raport\n");
        printf("3. Wyjscie\n");
        printf("Wybor: ");
        if (scanf("%d", &opcja) != 1) break;

        if (opcja == 1) {
            FILE* f_in = fopen("dane.txt", "r");
            if (f_in == NULL) {
                printf("Blad: Brak pliku 'dane.txt' w folderze projektu!\n");
                continue;
            }
            int temp, licznik = 0;
            while (fscanf(f_in, "%d", &temp) == 1) licznik++;

            if (licznik == 0) {
                printf("Plik 'dane.txt' jest pusty!\n");
                fclose(f_in);
                continue;
            }

            int* tab_plik = (int*)malloc(licznik * sizeof(int));
            rewind(f_in);
            for (int i = 0; i < licznik; i++) fscanf(f_in, "%d", &tab_plik[i]);
            fclose(f_in);

            printf("Wczytano %d elementow. Sortowanie...\n", licznik);
            counting_sort(tab_plik, licznik);

            FILE* f_out = fopen("wynik_sortowania.txt", "w");
            if (f_out != NULL) {
                for (int i = 0; i < licznik; i++) fprintf(f_out, "%d ", tab_plik[i]);
                fclose(f_out);
                printf("Sukces: Wynik zapisano do 'wynik_sortowania.txt'!\n");
            }
            free(tab_plik);
        }
        else if (opcja == 2) {
            FILE* plik_raportu = fopen("wyniki_czasowe.txt", "w");
            if (plik_raportu == NULL) {
                printf("Blad zapisu pliku raportu!\n");
                continue;
            }
            fprintf(plik_raportu, "RAPORT POMIARU CZASU\n====================\n\n");

            int wielkosci[] = { 100, 1000, 10000, 100000, 1000000 };
            for (int i = 0; i < 5; i++) {
                testuj_wydajnosc(wielkosci[i], plik_raportu);
            }

            fclose(plik_raportu);
            printf("\nSukces: Raport czasowy zapisano do 'wyniki_czasowe.txt'!\n");
        }
        else if (opcja == 3) {
            break;
        }
    }
    return 0;
}
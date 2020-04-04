#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char data[4][21];
} Human;

typedef struct {
    int pos;
    int sizeMas;
    Human *humans;
} Humans;

int push(Human human, Humans *humans) {
    if (humans->pos >= humans->sizeMas) {
        Human* temp = (Human *) realloc(humans->humans, 2 * humans->sizeMas * sizeof(Human));
        free(humans->humans);
        if (temp == NULL) {
            printf("There was no unexpected error, please try again");
            return 0;
        }
        humans->humans = temp;
        humans->sizeMas *= 2;
    }
    humans->humans[humans->pos] = human;
    humans->pos++;
    return 1;
}

// first < second!!!
int comparisonForHuman(Human first, Human second, int k) {
    int a = strcmp(first.data[k], second.data[k]);
    if (a == 0) {
        if (k == 3)
            return 0;
        return comparisonForHuman(first, second, k + 1);
    } else {
        return a;
    }
}

void QuickSort(int size, Human *humans) {
    if (size <= 1) return;
    Human x = humans[(size - 1) / 2];
    int i = 0, j = size - 1;
    while (i <= j) {
        while (comparisonForHuman(humans[i], x, 0) < 0) i++;
        while (comparisonForHuman(humans[j], x, 0) > 0) j--;
        if (i <= j) {
            Human t = humans[i];
            humans[i] = humans[j];
            humans[j] = t;
            i++;
            j--;
        }
    }
    if (j > 0) QuickSort(j + 1, humans);
    if (size - 1 > i) QuickSort(size - j - 1, &humans[i]);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("There was no unexpected error, please try again");
        return 0;
    }
    Humans humans = {0, 10};
    humans.humans = (Human *) malloc(humans.sizeMas * sizeof(Human));
    if (humans.humans == NULL) {
        printf("There was no unexpected error, please try again");
        return 0;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Not found input file exception");
        fclose(f);
        return 0;
    }
    char surname[21], name[21], middleName[21], num[21];
    while ((fscanf(f, "%s%s%s%s", &surname, &name, &middleName, &num) != EOF)) {
        Human temp;
        strcpy(temp.data[0], surname);
        strcpy(temp.data[1], name);
        strcpy(temp.data[2], middleName);
        strcpy(temp.data[3], num);
        if (!push(temp, &humans)) {
            free(humans.humans);
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        printf("Not found output file exception");
        fclose(out);
    }
    QuickSort(humans.pos,  humans.humans);
    for (int i = 0; i < humans.pos; i++) {
        fprintf(out, "%s %s %s %s \n", humans.humans[i].data[0], humans.humans[i].data[1], humans.humans[i].data[2],
                humans.humans[i].data[3]);
    }
    free(humans.humans);
    fclose(out);
    return 0;
}


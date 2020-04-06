#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char data[4][21];
} Human;

typedef struct {
    long pos;
    long sizeMas;
    Human *humans;
} Humans;

typedef struct {
    long pos;
    long sizeMas;
    long* stack;
} Stack;

int push(Human human, Humans *humans) {
    if (humans->pos >= humans->sizeMas) {
        Human* temp = (Human *) realloc(humans->humans, 2 * humans->sizeMas * sizeof(Human));
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

void pushForStack(long element, Stack* stack) {
    if (stack->pos >= stack->sizeMas) {
        long* temp = realloc(stack->stack, 2 * stack->sizeMas * sizeof(long));
        if (temp == NULL) {
            printf("There was no unexpected error, please try again");
        }
        stack->stack = (long *) temp;
        stack->sizeMas *= 2;
    }
    stack->stack[stack->pos] = element;
    stack->pos++;
}

long pop(Stack* stack) {
    return stack->stack[--stack->pos];
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

void QuickSort(long size, Human *humans) {
    long lb, ub, stackPos = 1;
    Stack leftStack = {0, 10};
    Stack rigthStack = {0, 10};
    leftStack.stack = malloc(leftStack.sizeMas * sizeof(long));
    rigthStack.stack = malloc(rigthStack.sizeMas * sizeof(long));
    pushForStack(0, &leftStack);
    pushForStack(size - 1, &rigthStack);
    do {
        lb = pop(&leftStack);
        ub = pop(&rigthStack);
        do {
            Human x = humans[(ub + lb) / 2];
            int i = lb, j = ub;
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
            if (i < (ub + lb) / 2) {
                if (i < ub) {
                    pushForStack(i, &leftStack);
                    pushForStack(ub, &rigthStack);
                }
                ub = j;
            } else  {
                if (j > lb) {
                    pushForStack(lb, &leftStack);
                    pushForStack(j, &rigthStack);
                }
                lb = i;
            }
        } while(lb < ub);
    } while (leftStack.pos != 0);
}

int main(int argc, char **argv) {
    if (argc != 3) {
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
    if (humans.pos != 0) {
        QuickSort(humans.pos, humans.humans);
        for (int i = 0; i < humans.pos; i++) {
            fprintf(out, "%s %s %s %s\n", humans.humans[i].data[0], humans.humans[i].data[1], humans.humans[i].data[2],
                    humans.humans[i].data[3]);
        }
    }
    free(humans.humans);
    fclose(out);
    return 0;
}

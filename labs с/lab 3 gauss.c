#include <stdio.h>
#include <stdlib.h>
#define eps 1e-15

void getMatrix(double **linSystem, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size + 1; j++) {
            printf("%lf ", linSystem[i][j]);
        }
        printf("\n");
    }
}

double absolute(double a) {
    if (a < 0) return -a;
    else return a;
}

_Bool pivoting(double **linSystem, size_t row, size_t col, int size) {
    size_t maxLineIndex = row;
    double localMax = 0;
    for (size_t i = row; i < size; i++) {
        if (absolute(linSystem[i][col]) > localMax) {
            maxLineIndex = i;
            localMax = linSystem[i][col];
        }
    }
    if (!localMax)
        return 0;
    double *temp = linSystem[row];
    linSystem[row] = linSystem[maxLineIndex];
    linSystem[maxLineIndex] = temp;
    return 1;
}

void madeTriangularView(double **linSystem, size_t row, size_t col, int size) {
    if (row < size && col < size && !linSystem[row][col])
        return;
    for (size_t i = row + 1; i < size; i++) {
        double delta = linSystem[i][col] / linSystem[row][col];
        for (size_t j = col; j < size + 1; j++)
            linSystem[i][j] -= delta * linSystem[row][j];
    }
}

_Bool checkIncompatibleLine(double **linSystem, size_t row, int size) {
    for (int i = 0; i < size; i++) {
        if (absolute(linSystem[row][i]) > eps)
            return 0;
    }
    if (absolute(linSystem[row][size]) <= eps)
        return 0;
    else return 1;
}

_Bool checkManySolution(double **linSystem, size_t row, int size) {
    for (int i = 0; i < size; i++) {
        if (absolute(linSystem[row][i]) > eps)
            return 0;
    }
    return 1;
}

void madeDiagView(double **linSystem, int col, int size) {
    for (int i = col - 1; i >= 0; i--) {
        double delta = linSystem[i][col] / linSystem[col][col];
        linSystem[i][col] -= delta * linSystem[col][col];
        linSystem[i][size] -= delta * linSystem[col][size];
    }
}

int gauss(double **linSystem, double *ans, int size) {
    for (size_t col = 0, row = 0; col < size && row < size; row++) {
        col = row;
        while (!pivoting(linSystem, row, col, size) && col < size)
            col++;
        if (col < size)
            madeTriangularView(linSystem, row, col, size);
    }
    for (size_t row = 0; row < size; row++) {
        if (checkIncompatibleLine(linSystem, row, size))
            return 0;
    }
    for (size_t row = 0; row < size; row++) {
        if (checkManySolution(linSystem, row, size))
            return 1;
    }
    for (int row = size - 1; row > 0; row--) {
        madeDiagView(linSystem, row, size);
    }
    for (int i = 0; i < size; i++) {
        ans[i] = linSystem[i][size] / linSystem[i][i];
    }
    return 2;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("There was no unexpected error, please try again");
        return 0;
    }
    FILE *in = fopen(argv[1], "r");
    if (!in) {
        printf("Not found input file exception");
        fclose(in);
        return 0;
    }
    int n;
    fscanf(in, "%i", &n);
    double *ans = malloc(sizeof(double) * n);
    double **linearSystem = malloc(sizeof(double *) * n);
    for (int i = 0; i < n; i++)
        linearSystem[i] = malloc(sizeof(double) * (n + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            fscanf(in, "%lf", &linearSystem[i][j]);
        }
    }
    fclose(in);
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        printf("Not found output file exception");
        fclose(out);
    }
    int res = gauss(linearSystem, ans, n);
    if (res == 0) {
        fprintf(out, "no solution");
    }
    if (res == 1) {
        fprintf(out, "many solutions");
    }
    if (res == 2) {
        for (int i = 0; i < n; i++) {
            fprintf(out, "%lf\n", ans[i]);
        }
    }
    for (int i = 0; i < n; i++) {
        free(linearSystem[i]);
    }
    free(linearSystem);
    free(ans);
    fclose(out);
    return 0;
}
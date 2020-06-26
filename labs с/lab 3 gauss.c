#include <stdio.h>
#include <stdlib.h>

double eps = 1e-14;

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

_Bool pivoting(double **linSystem, size_t col, size_t row, int size) {
    size_t maxLineIndex = col;
    double localMax = 0;
    for (size_t i = col; i < size; i++) {
        if (linSystem[i][row] > localMax) {
            maxLineIndex = i;
            localMax = linSystem[i][row];
        }
    }
    if (!localMax)
        return 0;
    double *temp = linSystem[col];
    linSystem[col] = linSystem[maxLineIndex];
    linSystem[maxLineIndex] = temp;
    return 1;
}

void madeTriangularView(double **linSystem, size_t col, size_t row, int size) {
    if (row < size && col < size && !linSystem[col][row])
        return;
    for (size_t i = col + 1; i < size; i++) {
        double delta = linSystem[i][row] / linSystem[col][row];
        for (size_t j = row; j < size + 1; j++)
            linSystem[i][j] -= delta * linSystem[col][j];
    }
}

_Bool checkIncompatibleLine(double **linSystem, size_t col, int size) {
    for (int i = 0; i < size; i++) {
        if (absolute(linSystem[col][i]) > eps)
            return 0;
    }
    if (absolute(linSystem[col][size]) <= eps)
        return 0;
    else return 1;
}

_Bool checkManySolution(double **linSystem, size_t col, int size) {
    for (int i = 0; i < size; i++) {
        if (absolute(linSystem[col][i]) > eps)
            return 0;
    }
    return 1;
}

void madeDiagView(double **linSystem, size_t col, int size) {
    for (int i = col - 1; i >= 0; i--) {
        double delta = linSystem[i][col] / linSystem[col][col];
        linSystem[i][col] -= delta * linSystem[col][col];
        linSystem[i][size] -= delta * linSystem[col][size];
    }
}

int gauss(double **linSystem, double *ans, int size) {
    for (size_t col = 0, row = 0; col < size && row < size; col++) {
        row = col;
        while (!pivoting(linSystem, col, row, size) && row < size)
            row++;
        madeTriangularView(linSystem, col, row, size);
    }
    for (size_t col = 0; col < size; col++) {
        if (checkIncompatibleLine(linSystem, col, size))
            return 0;
    }
    for (size_t col = 0; col < size; col++) {
        if (checkManySolution(linSystem, col, size))
            return 1;
    }
    for (size_t col = size - 1; col > 0; col--) {
        madeDiagView(linSystem, col, size);
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

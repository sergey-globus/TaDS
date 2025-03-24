/*!
 * Лабораторная работа 3, вариант 6
 * 1. Смоделировать операцию умножения вектора-строки хранящегося в форме
 * вектора A и вектора, содержащего номера столбцов этих элементов, и матрицы,
 * хранящейся в указанной форме, с получением результата в форме хранения
 * вектора-строки.
 * 2. Произвести операцию умножения, применяя стандартный алгоритм работы с
 * матрицами.3. Сравнить время выполнения операций и объем памяти при использовании
 * этих 2-х алгоритмов при различном проценте заполнения матриц.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MATRIX_FILE "mtr_1.txt"
#define N 10

#define SUCCESS 0
#define FILE_ERROR 1
#define INPUT_ERROR 2
#define DATA_ERROR 3
#define RANGE_ERROR 4
#define MULTY_ERROR 5

int finput(float **A, int **IA, int **JA, int *n, int *m, int *nz)
{
    float mtr[N][N];
    *nz = 0;

    /// Открываем файл
    FILE *f = fopen(MATRIX_FILE, "r");
    if (f == NULL)
        return FILE_ERROR;

    /// Проверка параметров матрицы
    if (fscanf(f, "%d%d", n, m) != 2)
        return INPUT_ERROR;

    /// Сохраняем матрицу в привычном формате
    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *m; j++)
            if (fscanf(f, "%f", &mtr[i][j]) != 1)
                return INPUT_ERROR;

    /// Считаем количество ненулевых элементов
    *nz = 0;
    for (int i = 0; i < *m; i++)
        for (int j = 0; j < *n; j++)
            if (mtr[j][i] != 0)
                (*nz)++;

    /// Выделяем память для массивов A, IA
    *A = malloc(sizeof(float) * (*nz));
    if (*A == NULL)
        return DATA_ERROR;
    *IA = malloc(sizeof(int) * (*nz));
    if (*IA == NULL)
        return DATA_ERROR;

    /// Выделяем память для массива JA
    *JA = malloc(sizeof(int) * (*m + 1));
    if (*JA == NULL)
        return DATA_ERROR;

    /// Сохраняем матрицу в формате трех обьектов
    (*JA)[0] = 0;
    for (int i = 0, count = 0; i < *m; i++)
    {
        for (int j = 0; j < *n; j++)
            if (mtr[j][i] != 0)
            {
                (*A)[count] = mtr[j][i];
                (*IA)[count] = j;
                count++;
            }
        (*JA)[i + 1] = count;
    }

    /// Закрываем файл
    if (f != NULL)
        fclose(f);

    return SUCCESS;
}

int input(float **A, int **IA, int **JA, int *n, int *m, int *nz)
{
    char read_str[50];
    char *token;
    char *endptr;
    *nz = 0;
    int count;

    printf("!Для корректной работы программы ввод элементов матрицы производится строго по столбцам, начиная с первого!\n");

    printf("\nВведите количество строк матрицы: ");
    if (scanf("%d", n) != 1 || *n <= 0)
        return INPUT_ERROR;
    printf("Введите количество столбцов матрицы: ");
    if (scanf("%d", m) != 1 || *m <= 0)
        return INPUT_ERROR;

    printf("Введите вектор A ненулевых элементов (1 2 3): ");
    scanf(" %[^\n]", read_str);
    /// Заполняем массив A
    token = strtok(read_str, " ");
    do
    {
        /// Выделяем память для элемента
        *A = realloc(*A, sizeof(float) * (*nz + 1));
        if (*A == NULL)
            return DATA_ERROR;
        /// Проверка на число
        (*A)[*nz] = strtod(token, &endptr);
        if (*endptr != '\0')
            return INPUT_ERROR;
        (*nz)++;
    } while((token = strtok(NULL, " ")) != NULL);

    printf("Введите вектор IA, содержащий индексы строк элементов A (0 1 2): ");
    count = 0;
    /// Выделяем память для массива IA
    *IA = malloc(sizeof(int) * (*nz));
    if (*IA == NULL)
        return DATA_ERROR;
    scanf(" %[^\n]", read_str);
    /// Заполняем массив IA
    token = strtok(read_str, " ");
    do
    {
        if (count == *nz)
        {
            printf("Не соответствует количеству элементов в A\n");
            return RANGE_ERROR;
        }
        /// Проверка на целое число
        (*IA)[count] = strtol(token, &endptr, 10);
        if (*endptr != '\0')
            return RANGE_ERROR;
        /// Проверка на индекс
        if ((*IA)[count] < 0 || (*IA)[count] >= *n)
        {
            printf("Индекс строки вышел за пределы [0, %d]\n", *n - 1);
            return RANGE_ERROR;
        }
        count++;
    } while((token = strtok(NULL, " ")) != NULL);
    if (count != *nz)
    {
        printf("Не соответствует количеству элементов в A\n");
        return RANGE_ERROR;
    }

    printf("Введите вектор JA, содержащий индексы из A и IA, с которых начинается описание столбцов матрицы A (0 1 2): ");
    scanf(" %[^\n]", read_str);
    count = 0;
    /// Выделяем память для массива JA
    *JA = malloc(sizeof(int) * (*m + 1));
    if (*JA == NULL)
        return DATA_ERROR;
    /// Заполняем массив JA
    token = strtok(read_str, " ");
    do
    {
        if (count == *m)
        {
            printf("Не соответстует количеству столбцов\n");
            return RANGE_ERROR;
        }
        /// Проверка на целое число >= 0
        (*JA)[count] = strtol(token, &endptr, 10);
        if (*endptr != '\0')
            return INPUT_ERROR;
        /// Проверка на индекс
        if ((*JA)[count] < 0)
            return INPUT_ERROR;
        if ((*JA)[count] > *nz)
        {
            printf("В массиве A не существует %d индекса\n", (*JA)[count]);
            return RANGE_ERROR;
        }
        if (count > 0 && (*JA)[count] - (*JA)[count - 1] > *n)
        {
            printf("В столбце не может быть больше %d элементов\n", *n);
            return RANGE_ERROR;
        }
        if (count > 0 && (*JA)[count] < (*JA)[count - 1])
        {
            printf("Некорректный ввод матрицы\n");
            return RANGE_ERROR;
        }
        count++;
    } while((token = strtok(NULL, " ")) != NULL);
    if ((*JA)[0] != 0)
    {
        printf("Некорректный ввод матрицы\n");
        return RANGE_ERROR;
    }
    if (count != *m)
    {
        printf("Не соответстует количеству столбцов\n");
        return RANGE_ERROR;
    }
    (*JA)[count] = *nz;



    return SUCCESS;
}

int input_row(float **ROW, int **J_ROW, int *n_row, int *nz_row, int n)
{
    char read_str[50];
    char *token;
    char *endptr;
    int count;
    *nz_row = 0;

    printf("\nВведите количество элементов в строке: ");
    if (scanf("%d", n_row) != 1 || *n_row <= 0)
        return INPUT_ERROR;
    if (*n_row != n)
    {
        printf("Не соответствует количеству строк в матрице\n");
        return MULTY_ERROR;
    }

    if (*n_row <= N)
    {
        /// Для ввода самой строки
        float read_float;
        *ROW = malloc(sizeof(float) * (*n_row));
        if (*ROW == NULL)
            return DATA_ERROR;
        *J_ROW = malloc(sizeof(int) * (*n_row));
        if (*J_ROW == NULL)
            return DATA_ERROR;
        for (int i = 0; i < *n_row; i++)
        {
            printf("Введите %d-ый элемент вектор-строки: ", i + 1);
            if (scanf("%f", &read_float) != 1)
                return INPUT_ERROR;
            if (read_float != 0)
            {
                (*ROW)[*nz_row] = read_float;
                (*J_ROW)[*nz_row] = i;
                (*nz_row)++;
            }
        }
    }
    else
    {
        /// Для ввода только ненулевых элементов
        printf("Введите вектор ненулевых элементов (1 2 3): ");
        scanf(" %[^\n]", read_str);
        /// Заполняем массив ROW
        token = strtok(read_str, " ");
        do
        {
            /// Выделяем память для элемента
            *ROW = realloc(*ROW, sizeof(float) * (*nz_row + 1));
            if (*ROW == NULL)
                return DATA_ERROR;
            /// Проверка на число
            (*ROW)[*nz_row] = strtod(token, &endptr);
            if (*endptr != '\0')
                return INPUT_ERROR;
            (*nz_row)++;
        } while((token = strtok(NULL, " ")) != NULL);
        printf("Введите вектор, содержащий индексы строки (0 1 2): ");
        scanf(" %[^\n]", read_str);
        count = 0;
        /// Выделяем память для массива
        *J_ROW = malloc(sizeof(int) * (*nz_row));
        if (*J_ROW == NULL)
            return DATA_ERROR;
        /// Заполняем массив IA
        token = strtok(read_str, " ");
        do
        {
            if (count == *nz_row)
            {
                printf("Не соответствует количеству элементов в строке\n");
                return RANGE_ERROR;
            }
            /// Проверка на целое число
            (*J_ROW)[count] = strtol(token, &endptr, 10);
            if (*endptr != '\0')
                return RANGE_ERROR;
            /// Проверка на индекс
            if ((*J_ROW)[count] < 0 || (*J_ROW)[count] >= *n_row)
            {
                printf("Индекс строки вышел за пределы [0, %d]\n", *n_row - 1);
                return RANGE_ERROR;
            }
            count++;
        } while((token = strtok(NULL, " ")) != NULL);
        if (count != *nz_row)
        {
            printf("Не соответствует количеству элементов в строке\n");
            return RANGE_ERROR;
        }
    }

    return SUCCESS;
}

int special_multy(float *ROW, int *J_ROW, int n, int nz_row, float *A, int *IA, int *JA, int m, float **COL_SOL)
{
    int *IP = NULL;

    /// Заполняем IP строкой
    IP = malloc(sizeof(int) * n);
    if (IP == NULL)
        return DATA_ERROR;
    for (int i = 0; i < n; i++)
        IP[i] = -1;
    for (int i = 0; i < nz_row; i++)
        IP[J_ROW[i]] = i;

    /// Выделяем память для результата умножения строки на матрицу (строки)
    *COL_SOL = calloc(m, sizeof(float));
    if (*COL_SOL == NULL)
        return DATA_ERROR;

    /// Находим матричное умножение для каждого столбца матрицы
    for (int k = 0; k < m; k++)
        for (int i = JA[k]; i < JA[k + 1]; i++)
            if (IP[IA[i]] != -1)
                (*COL_SOL)[k] += (ROW[IP[IA[i]]] * A[i]);

    free(IP);
    return SUCCESS;
}

int standart_multy(float *MATRIX, int n, int m, float *MATRIX_ROW, float **COL_SOL)
{
    *COL_SOL = calloc(m, sizeof(float));
    if (!*COL_SOL)
    {
        printf("Не удалось выделить память под строку решений\n");
        return DATA_ERROR;
    }

    for (int i = 0; i < m; i++)
        for (int k = 0; k < n; k++)
            (*COL_SOL)[i] += (MATRIX_ROW[k] * MATRIX[k * m + i]);

    return SUCCESS;
}

int transfer(float **MATRIX, float *A, int *IA, int *JA, int n, int m, float **MATRIX_ROW, float *ROW, int *J_ROW, int nz_row)
{
    *MATRIX = calloc(m * n, sizeof(float));
    if (!*MATRIX)
    {
        printf("Не удалось выделить память под матрицу\n");
        return DATA_ERROR;
    }
    for (int k = 0; k < m; k++)
        for (int i = JA[k]; i < JA[k + 1]; i++)
            (*MATRIX)[IA[i] * m + k] = A[i];

    *MATRIX_ROW = calloc(n, sizeof(float));
    if (!*MATRIX_ROW)
    {
        printf("Не удалось выделить память под строку\n");
        return DATA_ERROR;
    }
    for (int i = 0; i < nz_row; i++)
        (*MATRIX_ROW)[J_ROW[i]] = ROW[i];

    return SUCCESS;
}

void print(float *COL_SOL, int m)
{
    if (m <= N)
    {
        printf("Результат умножения строки на матрицу (строка): ");
        for (int i = 0; i < m; i++)
            printf("%.0f ", COL_SOL[i]);
        printf("\n");
    }
    else
    {
        printf("Результат умножения строки на матрицу\n");
        printf("Массив ненулевых элементов: ");
        for (int i = 0; i < m; i++)
            if (COL_SOL[i] != 0)
                printf("%.0f ", COL_SOL[i]);
        printf("\n");
        printf("Массив, содержащий индексы строк элементов: ");
        for (int i = 0; i < m; i++)
            if (COL_SOL[i] != 0)
                printf("%d ", i);
        printf("\n");
    }
}

int fill_random(float *A, int *IA, int *JA, int *nz, int n, int m, float *ROW, int *J_ROW, int *nz_row, int percent)
{
    /// Инициализируем матрицу
    float *matrix = NULL;
    matrix = calloc(n * m, sizeof(float));
    if (matrix == NULL)
        return DATA_ERROR;

    /// Заполняем матрицу на ...%
    *nz = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (rand() % 100 < percent)
            {
                (*nz)++;
                matrix[i * m + j] = 1;
            }

    /// Сохраняем матрицу в формате трех обьектов
    JA[0] = 0;
    for (int i = 0, count = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            if (matrix[j * m + i] != 0)
            {
                A[count] = matrix[j * m + i];
                IA[count] = j;
                count++;
            }
        JA[i + 1] = count;
    }

    /// Заполняем строку на ...%
    for (int i = 0; i < n; i++)
        if (rand() % 100 < percent)
        {
            ROW[*nz_row] = 1;
            J_ROW[*nz_row] = i;
            (*nz_row)++;
        }

    free(matrix);
    return SUCCESS;
}

int diff(int n, int m, int percent)
{
    int code_exit = SUCCESS;
    float *A = NULL;
    int *IA = NULL;
    int *JA = NULL;
    int nz = 0;
    float *ROW = NULL;
    int *J_ROW = NULL;
    int nz_row = 0;
    float *COL_SOL = NULL;
    float *MATRIX = NULL;
    float *MATRIX_ROW = NULL;
    clock_t start, end;
    double time_lead_1, time_lead_2;
    unsigned long memory_1, memory_2;

    /// Выделяем память для массивов A, IA, JA, ROW, J_ROW
    A = malloc(sizeof(float) * n * m);
    if (A == NULL)
        code_exit = DATA_ERROR;
    IA = malloc(sizeof(int) * n * m);
    if (IA == NULL)
        code_exit = DATA_ERROR;
    JA = malloc(sizeof(int) * (m + 1));
    if (JA == NULL)
        code_exit = DATA_ERROR;
    ROW = malloc(sizeof(float) * n);
    if (ROW == NULL)
        code_exit = DATA_ERROR;
    J_ROW = malloc(sizeof(int) * n);
    if (J_ROW == NULL)
        code_exit = DATA_ERROR;

    if (code_exit == SUCCESS)
        code_exit = fill_random(A, IA, JA, &nz, n, m, ROW, J_ROW, &nz_row, percent);
    if (code_exit == SUCCESS)
    {
        start = clock();
        code_exit = special_multy(ROW, J_ROW, n, nz_row, A, IA, JA, m, &COL_SOL);
        end = clock();
        time_lead_1 = ((double) (end - start) / CLOCKS_PER_SEC);
        memory_1 = sizeof(float) * (nz + nz_row) + sizeof(int) * (nz + nz_row) + sizeof(int) * (m + 1);
    }
    if (code_exit == SUCCESS)
        code_exit = transfer(&MATRIX, A, IA, JA, n, m, &MATRIX_ROW, ROW, J_ROW, nz_row);
    if (code_exit == SUCCESS)
    {
        start = clock();
        code_exit = standart_multy(MATRIX, n, m, MATRIX_ROW, &COL_SOL);
        end = clock();
        time_lead_2 = ((double) (end - start) / CLOCKS_PER_SEC);
        memory_2 = sizeof(float) * n * m;
    }
    if (code_exit == SUCCESS)
    {
        printf("Время умножения матрицы, заполненной на %d процентов, в указанной форме: %lf\n", percent, time_lead_1);
        printf("Обьем выделенной памяти в байтах: %ld\n", memory_1);
        printf("Время умножения матрицы, заполненной на %d процентов, стандартным способом: %lf\n", percent, time_lead_2);
        printf("Обьем выделенной памяти в байтах: %ld\n", memory_2);
    }

    free(A);
    free(IA);
    free(JA);
    free(ROW);
    free(J_ROW);
    free(COL_SOL);
    free(MATRIX);
    free(MATRIX_ROW);
    return code_exit;
}

int main()
{
    int code_exit = SUCCESS;
    int choice;
    float *A = NULL;
    int *IA = NULL;
    int *JA = NULL;
    int n, m;
    int nz = 0; // количество ненулевых элементов
    float *ROW = NULL;
    int *J_ROW = NULL;
    int n_row;
    int nz_row = 0; // количество ненулевых элементов в строке
    float *COL_SOL = NULL;
    float *MATRIX = NULL;
    float *MATRIX_ROW = NULL;

    do
    {
        printf("1. Умножить вектор-строку на матрицу, хранящейся в указанной форме\n");
        printf("2. Умножить вектор-строку на матрицу из файла, хранящейся в указанной форме\n");
        printf("3. Умножить вектор-строку на матрицу стандартным алгоритмом\n");
        printf("4. Умножить вектор-строку на матрицу из файла стандартным алгоритмом\n");
        printf("5. Сравнение эффективности двух алгоритмов при разном проценте заполненности матрицы\n");
        printf("0. Выход\n");

        printf("Выберите действие: ");
        if (scanf("%d", &choice) != 1)
            code_exit = INPUT_ERROR;
        if (choice < 0 || choice > 5)
            code_exit = INPUT_ERROR;

        switch(choice)
        {
        case 1:
            if (code_exit == SUCCESS)
                code_exit = input(&A, &IA, &JA, &n, &m, &nz);
            if (code_exit == SUCCESS)
                code_exit = input_row(&ROW, &J_ROW, &n_row, &nz_row, n);
            if (code_exit == SUCCESS)
                special_multy(ROW, J_ROW, n_row, nz_row, A, IA, JA, m, &COL_SOL);
            if (code_exit == SUCCESS)
                print(COL_SOL, m);
            break;
        case 2:
            if (code_exit == SUCCESS)
                code_exit = finput(&A, &IA, &JA, &n, &m, &nz);
            if (code_exit == SUCCESS)
                code_exit = input_row(&ROW, &J_ROW, &n_row, &nz_row, n);
            if (code_exit == SUCCESS)
                special_multy(ROW, J_ROW, n_row, nz_row, A, IA, JA, m, &COL_SOL);
            if (code_exit == SUCCESS)
                print(COL_SOL, m);
            break;
        case 3:
            if (code_exit == SUCCESS)
                code_exit = input(&A, &IA, &JA, &n, &m, &nz);
            if (code_exit == SUCCESS)
                code_exit = input_row(&ROW, &J_ROW, &n_row, &nz_row, n);
            if (code_exit == SUCCESS)
                code_exit = transfer(&MATRIX, A, IA, JA, n, m, &MATRIX_ROW, ROW, J_ROW, nz_row);
            if (code_exit == SUCCESS)
                code_exit = standart_multy(MATRIX, n, m, MATRIX_ROW, &COL_SOL);
            if (code_exit == SUCCESS)
                print(COL_SOL, m);
            break;
        case 4:
            if (code_exit == SUCCESS)
                code_exit = finput(&A, &IA, &JA, &n, &m, &nz);
            if (code_exit == SUCCESS)
                code_exit = input_row(&ROW, &J_ROW, &n_row, &nz_row, n);
            if (code_exit == SUCCESS)
                code_exit = transfer(&MATRIX, A, IA, JA, n, m, &MATRIX_ROW, ROW, J_ROW, nz_row);
            if (code_exit == SUCCESS)
                code_exit = standart_multy(MATRIX, n, m, MATRIX_ROW, &COL_SOL);
            if (code_exit == SUCCESS)
                print(COL_SOL, m);
            break;
        case 5:
            for (int i = 0; i <= 100; i += 25)
                diff(200, 200, i);
            break;
        }


    printf("\n");
    } while (code_exit == SUCCESS && choice != 0);

    free(A);
    free(IA);
    free(JA);
    free(ROW);
    free(J_ROW);
    free(COL_SOL);
    free(MATRIX);
    free(MATRIX_ROW);

    if (code_exit == INPUT_ERROR)
        printf("Некорректное значение\n");
    return code_exit;
}

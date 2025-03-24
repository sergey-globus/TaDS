#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 40

#define SUCCESS 0
#define TOO_LONG 1
#define NOT_NUMBER 2
#define INPUT_FLOAT_ERROR 3
#define EMPTY_NUMBER 4
#define EXCESS_ORDER 5


int input_int (int number[N + 1], int *n, char *sign_int)
{
    char tmp[N + 2];
    int first = 0; // индекс первой цифры(есть знак - 1/нет знака - 0)

    /// Ввод числа
    char ch;
    int j;
    printf("Введите целое число\n");
    printf("+---------1---------2---------3---------4\n");
    for (j = 0; (ch = getchar()) != '\n'; j++)
    {
        if (j == N + 2)
        {
            printf("Слишком большое выражение\n");
            return TOO_LONG;
        }
        tmp[j] = ch;
    }
    tmp[j] = '\0';

    /// Перенеос числа в целочисленный массив
    if (tmp[0] == '+' || tmp[0] == '-')
        first = 1;
    for (int i = first; tmp[i]; i++)
    {

        if (!(tmp[i] >= '0' && tmp[i] <= '9'))
        {
            printf("Введено нечисло\n");
            return NOT_NUMBER;
        }
        if (*n == N)
        {
            printf("Слишком большое число\n");
            return TOO_LONG;
        }
        number[(*n)++] = tmp[i] - '0';
    }

    /// Проверка на пустое число
    if (*n == 0)
    {
        printf("Не введено число\n");
        return EMPTY_NUMBER;
    }

    /// Знак целого числа
    if (tmp[0] == '-')
        *sign_int = '-';

    return 0;
}

int input_float(int mantissa[N], int *m, int *k, char *sign_float)
{
    char tmp[65];
    char number[N + 10];
    char e[2];
    char order[7];
    char extra[1];
    int args = 0;
    int first = 0; // индекс первой цифры(есть знак - 1/нет знака - 0)
    int dote = -1; // индекс точки, отрицательное значение при ее отсутствии

    /// Ввод вещественного выражения
    char ch;
    int j;
    printf("Введите вещественное число\n");
    printf("+---------1---------2---------3---------4---------5\n");
    for (j = 0; (ch = getchar()) != '\n'; j++)
    {
        if (j == 65)
        {
            printf("Слишком большое выражение\n");
            return TOO_LONG;
        }
        tmp[j] = ch;
    }
    tmp[j] = '\0';

    /// Разбиение на мантиссу и порядок
    args = sscanf(tmp, "%s %s %s %s", number, e, order, extra);
    if (args == 1)
        *k = 0;
    else if (args == 3 && strcmp(e, "E") == 0)
    {
        if (order[0] == '+' || order[0] == '-')
            first = 1;
        for (int i = first; order[i]; i++)
        {
            if (!(order[i] >= '0' && order[i] <= '9'))
            {
                printf("В порядке числа введено нечисло\n");
                return NOT_NUMBER;
            }
            *k = 10 * (*k) + (order[i] - '0');
            if (abs(*k) > 99999)
            {
                printf("Слишком большой порядок числа\n");
                return TOO_LONG;
            }
        }
        if (order[0] == '-')
            *k *= -1;
    }
    else
    {
        printf("Ошибка при вводе вещественного выражения\n");
        return INPUT_FLOAT_ERROR;
    }

    /// Определяем индекс первой цифры мантиссы
    if (number[0] == '+' || number[0] == '-')
        first = 1;
    else
        first = 0;

    /// Ввод числа до точки
    for (int i = first; number[i]; i++)
    {
        if (number[i] == '.')
        {
            dote = i;
            break;
        }
        if (!(number[i] >= '0' && number[i] <= '9'))
        {
            printf("В мантиссе числа введено нечисло\n");
            return NOT_NUMBER;
        }
        if (number[i] == '0' && *m == 0)
            continue;
        if (*m == N)
        {
            printf("Слишком большая мантисса числа\n");
            return TOO_LONG;
        }
        mantissa[(*m)++] = number[i] - '0';
    }

    /// Ввод числа после точки
    if (dote >= 0)
        for (int i = dote + 1; number[i]; i++)
        {
            if (!(number[i] >= '0' && number[i] <= '9'))
            {
                printf("В мантиссе числа введено нечисло\n");
                return NOT_NUMBER;
            }
            if (number[i] == '0' && *m == 0)
            {
                (*k)--;
                continue;
            }
            if (*m == N)
            {
                printf("Слишком большая мантисса числа\n");
                return TOO_LONG;
            }
            mantissa[(*m)++] = number[i] - '0';
            (*k)--;
        }

    /// Знак вещественного числа
    if (number[0] == '-')
        *sign_float = '-';

    /// Проверка введенного числа
    if (*m == 0)
    {
        printf("Не введено число или введен ноль\n");
        return EMPTY_NUMBER;
    }

    return SUCCESS;
}

int comparator(int num_1[N + 1], int *n1, int num_2[N], int n2)
{
    int first = 0; // Индекс первого ненулевого числа

    /// Убираем нули в начале первого числа
    for (int i = 0; num_1[i] == 0; i++)
        first++;
    if (first > 0)
    {
        *n1 -= first;
        for (int i = 0; i < (*n1); i++)
            num_1[i] = num_1[first + i];
    }

    /// Сравнение чисел по длине
    if (*n1 < n2)
        return 0;

    /// Сравнение равных по длине чисел
    if (*n1 == n2)
        for (int i = 0; i < *n1; i++)
        {
            if (num_1[i] < num_2[i])
                return 0;
            else if (num_1[i] > num_2[i])
                return 1;
        }

    return 1;
}

int integer_div(int num_1[N + 1], int *n1, int num_2[N], int n2)
{
    int q = 0; // целочисленное частное
    int n;

    while (comparator(num_1, n1, num_2, n2)) // Пока первое число больше вычетаем из него второе
    {
        n = *n1 - n2;
        for (int i = n2 - 1; i >= 0; i--)
        {
            num_1[n + i] -= num_2[i];
            if (num_1[n + i] < 0)
            {
                num_1[n + i] += 10;
                num_1[n + i - 1] -= 1;
            }
        }
        q++;
    }

    return q;
}

int dividing(int num_1[N + 1], int n1, int num_2[N], int n2, int ratio[N], int *m1 ,int *k1)
{
    int tmp;


    /// "Выравнивание" вещественного числа под целочисленное
    if (n1 > n2)
        for (int i = n2; i < n1; i++)
        {
            num_2[i] = 0;
            n2++;
            (*k1)++;
        }

    /// Вычисление частного чисел
    tmp = integer_div(num_1, &n1, num_2, n2);
    if (tmp != 0)
    {
        ratio[(*m1)++] = tmp;
        (*k1)++;
    }
    while (num_1[0] != 0 && *m1 < N) // пока остаток не равен нулю
    {
        /// Умножаем первое число на 10
        num_1[n1] = 0;
        n1++;
        /// Вычисляем i-тую цифру частного и записваем в первое число остаток
        tmp = integer_div(num_1, &n1, num_2, n2);
        if (tmp == 0 && *m1 == 0) // первые нули не пишем как незначащие
            (*k1)--;
        else
            ratio[(*m1)++] = tmp;
    }

    /// Округление числа
    if (*m1 == N && num_1[0] != 0)
    {
        num_1[n1] = 0;
        n1++;
        tmp = integer_div(num_1, &n1, num_2, n2);

        if (tmp >= 5)
        {
            ratio[N - 1] += 1;
            for (int i = N - 1; ratio[i] == 10; i--)
            {
                if (i == 0)
                {
                    ratio[0] = 1;
                    (*k1)++;
                }
                ratio[i] = 0;
                ratio[i - 1] += 1;
            }
        }
    }

    /// Проверка на диапазон порядка
    if (abs(*k1) > 99999)
    {
        printf("Порядок результирующего числа больше пяти цифр\n");
        return EXCESS_ORDER;
    }

    return 0;
}

int main()
{
    int code_exit = SUCCESS;
    int number_int[N + 1];
    int n = 0;
    char sign_int = '+';
    int mantissa[N];
    int m = 0;
    int k = 0;
    char sign_float = '+';
    int ratio[N];
    int m1 = 0;
    int k1;
    char sign_ratio = '+';

    /// Ввод целого числа
    if (code_exit == SUCCESS)
        code_exit = input_int(number_int, &n, &sign_int);

    /// Ввод вещественного числа
    if (code_exit == SUCCESS)
        code_exit = input_float(mantissa, &m, &k, &sign_float);

/*!
    /// Промежуточный вывод введенных чискл
    if (code_exit == SUCCESS)
    {
        printf("%c", sign_int);
        for (int i = 0; i < n; i++)
            printf("%d", number_int[i]);
        printf("\n");
        printf("%c", sign_float);
        for (int i = 0; i < m; i++)
            printf("%d", mantissa[i]);
        printf(" %d\n", k);
    }
*/

    /// Деление числа
    if (code_exit == SUCCESS)
    {
        k1 = -k;
        code_exit = dividing(number_int, n, mantissa, m, ratio, &m1, &k1);
    }

    /// Вывод числа
    if (code_exit == SUCCESS)
    {
        if (sign_int == sign_float)
            sign_ratio = '+';
        else
            sign_ratio = '-';

        printf("Частное чисел:\n");
        printf("%c0.", sign_ratio);
        for (int i = 0; i < m1; i++)
            printf("%d", ratio[i]);
        printf(" E %d\n", k1);
    }

    return code_exit;
}

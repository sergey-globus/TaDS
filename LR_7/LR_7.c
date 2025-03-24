#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUCCESS 0

typedef struct node node_t;

struct node
{
    int num;
    node_t *next;
};

node_t* create_node(int number)
{
    node_t *elem = NULL;
    elem = malloc(sizeof(node_t));

    if (elem)
    {
        elem->num = number;
        elem->next = NULL;
    }

    return elem;
}

void add_end(node_t **head, node_t *new_elem)
{
    node_t *cur = *head;
    if (cur == NULL)
        *head = new_elem;
    else
    {
        for ( ; cur->next; cur = cur->next)
            ;
        cur->next = new_elem;
    }
}

int hash_func(int number, int N)
{
    return number % N;
}

node_t **input(int *N)
{
    char str[100];
    int num;
    node_t **arr;

    printf("Введите строку: ");
    scanf(" %[^\n]", str);

    int total_n = 0;
    int n;
    int index;
    node_t *cur;
    while (sscanf(str + total_n, "%d%n", &num, &n) == 1)
    {
        total_n += n;
        (*N)++;
    }
    total_n = 0;
    (*N) = (int) ((*N) * 0.7);
    arr = malloc((*N) * sizeof(node_t*));

    while (sscanf(str + total_n, "%d%n", &num, &n) == 1)
    {
        total_n += n;
        index = hash_func(num, *N);
        cur = create_node(num);
        add_end(&arr[index], cur);
    }
    return arr;
}

void print(node_t **arr, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (node_t *cur = arr[i]; cur; cur = cur->next)
            printf("%d ", cur->num);
        if (arr[i] != NULL)
            printf("\n");
    }
}

int exist(node_t **arr, int N, int number)
{
    int flag_exist = 0;
    int index = hash_func(number, N);

    for (node_t *cur = arr[index]; cur; cur = cur->next)
        if (cur->num == number)
            flag_exist = 1;

    return flag_exist;
}

int *hash_massive(node_t **arr, int N, int *n)
{
    int *massive = NULL;
    for (int i = 0; i < N; i++)
        for (node_t *cur = arr[i]; cur; cur = cur->next)
            (*n)++;
    massive = malloc((*n) * sizeof(int));
    int k = 0;
    for (int i = 0; i < N; i++)
        for (node_t *cur = arr[i]; cur; cur = cur->next)
            massive[k++] = cur->num;
    return massive;
}

int find(node_t **arr, int N, int number)
{
    int k = 1;
    int index = hash_func(number, N);
    for (node_t *cur = arr[index]; cur && cur->num != number; cur = cur->next)
        k++;
    return k;
}

int average_compare(node_t **arr, int N, int *massive, int n)
{
    int k = 0;
    for (int i = 0; i < n; i++)
        k += find(arr, N, massive[i]);
    return k;
}

int main()
{
    int code_exit = SUCCESS;
    node_t **arr;
    int N = 0;

    arr = input(&N);
    print(arr, N);

    int number;
    printf("Введите число: ");
    scanf("%d", &number);
    if (exist(arr, N, number))
        printf("Введенное число в хэш-таблице\n");
    else
        printf("Введенное число не найдено\n");

    int n = 0;
    int *massive = hash_massive(arr, N, &n);
    printf("Количество сравнений при поиске всех данных - %d\n", average_compare(arr, N, massive, n));


    return code_exit;
}

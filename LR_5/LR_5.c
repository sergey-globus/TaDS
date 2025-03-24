#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2000

#define SUCCESS 0
#define EMPTY_ERROR 1
#define OVER_ERROR 2
#define INPUT_ERROR 3

typedef struct queue_array
{
    int arr[N];
    int len;
} queue_a;

typedef struct node
{
    int num;
    struct node *next;
} node_t;

typedef struct queue_list
{
    node_t *pin;
    node_t *pout;
} queue_l;

int pop_array(queue_a *queue)
{
    queue->len--;
    return queue->arr[queue->len];
}

void delete_array(queue_a *queue)
{
    queue->len--;
    for (int i = 0; i < queue->len; i++)
        queue->arr[i] = queue->arr[i + 1];
}

void push_array(queue_a *queue, int num)
{
    for (int i = queue->len; i > 0; i--)
        queue->arr[i] = queue->arr[i - 1];
    queue->arr[0] = num;
    queue->len++;
}

void push_list(queue_l *queue, int num)
{
    node_t *node = malloc(sizeof(node_t));
    node->next = NULL;
    node->num = num;

    if (queue->pout == NULL)
    {
        queue->pin = node;
        queue->pout = node;
    }
    else
    {
        node->next = queue->pin;
        queue->pin = node;
    }
}

node_t *pop_list(queue_l *queue)
{
    node_t *tmp = queue->pout;
    node_t *cur = queue->pin;
    if (cur == tmp)
    {
        queue->pin = NULL;
        queue->pout = NULL;
        return tmp;
    }

    for (; cur; cur = cur->next)
        if (cur->next == tmp)
        {
            cur->next = NULL;
            queue->pout = cur;
        }
    return tmp;
}

void delete_list(queue_l *queue)
{
    queue->pin = queue->pin->next;
}

void diff(int n)
{
    queue_a queue_arr;
    queue_l queue_list;
    clock_t start, end;
    double time_lead;
    unsigned long memory;

    start = clock();
    for (int i = 0; i < n; i++)
        push_array(&queue_arr, i + 1);
    for (int i = 0; i < queue_arr.len; i++)
        pop_array(&queue_arr);
    end = clock();
    time_lead = ((double) (end - start) / CLOCKS_PER_SEC);
    memory = N * sizeof(int);
    printf("МАССИВ: время обработки - %lf, выделенная память - %ld\n", time_lead, memory);

    start = clock();
    for (int i = 0; i < n; i++)
        push_list(&queue_list, i + 1);
    for (int i = 0; i < n; i++)
        pop_list(&queue_list);
    end = clock();
    time_lead = ((double) (end - start) / CLOCKS_PER_SEC);
    memory = n * sizeof(node_t);
    printf("СПИСОК: время обработки - %lf, выделенная память - %ld\n", time_lead, memory);
}

int main()
{
    int code_exit = SUCCESS;
    queue_a queue_arr;
    queue_l queue_list;
    int choice;

    int num;

    do
    {
        printf("1. Добавление элемента в массив\n");
        printf("2. Удаление элемента из массива\n");
        printf("3. Отображение текущего состояния очереди (массива)\n");
        printf("4. Добавление элемента в список\n");
        printf("5. Удаление элемента из списка\n");
        printf("6. Отображение текущего состояния очереди (списка)\n");
        printf("7. Сравнение эффективности\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");

        if (scanf("%d", &choice) != 1 || choice < 0 || choice > 7)
            code_exit = INPUT_ERROR;

        if (choice == 1)
        {
            printf("Введите число: ");
            if (scanf("%d", &num) != 1)
                code_exit = INPUT_ERROR;
            if (queue_arr.len == N)
                printf("Переполнение очереди\n");
            else
                push_array(&queue_arr, num);
        }
        else if (choice == 2)
        {
            if (queue_arr.len == 0)
                printf("Ошибка, очередь пуста\n");
            else
                pop_array(&queue_arr);
        }
        else if (choice == 3)
        {
            int len = queue_arr.len;
            if (len == 0)
                printf("Очередь пуста");
            for (int i = 0; i < len; i++)
                printf("%d ", pop_array(&queue_arr));
            printf("\n");
        }
        else if (choice == 4)
        {
            printf("Введите число: ");
            if (scanf("%d", &num) != 1)
                code_exit = INPUT_ERROR;
            int len = 0;
            for (node_t *cur = queue_list.pin; cur; cur = cur->next)
                len++;
            if (len == N)
                code_exit = OVER_ERROR;
            else
                push_list(&queue_list, num);
        }
        else if (choice == 5)
        {
            if (queue_list.pin == NULL)
                printf("Ошибка, очередь пуста\n");
            else
                pop_list(&queue_list);
        }
        else if (choice == 6)
        {
            node_t *cur;
            if (queue_list.pin == NULL)
                printf("Очередь пуста");
            while ((cur = pop_list(&queue_list)) != NULL)
                printf("%d ", cur->num);
            printf("\n");
        }
        else if (choice == 7)
        {
            printf("Для 10 элементов\n");
            diff(10);
            printf("Для 50 элементов\n");
            diff(50);
            printf("Для 100 элементов\n");
            diff(100);
            printf("Для 200 элементов\n");
            diff(200);
            printf("Для 500 элементов\n");
            diff(500);
            printf("Для 1000 элементов\n");
            diff(1000);
            printf("Для 1500 элементов\n");
            diff(1500);
            printf("Для 2000 элементов\n");
            diff(2000);
            printf("\n");
        }
    } while(code_exit == SUCCESS && choice != 0);

    if (code_exit == INPUT_ERROR)
        printf("Ошибка, некорректный ввод\n");
    else if (code_exit == EMPTY_ERROR)
        printf("Ошибка, очередь пуста\n");
    else if (code_exit == OVER_ERROR)
        printf("Переполнение очереди\n");

    return code_exit;
}

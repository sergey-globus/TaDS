/*!
 * Лабораторная работа 4, вариант 3
 * Элементами стека являются слова.
 * Распечатайте слова в обратном порядке,
 * каждое слово в перевернутом виде.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 100

#define SUCCESS 0
#define MEMORY_ERROR 1
#define INPUT_ERROR 2
#define STACK_OVERFLOW 3

typedef struct node node_t;

struct node
{
    void *data;
    node_t *next;
};

node_t *create_node(void *data, size_t size)
{
    node_t *node = NULL;
    node = malloc(sizeof(node_t));
    if (node)
    {
        node->data = malloc(size);
        if (!node->data)
        {
            free(node);
            return NULL;
        }
        memcpy(node->data, data, size);
        node->next = NULL;
    }
    return node;
}

int push(node_t **PS, void *data, size_t size)
{
    node_t *PS_new = create_node(data, size);
    if (!PS_new)
        return MEMORY_ERROR;

    PS_new->next = *PS;
    *PS = PS_new;
    return SUCCESS;
}

void *pop(node_t **PS)
{
    void *data = NULL;
    node_t *tmp;
    if (*PS)
    {
        tmp = *PS;
        data = (*PS)->data;
        *PS = (*PS)->next;
        free(tmp);
    }
    return data;
}

char *getword(void)
{
    size_t len = 0;
    char *read_word = NULL;
    printf("Введите слово: ");
    getline(&read_word, &len, stdin);
    sscanf(read_word, "%s", read_word);
    return read_word;
}

void print_word(char *word)
{
    node_t *PS = NULL;
    char *sym;
    for (int i = 0; word[i]; i++)
        push(&PS, &word[i], sizeof(char));

    while ((sym = pop(&PS)) != NULL)
        printf("%c", *sym);
    printf(" ");
}

void diff(int n)
{
    char *a[N];
    node_t *PS = NULL;
    clock_t start, end;
    double time_lead;
    unsigned long memory;
    char *word = "random_word";
    printf("СТЕК - СТАТИЧЕСКИЙ МАССИВ\n");
    start = clock();
    for (int i = 0; i < n; i++)
        a[i] = word;
    end = clock();
    time_lead = ((double) (end - start) / CLOCKS_PER_SEC);
    printf("Время добавления %d элементов: %lf\n", n, time_lead);

    start = clock();
    for (int i = 0; i < n; i++)
        ;
    end = clock();
    time_lead = ((double) (end - start) / CLOCKS_PER_SEC);
    printf("Время удаления %d элементов: %lf\n", n, time_lead);
    printf("Память в байтах: %ld\n", sizeof(char *) * N);

    printf("СТЕК - ОДНОСВЯЗНЫЙ СПИСОК\n");
    start = clock();
    for (int i = 0; i < n; i++)
        push(&PS, word, sizeof(word));
    end = clock();
    time_lead = ((double) (end - start) / CLOCKS_PER_SEC);
    printf("Время добавления %d элементов: %lf\n", n, time_lead);

    start = clock();
    for (int i = 0; i < n; i++)
        free(pop(&PS));
    end = clock();
    time_lead = ((double) (end - start) / CLOCKS_PER_SEC);
    printf("Время удаления %d элементов: %lf\n", n, time_lead);
    printf("Память в байтах: %ld\n", sizeof(node_t) * n);

}

int main()
{
    int code_exit = SUCCESS;
    char *a[N];
    int n = 0;
    node_t *PS = NULL;
    char *word = NULL;
    int choice = 0;
    int stack_empty;

    do
    {
        printf("***** Стек - статический массив *****\n");
        printf("1. Ввод элемента\n");
        printf("2. Удаление элемента\n");
        printf("3. Просмотр текущего состояния стека\n");
        printf("4. Вывод слов, каждое слово в перевернутом виде\n");
        printf("***** Стек - односвязный список *****\n");
        printf("5. Ввод элемента\n");
        printf("6. Удаление элемента\n");
        printf("7. Просмотр текущего состояния стека\n");
        printf("8. Вывод слов, каждое слово в перевернутом виде\n");
        printf("*************************************\n");
        printf("9. Сравнение эффективности двух реализаций стека\n");
        printf("0. Завершение программы\n");
        printf("\n");
        printf("Выберите пункт меню: ");
        if (scanf("%d", &choice) != 1 || choice < 0 || choice > 9)
        {
            printf("Некорректный пункт меню\n");
            code_exit = INPUT_ERROR;
        }
        getchar();

        switch (choice)
        {
        case 1:
            if (n == N)
            {
                printf("Переполнение стека\n");
                code_exit = STACK_OVERFLOW;
            }
            else
            {
                a[n] = getword();
                n++;
            }
            break;
        case 2:
            if (n == 0)
                printf("Стек пуст\n");
            else
            {
                free(a[n - 1]);
                n--;
            }
            break;
        case 3:
            if (n == 0)
                printf("Стек пуст\n");
            else
            {
                for (int i = n - 1; i >= 0; i--)
                {
                    printf("%s\n", a[i]);
                    free(a[i]);
                }
                n = 0;
            }
            break;
        case 4:
            if (n == 0)
                printf("Стек пуст\n");
            else
            {
                for (int i = n - 1; i >= 0; i--)
                {
                    print_word(a[i]);
                    free(a[i]);
                }
                n = 0;
            }
            break;
        case 5:
            word = getword();
            push(&PS, word, sizeof(word));
            free(word);
            break;
        case 6:
            if ((word = pop(&PS)) == NULL)
                printf("Стек пуст\n");
            free(word);
            break;
        case 7:
            stack_empty = 1;
            node_t **address;
            while (PS != NULL)
            {
                if (stack_empty)
                    printf("%15s %15s %16s\n\n", "Element", "Address", "Free memory area");
                stack_empty = 0;
                address = &PS;
                word = PS->data;
                printf("%15s %15p", word, (void *) address);
                pop(&PS);
                free(word);
                printf("%16p\n", (void *) address);
            }
            if (stack_empty)
                printf("Стек пуст\n");
            break;
        case 8:
            stack_empty = 1;
            while ((word = pop(&PS)) != NULL)
            {
                stack_empty = 0;
                print_word(word);
                free(word);
            }
            if (stack_empty)
                printf("Стек пуст\n");
            else
                printf("\n");
            break;
        case 9:
            diff(10);
            printf("\n");
            diff(50);
            printf("\n");
            diff(100);
            break;
        }
        printf("\n");

    } while (code_exit == SUCCESS && choice != 0);

    return code_exit;
}

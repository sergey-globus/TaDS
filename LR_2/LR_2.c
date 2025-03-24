/*!
 * Ввести список квартир, содержащий адрес, общую площадь,
 * количество комнат, стоимость квадратного метра, первичное
 * жилье или нет (первичное – с отделкой или без нее; вторичное –
 * время постройки, количество предыдущих собственников, были ли
 * животные). Найти все вторичное 2-х комнатное жилье в указанном
 * ценовом диапазоне без животных.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DATA_FILE "apart_data.txt"
#define N 80

#define SUCCESS 0
#define FILE_ERROR 1
#define INPUT_ERROR 2
#define EMPTY_STRUCT 3

struct apartments
{
    char address[51];
    float area;
    int rooms;
    float price;
    char  is_primary[10]; // Primary/Secondary
    union
    {
        struct
        {
            char with_finish[4]; // yes/no
        }primary;
        struct
        {
            int year;
            int owners;
            char has_pets[4]; // yes/no
        }secondary;
    }apart_type;
};

struct keys
{
    int src_index;
    float price;
};

void input(FILE *f, struct apartments *arr, int *n)
{
    char *read = NULL;
    size_t len = 0;
    char *delim = "|";
    while (getline(&read, &len, f) != -1)
    {
        strcpy(arr[*n].address, strtok(read, delim));
        arr[*n].area = atof(strtok(NULL, delim));
        arr[*n].rooms = atoi(strtok(NULL, delim));
        arr[*n].price = atof(strtok(NULL, delim));
        strcpy(arr[*n].is_primary, strtok(NULL, delim));
        if (strcmp(arr[*n].is_primary, "Primary") == 0)
            strcpy(arr[*n].apart_type.primary.with_finish, strtok(NULL, delim));
        else
        {
            arr[*n].apart_type.secondary.year = atoi(strtok(NULL, delim));
            arr[*n].apart_type.secondary.owners = atoi(strtok(NULL, delim));
            strcpy(arr[*n].apart_type.secondary.has_pets, strtok(NULL, delim));
        }

        (*n)++;
    }
    free(read);
}

int add(struct apartments *arr, int *n)
{
    printf("Введите адрес квартиры (не больше 50 символв): ");
    scanf(" %[^\n]", arr[*n].address);
    printf("Введите общую площадь в кв. м: ");
    if (scanf("%f", &arr[*n].area) != 1)
        return INPUT_ERROR;
    printf("Введите количество комнат: ");
    if (scanf("%d", &arr[*n].rooms) != 1)
        return INPUT_ERROR;
    printf("Введите стоимость квадратного метра $: ");
    if(scanf("%f", &arr[*n].price) != 1)
        return INPUT_ERROR;
    printf("Первичное жилье или нет (Primary/Secondary): ");
    scanf("%s", arr[*n].is_primary);
    if (strcmp(arr[*n].is_primary, "Primary") == 0)
    {
        printf("С отделкой или без нее (yes/no): ");
        scanf("%s", arr[*n].apart_type.primary.with_finish);
        if (!(strcmp(arr[*n].apart_type.primary.with_finish, "yes") == 0 || strcmp(arr[*n].apart_type.primary.with_finish, "no") == 0))
            return INPUT_ERROR;
    }
    else if (strcmp(arr[*n].is_primary, "Secondary") == 0)
    {
        printf("Введите год постройки: ");
        if (scanf("%d", &arr[*n].apart_type.secondary.year) != 1)
            return INPUT_ERROR;
        printf("Введите количество предыдущих собственников: ");
        if (scanf("%d", &arr[*n].apart_type.secondary.owners) != 1)
            return INPUT_ERROR;
        printf("Были ли животные (yes/no): ");
        scanf("%s", arr[*n].apart_type.secondary.has_pets);
        if (!(strcmp(arr[*n].apart_type.secondary.has_pets, "yes") || strcmp(arr[*n].apart_type.secondary.has_pets, "no")))
            return INPUT_ERROR;
    }
    else
        return INPUT_ERROR;
    (*n)++;
    return SUCCESS;
}

int delete(struct apartments *arr, int *n)
{
    int k;
    if (*n == 0)
    {
        printf("Ошибка, база данных пуста\n");
        return EMPTY_STRUCT;
    }

    printf("Введите порядковый номер квартиры, который нужно удалить: ");
    if (scanf("%d", &k) != 1)
        return INPUT_ERROR;
    if (k < 1 || k > *n)
        return INPUT_ERROR;

    for (int i = k; i < *n; i++)
        arr[i - 1] = arr[i];

    (*n)--;
    return SUCCESS;
}

void sort(struct apartments *arr, int n)
{
    struct apartments tmp;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (arr[j].price > arr[j + 1].price)
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
}

void sort_key(struct apartments *arr, struct keys *arr_keys, int n)
{
    struct keys tmp;

    /// Заполняем массив ключей
    for (int i = 0; i < n; i++)
    {
        arr_keys[i].src_index = i + 1;
        arr_keys[i].price = arr[i].price;
    }

    /// Сортируем
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (arr_keys[j].price > arr_keys[j + 1].price)
            {
                tmp = arr_keys[j];
                arr_keys[j] = arr_keys[j + 1];
                arr_keys[j + 1] = tmp;
            }
}

void print_key(struct keys *arr_keys, int n)
{
    for (int i = 0; i < 23; i++)
        printf("-");
    printf("\n| №  | Index | $Price |\n");
    for (int i = 0; i < 23; i++)
        printf("-");
    printf("\n");

    for (int i = 0; i < n; i++)
        printf("| %-3d|  %-5d|  %-6.0f|\n", i + 1, arr_keys[i].src_index, arr_keys[i].price);

    for (int i = 0; i < 23; i++)
        printf("-");
    printf("\n");
}

void print_with_key(struct apartments *arr, struct keys *arr_keys, int n)
{

    if (n == 0)
    {
        printf("База данных пуста\n");
        return;
    }

    for (int i = 0; i < 142; i++)
        printf("-");
    printf("\n|№ |%-50s| %-5s| %-6s| $%-6s| Primary/Secondary | With finish? | Year | Owners | Has pets? |\n", "Address", "Area", "Rooms", "Price");
    for (int i = 0; i < 142; i++)
        printf("-");
    printf("\n");

    int i;
    for (int j = 0; j < n; j++)
    {
        i = arr_keys[j].src_index - 1;
        printf("|%-2d|%-50s|  %-4.0f|   %-4d|  %-6.0f|", j + 1, arr[i].address, arr[i].area, arr[i].rooms, arr[i].price);
        if (strcmp(arr[i].is_primary, "Primary") == 0)
            printf("      %-13s|      %-8s|  --  |   --   |     --    |\n", arr[i].is_primary, arr[i].apart_type.primary.with_finish);
        else
        {
            printf("    %-15s|      --      | %-5d", arr[i].is_primary, arr[i].apart_type.secondary.year);
            printf("|   %-5d|     %-6s|\n", arr[i].apart_type.secondary.owners, arr[i].apart_type.secondary.has_pets);
        }
    }

    for (int i = 0; i < 142; i++)
        printf("-");
    printf("\n");
}

void print(struct apartments *arr, int n)
{
    if (n == 0)
    {
        printf("База данных пуста\n");
        return;
    }

    for (int i = 0; i < 142; i++)
        printf("-");
    printf("\n|№ |%-50s| %-5s| %-6s| $%-6s| Primary/Secondary | With finish? | Year | Owners | Has pets? |\n", "Address", "Area", "Rooms", "Price");
    for (int i = 0; i < 142; i++)
        printf("-");
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        printf("|%-2d|%-50s|  %-4.0f|   %-4d|  %-6.0f|", i + 1, arr[i].address, arr[i].area, arr[i].rooms, arr[i].price);
        if (strcmp(arr[i].is_primary, "Primary") == 0)
            printf("      %-13s|      %-8s|  --  |   --   |     --    |\n", arr[i].is_primary, arr[i].apart_type.primary.with_finish);
        else
        {
            printf("    %-15s|      --      | %-5d", arr[i].is_primary, arr[i].apart_type.secondary.year);
            printf("|   %-5d|     %-6s|\n", arr[i].apart_type.secondary.owners, arr[i].apart_type.secondary.has_pets);
        }
    }

    for (int i = 0; i < 142; i++)
        printf("-");
    printf("\n");
}

int find(struct apartments *arr, int n)
{
    int flag_no_find = 1;
    float min_price, max_price;
    char defis[1];
    printf("Введите ценовой диапазон $ (1500 - 2000): ");
    if (scanf("%f%s%f", &min_price, defis, &max_price) != 3 || strcmp(defis, "-") != 0)
        return INPUT_ERROR;
    if (min_price > max_price)
        return INPUT_ERROR;
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        if (arr[i].rooms == 2 && strcmp(arr[i].is_primary, "Secondary") == 0 && strcmp(arr[i].apart_type.secondary.has_pets, "no") == 0)
            if (arr[i].price >= min_price && arr[i].price <= max_price)
            {
                flag_no_find = 0;
                printf("|%-2d|%-50s|  %-4.0f|   %-4d|  %-6.0f|", i + 1, arr[i].address, arr[i].area, arr[i].rooms, arr[i].price);
                if (strcmp(arr[i].is_primary, "Primary") == 0)
                    printf("      %-13s|      %-8s|  --  |   --   |     --    |\n", arr[i].is_primary, arr[i].apart_type.primary.with_finish);
                else
                {
                    printf("    %-15s|      --      | %-5d", arr[i].is_primary, arr[i].apart_type.secondary.year);
                    printf("|   %-5d|     %-6s|\n", arr[i].apart_type.secondary.owners, arr[i].apart_type.secondary.has_pets);
                }
            }
    }
    if (flag_no_find)
        printf("Такого жилья не найдено\n");
    return SUCCESS;
}

void rewrite(struct apartments *arr, int n)
{
    FILE *f = fopen(DATA_FILE, "w");
    for (int i = 0; i < n; i++)
    {
        fprintf(f, "%s|%.0f|%d|%.0f|", arr[i].address, arr[i].area, arr[i].rooms, arr[i].price);
        if (strcmp(arr[i].is_primary, "Primary") == 0)
            fprintf(f, "%s|%s|\n", arr[i].is_primary, arr[i].apart_type.primary.with_finish);
        else
        {
            fprintf(f, "%s|%d", arr[i].is_primary, arr[i].apart_type.secondary.year);
            fprintf(f, "|%d|%s|\n", arr[i].apart_type.secondary.owners, arr[i].apart_type.secondary.has_pets);
        }
    }
    fclose(f);
}

void diff(struct apartments *arr, struct keys *arr_keys, int n)
{
    clock_t start, end;
    double time_lead_1, time_lead_2;
    struct apartments arr_tmp[N];
    struct keys arr_keys_tmp[N];

    for (int i = 0; i < n; i++)
        arr_tmp[i] = arr[i];

    start = clock();
    sort_key(arr_tmp, arr_keys_tmp, n);
    end = clock();
    time_lead_1 = ((double) (end - start) / CLOCKS_PER_SEC);

    start = clock();
    sort(arr_tmp, n);
    end = clock();
    time_lead_2 = ((double) (end - start) / CLOCKS_PER_SEC);

    printf("\n");
    printf("Время обработки данных с помощью массива ключей: %lf\n", time_lead_1);
    printf("Обьем выделенной памяти в байтах: %ld\n", n * sizeof(struct keys));
    printf("Время обработки данных с помощью исходной таблицы: %lf\n", time_lead_2);
    printf("Обьем выделенной памяти в байтах: %ld\n", n * sizeof(struct apartments));
}

int main()
{
    int code_exit = SUCCESS;
    struct apartments arr[N];
    struct keys arr_keys[N];
    int n = 0;
    int choice;

    /// Откроем файл
    FILE *f = fopen(DATA_FILE, "r");
    if (f == NULL)
        code_exit = FILE_ERROR;

    /// Занесем информацию из файла в структуру
    if (code_exit == SUCCESS)
        input(f, arr, &n);

    /// Закроем файл
    if (code_exit != FILE_ERROR)
        fclose(f);

    do
    {
        printf("1. Добавить запись\n");
        printf("2. Удалить запись\n");
        printf("3. Упорядочить список квартир по стоимости, используя таблицу\n");
        printf("4. Просмотр отсортированной таблицы стоимостей(ключей) без сортировки таблицы\n");
        printf("5. Вывести упорядоченный по стоимости список квартир, используя массив ключей\n");
        printf("6. Вывести таблицу на экран\n");
        printf("7. Найти все вторичное 2-х комнатное жилье в указанном ценовом диапазоне без животных\n");
        printf("8. Перезаписать файл\n");
        printf("9. Вывод сравнения результатов сортировки в исходной таблицы и спомощью массива ключей\n");
        printf("0. Выход\n");

        printf("Выберите действие: ");
        if (scanf("%d", &choice) != 1)
            code_exit = INPUT_ERROR;
        if (choice < 0 || choice > 9)
            code_exit = INPUT_ERROR;

        switch(choice)
        {
        case 1:
            code_exit = add(arr, &n);
            break;
        case 2:
            code_exit = delete(arr, &n);
            break;
        case 3:
            sort(arr, n);
            break;
        case 4:
            sort_key(arr, arr_keys, n);
            print_key(arr_keys, n);
            break;
        case 5:
            sort_key(arr, arr_keys, n);
            print_with_key(arr, arr_keys, n);
            break;
        case 6:
            print(arr, n);
            break;
        case 7:
            code_exit = find(arr, n);
            break;
        case 8:
            rewrite(arr, n);
            break;
        case 9:
            diff(arr, arr_keys, n);
            break;
        }
        printf("\n");
    } while(code_exit == SUCCESS && choice != 0);

    if (code_exit == INPUT_ERROR)
        printf("Некорректное значение\n");
    return code_exit;
}

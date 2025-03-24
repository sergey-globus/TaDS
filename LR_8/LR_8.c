#include <stdio.h>
#include <stdlib.h>

#define MATRIX_FILE "./mtr_1.txt"

#define SUCCESS 0

#define INF 99999

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

int *input(int *n)
{
    int *data;
    FILE *f = fopen(MATRIX_FILE, "r");
    fscanf(f, "%d", n);
    data = malloc((*n) * (*n) * sizeof(int));

    int s;
    node_t *cur;
    for (int i = 0; i < *n; i++)
    {
        for (int j = 0; j < *n; j++)
        {
            fscanf(f, "%d", &s);
            if (s >= 0)
            {
                data[i * (*n) + j] = s;
            }
        }
    }
    return data;
}

void generate_dot(int *beg, int n, FILE *dotFile)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (beg[i * n + j] > 0)
            fprintf(dotFile, "%d->%d [label=%d];\n", i + 1, j + 1, beg[i * n + j]);
}

void print(int *beg, int n)
{
    FILE* dotFile = fopen("./graph.dot", "w");
    fprintf(dotFile, "digraph test {\n");
    generate_dot(beg, n, dotFile);
    fprintf(dotFile, "}\n");
    fclose(dotFile);

    system("dot -Tpng ./graph.dot -o ./graph.png");
}

int *min_s(int *beg, int n)
{
    int *data = malloc(n * n * sizeof(int));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            if (i != j && beg[i * n + j] == 0)
                data[i * n + j] = INF;
            else
                data[i * n + j] = beg[i * n + j];
        }

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n ; j++)
                if (data[i * n + k] + data[k * n + j] < data[i * n + j])
                    data[i * n + j] = data[i * n + k] + data[k * n + j];
    return data;
}



int main()
{
    int *beg;
    int n = 0;

    beg = input(&n);
    print(beg, n);

    int *mtr = min_s(beg, n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n ; j++)
        {
            if (mtr[i * n + j] == INF)
                printf("INF");
            else
                printf("%d ", mtr[i * n + j]);
        }
        printf("\n");
    }

    return SUCCESS;
}

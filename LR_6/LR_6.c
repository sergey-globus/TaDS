#include <stdio.h>
#include <stdlib.h>

#define file "./in_1.txt"

#define SUCCESS 0
#define FILE_ERROR 1
#define MEM_ERROR 2
#define EMPTY_ERROR 3
#define INPUT_ERROR 4

typedef struct node node_t;

struct node
{
    int num;
    node_t *left;
    node_t *right;
};

node_t *create(int num)
{
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node)
    {
        new_node->num = num;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}

node_t *insert(node_t *tree, node_t *new_node)
{
    if (tree == NULL)
        return new_node;

    if (new_node->num < tree->num)
        tree->left = insert(tree->left, new_node);
    else if (new_node->num > tree->num)
        tree->right = insert(tree->right, new_node);

    return tree;
}

int input(FILE *f, node_t **tree)
{
    int num;
    node_t *cur;
    while (fscanf(f, "%d", &num) == 1)
    {
        cur = create(num);
        if (cur == NULL)
            return MEM_ERROR;
        *tree = insert(*tree, cur);
    }

    if (*tree == NULL)
        return EMPTY_ERROR;

    return SUCCESS;
}

void generateDotFile(struct node* tree, FILE* dotFile)
{
    if (tree != NULL)
    {
        if (tree->left != NULL)
        {
            fprintf(dotFile, "%d -- %d;\n", tree->num, tree->left->num);
            generateDotFile(tree->left, dotFile);
        }

        if (tree->right != NULL)
        {
            fprintf(dotFile, "%d -- %d;\n", tree->num, tree->right->num);
            generateDotFile(tree->right, dotFile);
        }
    }
}

void print(node_t *tree)
{
    FILE* dotFile = fopen("./binary_tree.dot", "w");
    fprintf(dotFile, "graph Binary_Tree {\n");
    generateDotFile(tree, dotFile);
    fprintf(dotFile, "}\n");
    fclose(dotFile);

    system("dot -Tpng ./binary_tree.dot -o ./binary_tree.png");
}

node_t *find(node_t *tree, int num, int *deep, int *num_compare)
{
    (*deep)++;
    (*num_compare)++;

    if (tree == NULL)
        return NULL;

    if (num == tree->num)
        return tree;
    else if (num < tree->num)
        tree = find(tree->left, num, deep, num_compare);
    else
        tree = find(tree->right, num, deep, num_compare);
}

int main()
{
    int code_exit = SUCCESS;
    node_t *tree;
    int deep = 0;
    int num_compare = 0;
    int num;

    FILE *f = fopen(file, "r");
    if (f == NULL)
        code_exit = FILE_ERROR;

    if (code_exit == SUCCESS)
        code_exit = input(f, &tree);

    if (code_exit == SUCCESS)
        print(tree);

    if (code_exit == SUCCESS)
    {
        printf("Введите значение: ");
        if (scanf("%d", &num) != 1)
            code_exit = INPUT_ERROR;
    }

    if (code_exit == SUCCESS)
    {
        node_t *cur = find(tree, num, &deep, &num_compare);
        if (cur == NULL)
            printf("Значение не найдено, количество сравнений - %d\n", num_compare - 1);
        else
            printf("Глубина расположения - %d\n", deep - 1);
    }

    return code_exit;
}

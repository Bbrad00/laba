#include <stdio.h>
#include <malloc.h>
#include <Windows.h>
#include <iostream>

using namespace std;

struct Grid
{
    char* Info;
    int Key;
    Grid* left;
    Grid* right;
};

struct Tree
{
    Grid field;           // поле данных
    Tree* left;  // левый потомок
    Tree* right; // правый потомок
};
void CheckInp(int* number)
{
    while (!scanf_s("%d", number))
    {
        rewind(stdin);
        cout << "Неверный формат ввода, попробуйте снова: " << endl;
    };
}

void CheckInpd(double* number)
{
    while (!scanf_s("%lf", number))
    {
        rewind(stdin);
        cout << "Неверный формат ввода, попробуйте снова: " << endl;
    };
}

char* StringInp(char* String)
{
    rewind(stdin);
    int i = 0, n = 1;
    String = 0;
    do
    {
        String = (char*)realloc(String, n++ * sizeof(char));
        String[i] = getchar();
        i++;
    } while (String[i - 1] != '\n');
    String[i - 1] = 0;
    return String;
}

void Sort(Grid* elems, int size)
{
    int counter;
    Grid arrt;
    for (int i = 1; i < size; i++)
    {
        arrt = elems[i];
        counter = i - 1;
        while (counter >= 0 && elems[counter].Key > arrt.Key)
        {
            elems[counter + 1] = elems[counter];
            elems[counter] = arrt;
            counter--;
        }
    }
}

Tree* AddNode(Grid x, Tree* root)
{
    Tree* tree = root;
    if (tree == NULL)
    {
        tree = (Tree*)calloc(1, sizeof(Tree));
        tree->field = x;
    }
    else  if (x.Key < tree->field.Key)
        tree->left = AddNode(x, tree->left);
    else
        tree->right = AddNode(x, tree->right);
    return tree;
}

void printTree(Tree* root, int depth = 0) {
    if (root == nullptr) {
        return;
    }

    // Выводим правую ветвь
    printTree(root->right, depth + 1);

    // Выводим узел
    for (int i = 0; i < depth; i++) {
        std::cout << "    ";
    }
    std::cout << root->field.Key << std::endl;

    // Выводим левую ветвь
    printTree(root->left, depth + 1);
}

void Find(Tree* tree, int Key)
{
    if (tree != NULL)
    {
        if (tree->field.Key == Key)
        {
            cout << tree->field.Info << endl;
            cout << tree->field.Key << endl;
        }
        Find(tree->left, Key);
        Find(tree->right, Key);
    }
}

int countLeftNodes(Tree* root) {
    if (root == NULL) { // если корень пустой, то количество равно 0
        return 0;
    }
    if (root->left == NULL) { // если нет левого потомка, то количество равно 0
        return 0;
    }
    return 1 + countLeftNodes(root->left) + countLeftNodes(root->left->right); // возвращаем сумму узлов в левом поддереве и его правом поддереве
}



int MaxValue(Tree* node)
{
    if (node == NULL)
        return INT_MIN;

    int right = MaxValue(node->right);
    int left = MaxValue(node->left);

    if (node->field.Key > left)
        if (node->field.Key > right)
            return node->field.Key;
        else
            return right;
    else if (left < right)
        return left;
    else
        return right;
}


int findMin(Tree* root)
{
    if (root == NULL)
        return INT_MAX;

    int res = root->field.Key;
    int lres = findMin(root->left);
    int rres = findMin(root->right);
    if (lres < res)
        res = lres;
    if (rres < res)
        res = rres;
    return res;
}


void TreePrintIn(Tree* tree)
{
    if (tree != NULL)
    {
        TreePrintIn(tree->left);
        cout << tree->field.Info << endl;
        cout << tree->field.Key << endl;
        TreePrintIn(tree->right);
    }
}

void TreePrintPost(Tree* tree)
{
    if (tree != NULL)
    {
        TreePrintPost(tree->left);
        TreePrintPost(tree->right);
        cout << tree->field.Info << endl;
        cout << tree->field.Key << endl;
    }
}

void Balance(Tree** tree, int first, int last, Grid* elems)
{
    if (first == last)
    {
        *tree = NULL;
        return;
    }
    else
    {
        int m = (first + last) / 2;
        *tree = new Tree;
        (*tree)->field = elems[m];
        Balance(&(*tree)->left, first, m, elems);
        Balance(&(*tree)->right, m + 1, last, elems);
    }
}

void freemem(Tree* tree) {
    if (tree != NULL) {
        freemem(tree->left);
        freemem(tree->right);
        delete tree;
    }
}


Tree* Delete(Tree* root, int key)
{
    Tree* Del, * Prev_Del, * R, * Prev_R;
    // Del, Prev_Del – удаляемый узел и его предыдущий (предок); 
    // R, Prev_R – элемент, на который заменяется удаленный узел, и его предок;
    Del = root;
    Prev_Del = NULL;
    //-------- Поиск удаляемого элемента и его предка по ключу key ---------
    while (Del != NULL && Del->field.Key != key)
    {
        Prev_Del = Del;
        if (Del->field.Key > key) Del = Del->left;
        else Del = Del->right;
    }
    if (Del == NULL)
    { // Элемент не найден
        cout << "Нет ключа!\n" << endl;
        return root;
    }
    //-------------------- Поиск элемента R для замены --------------------------------
    if (Del->right == NULL)
        R = Del->left;
    else
        if (Del->left == NULL)
            R = Del->right;
        else
        {
            //---------------- Ищем самый правый узел в левом поддереве -----------------
            Prev_R = Del;
            R = Del->left;
            while (R->right != NULL)
            {
                Prev_R = R;
                R = R->right;
            }
            //----------- Нашли элемент для замены R и его предка Prev_R -------------
            if (Prev_R == Del)
                R->right = Del->right;
            else
            {
                R->right = Del->right;
                Prev_R->right = R->left;
                R->left = Prev_R;
            }
        }
    if (Del == root)
        root = R; // Удаляя корень, заменяем его на R
    else
        //------- Поддерево R присоединяем к предку удаляемого узла -----------
        if (Del->field.Key < Prev_Del->field.Key)
            Prev_Del->left = R; // На левую ветвь
        else
            Prev_Del->right = R; // На правую ветвь
    delete Del;
    return root;
}

void Task(Tree* root)
{
    Tree* MAXT, * MINT;
    MAXT = root;
    MINT = root;

    int min = findMin(root);
    int max = MaxValue(root);

    while (MAXT != NULL && MAXT->field.Key != max)
    {
        if (MAXT->field.Key > max) MAXT = MAXT->left;
        else MAXT = MAXT->right;
    }

    while (MINT != NULL && MINT->field.Key != min)
    {
        if (MINT->field.Key > min) MINT = MINT->left;
        else MINT = MINT->right;
    }
    cout << "Максимальное значение:" << MAXT->field.Info << " " << MAXT->field.Key << endl;
    cout << "Минимальное значение:" << MINT->field.Info << " " << MINT->field.Key << endl;

    char* stemp = nullptr;
    int temp = 0, choice;
    //-------------------- Поиск элемента R для замены --------------------------------
    if (MAXT->field.Key == max && MINT->field.Key == min) {
        stemp = MAXT->field.Info;
        temp = MAXT->field.Key;
        MAXT->field.Info = MINT->field.Info;
        MAXT->field.Key = MINT->field.Key;
        MINT->field.Info = stemp;
        MINT->field.Key = temp;
    }

    cout << "1.Префиксная\n2.Инфиксная\n3.Постфиксная\n" << endl;
    CheckInp(&choice);
    switch (choice)
    {
    case 1:
    {
        if (root)
            TreePrintPost(root);
        else
            cout << "Дерево пустое\n" << endl;
        break;
    }
    case 2:
    {
        if (root)
            TreePrintIn(root);
        else
            cout << "Дерево пустое\n" << endl; }


    delete  MAXT, MINT, stemp;
    }
}

int main() {
    Tree* root = 0;
    Grid* StringGrid = 0;
    int amount, choice, a = 1;
    setlocale(LC_ALL, "RUS");
    while (a)
    {
        cout << "1.Создать\n2.Добавить\n3.Просмотреть\n4.Сбалансировать\n5.Найти по ключу\n6. Определить количество записей в левой ветви дерева.\n7.Удалить по ключу\n8.Выход\n" << endl;
        cout << "Число: " << endl;
        CheckInp(&a);
        switch (a)
        {
        case 1:
        {
            if (!root)
            {
                cout << "Введите кол-во элементов: \n" << endl;
                CheckInp(&amount);
                StringGrid = (Grid*)calloc(amount, sizeof(Grid));
                for (int i = 0; i < amount; i++)
                {
                    cout << "ФИО: " << endl;
                    StringGrid[i].Info = StringInp(StringGrid[i].Info);
                    cout << "Номер паспорта: " << endl;
                    CheckInp(&StringGrid[i].Key);
                }
                for (int i = 0; i < amount; i++)
                    root = AddNode(StringGrid[i], root);
            }
            else
                cout << "Дерево уже создано\n" << endl;
            break;
        }
        case 2:
        {
            Grid* Info = new Grid;
            cout << "ФИО: " << endl;
            Info->Info = StringInp(Info->Info);
            cout << "Номер паспорта: " << endl;
            rewind(stdin);
            CheckInp(&Info->Key);
            root = AddNode(*Info, root);
            amount++;
            break;
        }
        case 3:
        {
            cout << "\n";
            cout << "\n";
            printTree(root);
            break;
            cout << "\n";
            cout << "\n";
        }
        case 4:
        {
            Sort(StringGrid, amount);
            Balance(&root, 0, amount, StringGrid);
            break;
        }
        case 5:
        {
            if (root)
            {
                int key;
                cout << "Ключ: " << endl;
                CheckInp(&key);
                Find(root, key);
            }
            else
                cout << "Дерево пустое\n" << endl;
            break;
        }
        case 6:
        {
            int u = countLeftNodes(root);
            cout << u << endl;
            break;
        }
        case 7:
        {
            int key;
            if (root)
            {
                cout << "Ключ: " << endl;
                CheckInp(&key);
                root = Delete(root, key);
                amount--;
            }
            else
                cout << "Дерево пустое\n" << endl;
            break;
        }
        case 8:
        {
            freemem(root);
            return 0;
        }

        }
    }
}






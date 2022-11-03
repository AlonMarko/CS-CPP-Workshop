//
// Created by alonn on 5/22/2020.
//
#include "RBTree.h"
#include <stdlib.h>
#include <string.h>


/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *redBlackTree;
    redBlackTree = (RBTree *) malloc(sizeof(RBTree));
    if (redBlackTree == NULL)
    {
        return NULL;
    }
    redBlackTree->root = NULL;
    redBlackTree->compFunc = compFunc;
    redBlackTree->freeFunc = freeFunc;
    redBlackTree->size = 0;
    return redBlackTree;
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
    if (tree == NULL)
    {
        return 0;
    }
    Node *temp = tree->root;
    int res = 0;
    while (temp != NULL)
    {
        res = tree->compFunc(data, temp->data);
        if (res == 0)
        {
            return 1;
        }
        else if (res < 0)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    return 0;
}

/**
 * @brief gets the parent
 * @param node node
 * @return NULL or the parent node
 */
static Node *getParent(Node *node)
{
    return node == NULL ? NULL : node->parent;
}

/**
 * @brief gets the other son  node - can accept nulls
 * @param node the node
 * @return othen son node or NULL if theres no other son
 */
static Node *secondSon(Node *son, Node *parent)
{
    if (son != parent->right)
    {
        return parent->right;
    }
    return parent->left;
}

/**
 * @brief gets the brother of the cur node
 * @param node the cur node
 * @return the brother node or NULL
 */
static Node *getBrother(Node *node)
{
    Node *temp = getParent(node);
    if (temp == NULL)
    {
        return NULL;
    }
    if (node == temp->left)
    {
        return temp->right;
    }
    return temp->left;
}

/**
 * @brief gets the uncle NODE
 * @param node the node to get its uncle
 * @return the uncle node or NULL
 */
static Node *getUncle(Node *node)
{
    Node *temp = getParent(node);
    return getBrother(temp);
}

/**
 * @brief right rotate function - performs a rotation around teh given node, cannot rotate right
 * if left child is RBNULL
 * @param tree tree object
 * @param node node to rotate
 */
static void rotateRight(RBTree *tree, Node *node)
{
    Node *temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL)
    {
        temp->right->parent = node;
    }
    temp->parent = node->parent;
    if (node->parent == NULL)
    {
        tree->root = temp;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = temp;
    }
    else
    {
        node->parent->left = temp;
    }
    temp->right = node;
    node->parent = temp;
}

/**
 * @brief rotates left around the given node - cannot rotate left if the right child is null
 * @param tree tree struct
 * @param node node to rotate around
 */
static void rotateLeft(RBTree *tree, Node *node)
{
    Node *temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL)
    {
        temp->left->parent = node;
    }
    temp->parent = node->parent;
    if (node->parent == NULL)
    {
        tree->root = temp;
    }
    else
    {
        if (node == node->parent->left)
        {
            node->parent->left = temp;
        }
        else
        {
            node->parent->right = temp;
        }
    }
    temp->left = node;
    node->parent = temp;
}

/**
 * @brief function that returns the minimum node in the tree
 * @param root the root of the tree - can also be a node in teh tree so we will get back the
 * minimum in the current sub-tree.
 * @return the minimum node
 */
static Node *findMin(Node *root)
{
    Node *current = root;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;
}

static Node *find(RBTree *tree, const void *data)
{
    Node *temp = tree->root;
    while (temp != NULL)
    {
        if (tree->compFunc(data, temp->data) == 0)
        {
            return temp;
        }
        if (tree->compFunc(data, temp->data) < 0)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    return NULL;
} // cannot reach end without finding nothing since we know the value is in the tree but we still
// return NULL just for coding.

/**
 * @brief function to get successor node in the tree.
 * @param node the node we want to get successor of.
 * @return the successor node
 */
static Node *successor(Node *node)
{
    if (node->right != NULL)
    {
        return findMin(node->right);
    }
    Node *current = node->parent;
    while (current != NULL && node == current->right)
    {
        node = current;
        current = current->parent;
    }
    return current;
}

/**
 * @brief function that gets a node that needs to be fixed and fixed the whole tree accordingly.
 * if the node is the root it colors it black.
 * @param tree tree struct
 * @param node node that was added and has to be fixed.
 */
static void insertFix(RBTree *tree, Node *node)
{
    if (node->parent == NULL)// case 1
    {
        node->color = BLACK;
        return;
    }
    if (node->parent->color == BLACK)// case 2
    {
        return;
    }
    Node *uncleNode = getUncle(node);
    Color uncle = uncleNode ? uncleNode->color : BLACK;
    if (node->parent->color == RED && uncle == RED) // case 3
    {
        node->parent->color = BLACK;
        uncleNode->color = BLACK; // cannot be NULL since we enter if red
        node->parent->parent->color = RED;
        insertFix(tree, node->parent->parent);
    }
    else if (node->parent->color == RED && uncle == BLACK) // case 4
    {
        if (node == node->parent->right && node->parent == node->parent->parent->left)
        {
            rotateLeft(tree, node->parent);
            rotateRight(tree, node->parent);
            node->color = BLACK;
            node->right->color = RED;
            return;
        }
        else if (node == node->parent->left && node->parent == node->parent->parent->right)
        {
            rotateRight(tree, node->parent);
            rotateLeft(tree, node->parent);
            node->color = BLACK;
            node->left->color = RED;
            return;
        }
        if (node == node->parent->left && node->parent == node->parent->parent->left)
        {
            rotateRight(tree, node->parent->parent);
            node->parent->color = BLACK;
            node->parent->right->color = RED;
        }
        else
        {
            rotateLeft(tree, node->parent->parent);
            node->parent->color = BLACK;
            node->parent->left->color = RED;
        }
    }
}


/**
 * @brief function that creates a new tree node to be added and allocates its memory.
 * @param data to be added
 * @return NULL if failed or pointer to the node else.
 */
Node *createNode(void *data)
{
    Node *node = NULL;
    node = (Node *) malloc(sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->data = data;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->color = RED;
    return node;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL)
    {
        return 0;
    }
    if (data == NULL || RBTreeContains(tree, data) != 0)
    {
        return 0;
    }
    Node *toAdd = NULL;
    toAdd = createNode(data);
    if (toAdd == NULL)
    {
        return 0;
    }
    Node *temp = tree->root;
    Node *temp2 = NULL;
    while (temp != NULL)
    {
        temp2 = temp;
        if (tree->compFunc(data, temp->data) < 0)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    toAdd->parent = temp2;
    if (temp2 == NULL)
    {
        tree->root = toAdd;
    }
    else
    {
        if (tree->compFunc(toAdd->data, temp2->data) < 0)
        {
            temp2->left = toAdd;
        }
        else
        {
            temp2->right = toAdd;
        }
    }
    tree->size++;
    insertFix(tree, toAdd);
    return 1;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL || func == NULL)
    {
        return 0;
    }
    if (tree->root == NULL)
    {
        return 1;
    }
    Node *current = findMin(tree->root);
    if (func(current->data, args) == 0)
    {
        return 0;
    }
    while (successor(current) != NULL)
    {
        current = successor(current);
        if (func(current->data, args) == 0)
        {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief small function to switch nodes
 * @param one node
 * @param switchCase second node
 */
static void nodeSwitch(Node *node, Node *switchCase)
{
    if (switchCase != NULL)
    {
        switchCase->parent = node->parent;
    }
    if (node->parent == NULL)
    {
    }
    else if (node == node->parent->left)
    {
        node->parent->left = switchCase;
    }
    else
    {
        node->parent->right = switchCase;
    }
    free(node);
}


static void deleteFix3(RBTree *tree, Node *node, Node *son)
{
    if (node == NULL) // 3a
    {
        return;
    }
    Node *bro = secondSon(son, node);
    Color broLeft = bro->left ? bro->left->color : BLACK;
    Color broRight = bro->right ? bro->right->color : BLACK;
    Node *leftSon = bro->left ? bro->left : NULL;
    Node *rightSon = bro->right ? bro->right : NULL;
    if (bro->color == BLACK && broLeft == BLACK && broRight == BLACK)
    {
        if (node->color == RED) // 3b1
        {
            node->color = BLACK;
            bro->color = RED;
            return;
        }
        else // 3b2
        {
            bro->color = RED;
            deleteFix3(tree, node->parent, node);
        }
    }
    else if (bro->color == RED) //3c
    {
        bro->color = BLACK;
        node->color = RED;
        Node *db = getBrother(bro);
        Node *retainer = node;
        if (node->left == db)
        {
            rotateLeft(tree, node);
        }
        else
        {
            rotateRight(tree, node);
        }
        deleteFix3(tree, retainer, son);
    }
    else if (bro->color == BLACK) //3d
    {
        if (bro == node->right)
        {
            if (broLeft == RED && broRight == BLACK)
            {
                leftSon->color = BLACK;
                bro->color = RED;
                Node *temp = bro;
                bro = leftSon;
                rotateRight(tree, temp);
            }
        }
        else if (bro == node->left)
        {
            if (broLeft == BLACK && broRight == RED)
            {
                rightSon->color = BLACK;
                bro->color = RED;
                Node *temp = bro;
                bro = rightSon;
                rotateLeft(tree, temp);
            }
        }
    }
    if (bro->color == BLACK) //3hey
    {
        broLeft = bro->left ? bro->left->color : BLACK;
        broRight = bro->right ? bro->right->color : BLACK;
        leftSon = bro->left ? bro->left : NULL;
        rightSon = bro->right ? bro->right : NULL;
        if (bro == node->right && broRight == RED)
        {
            Color temp = bro->color;
            bro->color = node->color;
            node->color = temp;
            rightSon->color = BLACK;
            rotateLeft(tree, node);
        }
        else if (bro == node->left && broLeft == RED)
        {
            Color temp = bro->color;
            bro->color = node->color;
            node->color = temp;
            leftSon->color = BLACK;
            rotateRight(tree, node);
        }
        return;
    }
}

/**
 * @brief deletes the node and puts DB but not really since it can be NULL so we send the parent
 * forward
 * @param tree
 * @param node
 */
static void subDeleteFix3(RBTree *tree, Node *node)
{
    Node *parent = NULL;
    if (node->parent != NULL)
    {
        parent = node->parent;
        if (node == node->parent->left)
        {
            node->parent->left = NULL;
        }
        else
        {
            node->parent->right = NULL;
        }
        tree->freeFunc(node->data);
        free(node);
        tree->size--;
    }
    else
    {
        tree->root = NULL;
        tree->freeFunc(node->data);
        free(node);
        node = NULL;
        tree->size--;
    }
    deleteFix3(tree, parent, NULL);
}

/**
 * @brief second delete case if its not that than we have DB so it calls the next
 * @param tree
 * @param node
 */
static void deleteFix2(RBTree *tree, Node *node)
{
    if (node->color == BLACK)
    {
        if ((node->right != NULL) && (node->right->color == RED))
        {
            if (tree->root == node)
            {
                tree->root = node->right;
            }
            tree->freeFunc(node->data);
            node->right->color = BLACK;
            nodeSwitch(node, node->right);
            tree->size--;
            return;
        }
        else if ((node->left != NULL) && (node->left->color == RED))
        {
            if (tree->root == node)
            {
                tree->root = node->left;
            }
            tree->freeFunc(node->data);
            node->left->color = BLACK;
            nodeSwitch(node, node->left);
            tree->size--;
            return;
        }
    }
    subDeleteFix3(tree, node);
}

/**
 * @brief teh first delete case besides being only a root if this is not the case it calls the next
 * @param tree tree struct
 * @param node the node to be deleted
 */
static void deleteFix1(RBTree *tree, Node *node)
{
    if (node->color == RED)
    {
        if (node == node->parent->right)
        {
            node->parent->right = NULL;
        }
        else
        {
            node->parent->left = NULL;
        }
        tree->freeFunc(node->data);
        free(node);
        tree->size--;
        return;
    }
    deleteFix2(tree, node);
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
    if (data == NULL || RBTreeContains(tree, data) == 0)
    {
        return 0;
    }
    Node *toRemove = find(tree, data);
    if (toRemove->right == NULL && toRemove->left == NULL && toRemove == tree->root)
    {
        tree->freeFunc(toRemove->data);
        free(toRemove);
        tree->root = NULL;
        tree->size--;
        return 1;
    }
    Node *succ = NULL;
    if (toRemove->right && toRemove->left)
    {
        succ = successor(toRemove);
        toRemove->data = succ->data;
        succ->data = data;
    }
    if (succ != NULL)
    {
        deleteFix1(tree, succ);
    }
    else
    {
        deleteFix1(tree, toRemove);
    }
    return 1;
}

/**
 * @brief free recursive helper function to free the nodes and data
 * @param tree object
 * @param root current node to recurse upon
 */
void freeNodes(RBTree **tree, Node **root)
{
    if (*root == NULL)
    {
        return;
    }
    freeNodes(&(*tree), &(*root)->right);
    freeNodes(&(*tree), &(*root)->left);
    (*tree)->freeFunc((*root)->data);
    free(*root);
    *root = NULL;
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
    if (*tree == NULL)
    {
        return;
    }
    freeNodes(tree, &(*tree)->root);
    free(*tree);
}
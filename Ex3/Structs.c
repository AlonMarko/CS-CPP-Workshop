//
// Created by alonn on 5/30/2020.
//

#include "Structs.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    return strcmp((char *) a, (char *) b);
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL)
    {
        return 0;
    }
    strcat((char *) pConcatenated, (char *) word);
    strcat((char *) pConcatenated, "\n");
    return 1;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    free(s);
    s = NULL;
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *aV = (Vector *) a;
    Vector *bV = (Vector *) b;
    int lenA = aV->len, lenB = bV->len, i = 0, j = 0;
    while (i < lenA && j < lenB)
    {
        if (aV->vector[i] < bV->vector[j])
        {
            return -1;
        }
        else if (aV->vector[i] < bV->vector[j])
        {
            return 1;
        }
        else
        {
            j++;
            i++;
        }
    }
    if (lenA < lenB)
    {
        return -1;
    }
    else if (lenA > lenB)
    {
        return 1;
    }
    return 0;
}


void freeV(Vector *vec)
{
    free(vec->vector);
    free(vec);
    vec = NULL;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if (pVector == NULL)
    {
        return;
    }
    freeV((Vector *) pVector);
}


/**
 * @brief gets a vector and returns the norm
 * @param v vector
 * @return norm - double
 */
double findNorm(Vector *v)
{
    double norm = 0;
    if (v->len != 0)
    {
        for (int i = 0; i < v->len; i++)
        {
            norm = norm + pow(v->vector[i], 2);
        }
        norm = fabs((sqrt(norm)));
    }
    return norm;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector that will hold a copy of the data of pVector.
 * @return 1 on success, 0 on failure (if pVector == NULL || pMaxVector==NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL || pMaxVector == NULL)
    {
        return 0;
    }
    double norm1 = 0, norm2 = 0;
    Vector *first = (Vector *) pVector;
    Vector *second = (Vector *) pMaxVector;
    if (first->vector == NULL || second->vector == NULL)
    {
        return 0;
    }
    norm1 = findNorm(first);
    norm2 = findNorm(second);
    if (norm1 <= norm2)
    {
        return 1;
    }
    second->vector = realloc(first->vector, sizeof(double) * first->len);
    second->len = first->len;
    return 1;
}


/**
 * @brief creates a new vector
 * @return  the vector
 */
Vector *createMax()
{
    Vector *max = NULL;
    max = (Vector *) malloc(sizeof(Vector));
    if (max == NULL)
    {
        return NULL;
    }
    max->vector = NULL;
    max->len = 0;
    return max;
}

/**
* This function allocates memory it does not free.
* @param tree a pointer to a tree of Vectors
* @return pointer to a *copy* of the vector that has the largest norm (L2 Norm), NULL on failure.
*/
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree->root == NULL)
    {
        return NULL;
    }
    Vector *maxVec = createMax();
    if (maxVec == NULL)
    {
        return NULL;
    }
    int status = forEachRBTree(tree, (forEachFunc) copyIfNormIsLarger, maxVec);
    if (status == 0)
    {
        freeVector(maxVec);
        maxVec = NULL;
    }
    return maxVec;
}

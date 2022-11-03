//
// Created by alon markovich on 4/28/2020.
// include section:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

/**
 * @brief Disclaimer - Genereal idea of the implementation,
 * im using alot of global variables for this exercise instead of using pointers to pointers.
 * it makes the code easier in my opinion but for the 2d array algorithem ive put variables in
 * the function name and give it the global ones just for readability - to make it portable code
 * so people can understand the function without searching for the global variables if they dont
 * see them.
 * and for the 2d array i used pointer arithmethics instead of double pointers.
 * im also using a global message method to track succes/ failure combined with the return values
 * of the functions.
 */

// define section:
// the number of lines that must be in the input file if its not empty
#define OUTNAME "railway_planner_output.txt"


/**
 * @brief struct for a railway part with the parameters
 * shown below
 */
typedef struct railPart
{
    int length;
    int price;
    char start;
    char end;
} railPart;


//changeable global section:
railPart *gPartsList;
int gPartsListLength = 0;
char *gFileOutPut = "";
int gRailWayLength;
int gPartTypes;
char gPartsCatalouge[515];
char minimal[] = "The minimal price is: ";
char noFile[] = "File doesn't exists.";
char fEmpty[] = "File is empty.";
char usageM[] = "Usage: RailwayPlanner <InputFile>";
char lineInvalid[] = "Invalid input in line: ";


//const global section:
const int gBaseLines = 3;
const int gFirstLine = 1;
const int gSecondLine = 2;
const char *noPrice = "-1";
const char *dot = ".";
const int programOk = 0;
const int programBad = 1;


/**
 * @brief recieves the final exit code from the program and prints the global mesage to it than
 * closes it.
 * @param finalReturn the code 0 or 1
 * @return exit failure or exit success depending on the code - or exit failure if it had a
 * problem opening the file
 */
int filePrint(int finalReturn)
{
    FILE *fptr;
    fptr = fopen(OUTNAME, "w");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }
    fprintf(fptr, "%s", gFileOutPut);
    fclose(fptr);
    if (finalReturn == 0)
    {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/**
 * @brief checks if given filename/path exists
 * @param fileName - name or path to the file
 * @return true if it exists, false otherwise
 */
bool checkExists(char *fileName)
{
    FILE *fileP;
    fileP = fopen(fileName, "r");
    if (fileP != NULL)
    {
        fclose(fileP);
        return true;
    }
    return false;
}
/**
 * @brief checks if a given pointer points to an empty file
 * @param fileP file pointer
 * @return true if empty or false if it is not
 */
bool checkEmpty(FILE *fileP)
{
    int c = fgetc(fileP);
    if (c == EOF)
    {
        fclose(fileP);
        return true;
    }
    ungetc(c, fileP);
    return false;
}
/**
 * @brief checks if the input is valid and if so adds it to the proper place in the parts array
 * @return true of false
 */
bool inputIsValid(char *line, int curIndex)
{
    unsigned long i;
    railPart part;
    char s[1018], e[1018], p[1018], l[1018];
    sscanf(line, "%[^,],%[^,],%[^,],%[^,]", s, e, l, p);
    if ((strlen(s) != 1) || (strlen(e) != 1))
    {
        return false;
    }
    else
    {
        sscanf(s, "%c", &part.start);
        sscanf(e, "%c", &part.end);
        if ((strchr(gPartsCatalouge, part.start) == NULL) ||
            (strchr(gPartsCatalouge, part.end) == NULL))
        {
            return false;

        }
    }
    for (i = 0; i < strlen(l); i++)
    {
        if (isdigit(l[i]) == 0)
        {
            return false;
        }
    }
    for (i = 0; i < strlen(p); i++)
    {
        if (isdigit(p[i]) == 0)
        {
            return false;
        }
    }
    sscanf(l, "%d", &part.length);
    if (part.length <= 0)
    {
        return false;
    }
    sscanf(p, "%d", &part.price);
    if (part.price <= 0)
    {
        return false;
    }
    gPartsList[curIndex] = part;
    return true;
}

/**
 * @brief converts an int to str and adds it to the global message in order to be displayed
 * @param addon int value - either line number of minimum price
 */
void addToMsg(int addon)
{
    char toBeAdded[32];
    sprintf(toBeAdded, "%d", addon);
    strcat(gFileOutPut, toBeAdded);
}
/**
 * @brief thsi fucntions checks the first three lines and adds each line if legit to a global
 * variable accordignly - railway length, part types and array of part types ( with matching length)
 * @param line the input line
 * @param counter which line we are reading
 * @return true if succeeded or false otherwise
 */
bool firstThree(char *line, int counter)
{
    unsigned long i;
    if (counter == gFirstLine || counter == gSecondLine)
    {
        for (i = 0; line[i] != '\0'; i++)
        {
            if (isdigit(line[i]) == 0)
            {
                return false;
            }
        }
        if (counter == gFirstLine)
        {
            sscanf(line, "%d", &gRailWayLength);
        }
        else
        {
            sscanf(line, "%d", &gPartTypes);
            if (gPartTypes == 0)
            {
                return false;
            }
        }
    }
    else
    {
        i = 0;
        char *part = strtok(line, ",");
        while (part != NULL)
        {
            if (strlen(part) > 1)
            {
                return false;
            }
            sscanf(part, "%c", &gPartsCatalouge[i]);
            i++;
            part = strtok(NULL, ",");
        }
        if (strlen(gPartsCatalouge) != (unsigned long) i)
        {
            return false;
        }
    }
    return true;
}


/**
 * @brief this function tries to read input from the file, calls another program to check for
 * valid inputs and adds valid inputs to a global parts array. it also changes the global error
 * message accordingly, if everything is allright the message will be changed to the minimal price
 * format
 * @param fileName name of the file to open and read from
 * @return true if it succeeded in everything or false if there was a malfunction
 */
bool inputRead(FILE *fileP)
{
    int lineCounter = 0, arrCounter = 0, memoryExpand = 5;
    gPartsList = malloc(memoryExpand * sizeof(railPart));
    if (gPartsList == NULL)
    {
        gFileOutPut = "";
        return false;
    }
    char line[1025];
    while (fgets(line, sizeof(line), fileP) != NULL)
    {
        lineCounter++;
        char *newline = strchr(line, '\r');
        char *newliner = strchr(line, '\n');
        if (newline)
        {
            *newline = '\0';
        }
        if (newliner)
        {
            *newliner = '\0';
        }
        if (lineCounter <= gBaseLines)
        {
            if (!firstThree(line, lineCounter))
            {
                gFileOutPut = lineInvalid;
                addToMsg(lineCounter);
                free(gPartsList);
                fclose(fileP);
                return false;
            }
        }
        else
        {
            if (arrCounter >= memoryExpand)
            {
                memoryExpand *= 2;
                railPart *tempP = realloc(gPartsList, memoryExpand * sizeof(railPart));
                if (tempP == NULL)
                {
                    gFileOutPut = "";
                    free(gPartsList);
                    gPartsList = NULL;
                    fclose(fileP);
                    return false;
                }
                gPartsList = tempP;
            }
            if (!inputIsValid(line, arrCounter))
            {
                gFileOutPut = lineInvalid;
                free(gPartsList);
                addToMsg(lineCounter);
                fclose(fileP);
                return false;
            }
            arrCounter++;
        }
    }
    gPartsListLength = arrCounter;
    fclose(fileP);
    gFileOutPut = minimal;
    return true;
}

/**
 * @brief checks for the correct spot in the 2d array if there is a optional price in there and
 * if it is the minimal so far it replaces the minimal value for that part
 * @param parts how many parts
 * @param partsType array of parts (chars)
 * @param partsArr array of structs that represent parts
 * @param l length loop integer
 * @param j parts loop integer
 * @param k railparts array integer
 * @param cPrice the current Price that we check against and replace (int)
 * @param table pointer to start of 2d array
 * @return the minimal price - changed or not.
 */
int
cellMinPrice(int parts, const char *partsType, const railPart *partsArr, int l, int j, int k,
             int cPrice,
             const int *table)
{
    if (partsArr[k].end == partsType[j])
    {
        if ((l - partsArr[k].length == 0) && (partsArr[k].price < cPrice))
        {
            cPrice = partsArr[k].price;
        }
        else if ((l - partsArr[k].length > 0))
        {
            char *cPointer = strchr(partsType, partsArr[k].start);
            int index = (int) (cPointer - partsType);
            int lengthIndex = l - partsArr[k].length;
            if ((*(table + lengthIndex * parts + index) != INT_MAX) && (partsArr[k].length +
                lengthIndex == l) && (partsArr[k].price + *(table + lengthIndex * parts + index)
                < cPrice))
            {
                cPrice = partsArr[k].price + *(table + lengthIndex * parts + index);
            }
        }
    }
    return cPrice;
}

/**
 * @brief creates a 2d array in order to keep all the price costs for building railway up to
 * desired length - uses old results to build ahead
 * look for disclaimer - all are g lobal but for portabilty ive pur them here
 * @param length - length of the railway
 * @param parts - total number of part connections
 * @param partsType - array of part connections
 * @param partsArr - the actual parts we are checking - struct type
 * @param totalparts - the total number of parts we have to loop through
 * @return the minimal price - will be INT-MAX if it cannot be built.
 */
int findMinPrice(int length, int parts, char *partsType, railPart *partsArr, int totalparts)
{
    if (length == 0)
    {
        return 0;
    }
    int l, j, k, min = INT_MAX, cPrice = 0;
    length = length + 1;
    int *table = (int *) malloc(length * parts * sizeof(int));
    for (l = 0; l < length; l++)
    {
        for (j = 0; j < parts; j++)
        {
            if (l == 0)
            {
                *(table + l * parts + j) = 0;
            }
            else
            {
                *(table + l * parts + j) = INT_MAX;
            }
        }
    }
    for (l = 1; l < length; l++)
    {
        for (j = 0; j < parts; j++)
        {
            cPrice = INT_MAX;
            for (k = 0; k < totalparts; k++)
            {
                cPrice = cellMinPrice(parts, partsType, partsArr, l, j, k, cPrice, table);
            }
            if (cPrice != INT_MAX)
            {
                *(table + l * parts + j) = cPrice;
            }
        }
    }
    for (j = 0; j < parts; j++)
    {
        if (*(table + (length - 1) * parts + j) < min)
        {
            min = *(table + (length - 1) * parts + j);
        }
    }
    if (min < 0 || min == INT_MAX)
    {
        min = -1;
    }
    free(table);
    return min;
}


/**
 * @brief the main function that combines all - checks for CLI correctness and executes the program
 * opens the output file and prints to it if needed
 * @param argc - how many arguments we got
 * @param argv - array of arguments
 * @return 0 if success or 1 if failed.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        gFileOutPut = usageM;
        return filePrint(programBad);
    }
    if (checkExists(argv[1]))
    {
        FILE *fileP;
        fileP = fopen(argv[1], "r");
        if (!checkEmpty(fileP))
        {
            if (inputRead(fileP))
            {
                int min = findMinPrice(gRailWayLength, gPartTypes, gPartsCatalouge, gPartsList,
                                       gPartsListLength);
                free(gPartsList);
                if (min == -1)
                {
                    strcat(gFileOutPut, noPrice);
                }
                else
                {
                    addToMsg(min);
                }
                return filePrint(programOk);
            }
            else
            {
                strcat(gFileOutPut, dot);
                return filePrint(programBad);
            }
        }
        else
        {
            gFileOutPut = fEmpty;
            return filePrint(programBad);
        }
    }
    else
    {
        gFileOutPut = noFile;
        return filePrint(programBad);
    }
}



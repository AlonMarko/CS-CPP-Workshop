#include <stdio.h>
#include <string.h>
#include <ctype.h>

int maxAge = 120;
int minAge = 18;
int minGrade = 0;
int maxGrade = 100;
/**
 * @brief struct that represents student and contains
 * his deatils
 */
typedef struct Student
{
    char studentId[43];
    char grade[43];
    char age[43];
    char name[43];
    char country[43];
    char city[43];
} Student;
Student students[5500];

/**
 * @brief prints student details
 * @param singleStudent a struct that represents a student
 */
void print(Student singleStudent)
{
    printf("%s,%s,%s,%s,%s,%s\n", singleStudent.studentId,
           singleStudent.name, singleStudent.grade, singleStudent.age,
           singleStudent.country,
           singleStudent.city);
}

/**
 * @brief convers from string to int
 * @param arr the string we want to convert
 * @return the integer that has been converted
 */
int convert(char arr[])
{
    int converted;
    sscanf(arr, "%d", &converted);
    return converted;
}

/**
 * @brief prints the corresponding error
 * @param inputLine the line in which the error occured
 * @param errorType the type of error 1-6 is student detail related errors and
 * 7 is not enough parameters given
 * -1 if there is a EOF error
 */
void parameterError(int inputLine, int errorType)
{
    if (errorType == 1)
    {
        printf("ERROR: students id incorrect - must contain 10 numbers and not start with 0\n");
        printf("in line %d\n", inputLine);
    }
    else if (errorType == 2)
    {
        printf("ERROR: name can only contain alphabetic characters, whitespaces or '-'\n");
        printf("in line %d\n", inputLine);
    }
    else if (errorType == 3)
    {
        printf("ERROR: grade not in good format only integers from 0 to 100\n");
        printf("in line %d\n", inputLine);
    }
    else if (errorType == 4)
    {
        printf("ERROR: age has to be 18 to 120\n");
        printf("in line %d\n", inputLine);
    }
    else if (errorType == 5)
    {
        printf("ERROR: country has to include only small/big letters or '-'\n");
        printf("in line %d\n", inputLine);
    }
    else if (errorType == 6)
    {
        printf("ERROR: city has to include only small/big letters or '-'\n");
        printf("in line %d\n", inputLine);
    }
    else if(errorType == 7)
    {
        printf("ERROR: not entered 6 parameters, each parameter has to be divided by "
               ", mark\n");
        printf("in line %d\n", inputLine);
    }
    else
    {
        printf("ERROR: cannot read input\n");
        printf("in line %d\n", inputLine);
    }
}

/**
 * @brief checks if the input corresponds with the ex rules
 * @param student a struct of student
 * @return error type 1-6 if there is parameter error
 * 0 if there is no error
 */
int checkInput(Student student)
{
    long i;
    unsigned long j;
    char c;
    if (strlen(student.studentId) != 10)
    {
        return 1;
    }
    else
    {
        if (student.studentId[0] == '0')
        {
            return 1;
        }
        i = 0;
        while (student.studentId[i] >= '0' && student.studentId[i] <= '9' && i < 10)
        {
            i++;
        }
        if (i != 10)
        {
            return 1;
        }
    }
    for (i = 0; student.name[i] != '\0'; i++)
    {
        c = student.name[i];
        if ((isalpha(c) == 0 && islower(c) == 0 && c != '-' && isspace(c) == 0))
        {
            return 2;
        }
    }
    if (strlen(student.grade) < 4)
    {
        j = 0;
        while (student.grade[j] >= '0' && student.grade[j] <= '9' && j < strlen(student.grade))
        {
            j++;
        }
        if (j != strlen(student.grade))
        {
            return 3;
        }
        else
        {
            int grade;
            grade = convert(student.grade);
            if (grade < minGrade || grade > maxGrade)
            {
                return 3;
            }
        }
    }
    else
    {
        return 3;
    }
    if (strlen(student.age) >= 2 && strlen(student.age) < 4)
    {
        j = 0;
        while (student.age[j] >= '0' && student.age[j] <= '9' && j < strlen(student.age))
        {
            j++;
        }
        if (j == strlen(student.age))
        {
            int age;
            age = convert(student.age);
            if (age < minAge || age > maxAge)
            {
                return 4;
            }
        }
        else
        {
            return 4;
        }
    }
    else
    {
        return 4;
    }
    for (i = 0; student.country[i] != '\0'; i++)
    {
        c = student.country[i];
        if ((isalpha(c) == 0 && islower(c) == 0 && c != '-'))
        {
            return 5;
        }
    }
    for (i = 0; student.city[i] != '\0'; i++)
    {
        c = student.city[i];
        if ((isalpha(c) == 0 && islower(c) == 0 && c != '-'))
        {
            return 6;
        }
    }
    return 0;
}

/**
 * @brief check wich student is the best and returns its index number
 * @param totalStudents the total number of students in the array
 * @return the index of the best student(the first one)
 */
int bestStudent(int totalStudents)
{
    float current, max = 0;
    int age, grade, i = 0;
    int maxIndex = 0;
    do
    {
        age = convert(students[i].age);
        grade = convert(students[i].grade);
        current = (float) grade / (float) age;
        if (current > max)
        {
            max = current;
            maxIndex = i;
        }
        i++;
    } while (i < totalStudents);
    return maxIndex;
}

/**
 * @brief function that gets the user input, and if it is legal enters it into the students array
 * @return number of students succecfuly entered
 */
int enterS()
{
    char input[350];
    int taken, inputCheck;
    unsigned long last = 0;
    int line = 0;
    int entered = 0;
    do
    {
        Student singleStudent;
        printf("Enter student info. To exit press q, then enter\n");
        if (fgets(input, 350, stdin) == NULL)
        {
            taken = 0;
            entered = -1;
            parameterError(line, -1);
            continue;
        }
        last = strlen(input);
        input[last - 1] = '\0';
        taken = sscanf(input, " %[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", singleStudent.studentId,
                       singleStudent.name, singleStudent.grade, singleStudent.age,
                       singleStudent.country,
                       singleStudent.city);
        if (taken == 6)
        {
            inputCheck = checkInput(singleStudent);
            if (inputCheck != 0)
            {
                parameterError(line, inputCheck);
            }
            else
            {
                students[entered] = singleStudent;
                entered++;
            }
        }
        else
        {
            if (taken == 1)
            {
                if (strcmp(singleStudent.studentId, "q") == 0)
                {
                    taken = 0;
                }
                else
                {
                    inputCheck = 7;
                    parameterError(line, inputCheck);
                }
            }
            else
            {
                inputCheck = 7;
                parameterError(line, inputCheck);
            }
        }
        line++;
    } while (taken != 0);
    return entered;
}

/**
 * @brief merges two sorted sub arrays
 * @param firstStart first sub array index
 * @param firstStop first sub array stop index
 * @param secondStop second sub array stop
 */
void merge(int firstStart, int firstStop, int secondStop)
{
    int index1, index2, index3, grade1, grade2;
    int sub1 = firstStop - firstStart + 1;
    int sub2 = secondStop - firstStop;
    Student arr1[2800], arr2[2800];
    for (index1 = 0; index1 < sub1; index1++)
    {
        arr1[index1] = students[firstStart + index1];
    }
    for (index2 = 0; index2 < sub2; index2++)
    {
        arr2[index2] = students[firstStop + 1 + index2];
    }
    index1 = 0;
    index2 = 0;
    index3 = firstStart;
    while (index1 < sub1 && index2 < sub2)
    {
        grade1 = convert(arr1[index1].grade);
        grade2 = convert(arr2[index2].grade);
        if (grade1 <= grade2)
        {
            students[index3] = arr1[index1];
            index1++;
        }
        else
        {
            students[index3] = arr2[index2];
            index2++;
        }
        index3++;
    }
    while (index1 < sub1)
    {
        students[index3] = arr1[index1];
        index1++;
        index3++;
    }
    while (index2 < sub2)
    {
        students[index3] = arr2[index2];
        index2++;
        index3++;
    }
}

/**
 * @brief sorts the students array
 * @param leftIndex the start index of students in the array
 * @param rightIndex the stop index
 */
void mergeSort(int leftIndex, int rightIndex)
{
    if (leftIndex < rightIndex)
    {
        int newIndex = leftIndex + (rightIndex - leftIndex) / 2;
        mergeSort(leftIndex, newIndex);
        mergeSort(newIndex + 1, rightIndex);
        merge(leftIndex, newIndex, rightIndex);
    }
}

/**
 * @brief swaps 2 students in the students array
 * @param index1 first student index
 * @param index2 second student index
 */
void swap(int index1, int index2)
{
    Student temp;
    temp = students[index1];
    students[index1] = students[index2];
    students[index2] = temp;
}

/**
 * @brief sorts the student array according to first name
 * @param leftIndex start index of the current worked on subarray
 * @param rightIndex end index of the current worked on subarray
 */
void quickSort(int leftIndex, int rightIndex)
{
    int pivotIndex;
    if (rightIndex == 1)
    {
        return;
    }
    pivotIndex = leftIndex;
    for (int i = leftIndex + 1; i < rightIndex; i++)
    {
        if (strcmp(students[i].name, students[leftIndex].name) < 0)
        {
            pivotIndex++;
            swap(i, pivotIndex);
        }
    }
    swap(leftIndex, pivotIndex);
    if (pivotIndex > leftIndex)
    {
        quickSort(leftIndex, pivotIndex);
    }
    if (pivotIndex < rightIndex)
    {
        quickSort(pivotIndex + 1 , rightIndex);
    }

}

/**
 * @brief the main function that combines all other ones
 * @param argc how many parameters are given from cli
 * @param argv the array of parameters
 * @return 0 if finshed or 1 if cli command is wrong
 */
int main(int argc, char *argv[])
{
    if ((argc == 2) && (strcmp(argv[1], "best") == 0 || (strcmp(argv[1], "quick")) == 0 || strcmp
        (argv[1], "merge") == 0))
    {
        int numStudents, relevantIndex;
        numStudents = enterS();
        if (numStudents > 0)
        {
            if (strcmp(argv[1], "best") == 0)
            {
                relevantIndex = bestStudent(numStudents);
                printf("best student info is: ");
                print(students[relevantIndex]);
            }
            else if (strcmp(argv[1], "quick") == 0)
            {
                quickSort(0, numStudents);
                for (int i = 0; i < numStudents; i++)
                {
                    print(students[i]);
                }
            }
            else
            {
                mergeSort(0, numStudents);
                for (int i = 0; i < numStudents; i++)
                {
                    print(students[i]);
                }
            }
        }
        else if (numStudents == -1)
        {
            return 1;
        }
    }
    else
    {
        printf("USAGE: the program has to accept only one argument and it has to be either "
               "'merge', 'quick' or 'best' - the program will terminate\n");
        return 1;
    }
    return 0;
}



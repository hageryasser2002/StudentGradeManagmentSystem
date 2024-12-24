#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 10

#define RED 12
#define GREEN 10
#define YELLOW 14
#define BLUE 9
#define RESET 7

typedef struct Subject Subject;
typedef struct Student Student;


void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

struct Subject {
    char subjectName[50];
    float score;
};

struct Student {
    int id;
    char name[50];
    Subject subjects[MAX_SUBJECTS];
    int subjectCount;
    float average;
    char grade;
};


void addStudent(Student *students, int *count);
void updateStudent(Student *students, int count);
void calculateGrades(Student *students, int count);
void displayResults(Student *students, int count);
void saveDataToFile(Student *students, int count);
void loadDataFromFile(Student *students, int *count);
void displayMenu();


void calculateAverageAndGrade(Student *student);
float calculateClassAverage(Student *students, int count);
Student *findTopPerformer(Student *students, int count);
void displayPassFail(Student *students, int count, int pass);

int main() {
    Student students[MAX_STUDENTS];
    int studentCount = 0;
    int choice;

    loadDataFromFile(students, &studentCount);

    do {
        displayMenu();
        setColor(YELLOW);
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(students, &studentCount);
                break;
            case 2:
                updateStudent(students, studentCount);
                break;
            case 3:
                calculateGrades(students, studentCount);
                break;
            case 4:
                displayResults(students, studentCount);
                break;
            case 0:
                saveDataToFile(students, studentCount);
                setColor(GREEN);
                printf("\nData saved. Exiting...\n");
                break;
            default:
                setColor(RED);
                printf("\nInvalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}



void displayMenu() {
    setColor(BLUE);
    printf("\nStudent Grade Management System\n");
    setColor(GREEN);
    printf("1. Add Student\n");
    printf("2. Update Student\n");
    printf("3. Calculate Grades\n");
    printf("4. Display Results\n");
    printf("0. Exit\n");
}

void addStudent(Student *students, int *count) {
    if (*count >= MAX_STUDENTS) {
        setColor(RED);
        printf("\nMaximum student limit reached.\n");
        return;
    }

    Student *student = &students[*count];
    setColor(RESET);
    printf("\nEnter Student ID: ");
    scanf("%d", &student->id);
    printf("Enter Student Name: ");
    scanf("%s", student->name);
    printf("Enter number of subjects: ");
    scanf("%d", &student->subjectCount);

    int i;
    for (i = 0; i < student->subjectCount; i++) {
        printf("Enter Subject %d Name: ", i + 1);
        scanf("%s", student->subjects[i].subjectName);
        printf("Enter Subject %d Score: ", i + 1);
        scanf("%f", &student->subjects[i].score);
    }

    calculateAverageAndGrade(student);
    (*count)++;
    setColor(GREEN);
    printf("\nStudent added successfully.\n");
}

void updateStudent(Student *students, int count) {
    int id, found = 0;
    setColor(RESET);
    printf("\nEnter Student ID to update: ");
    scanf("%d", &id);

    int i;
    for (i = 0; i < count; i++) {
        if (students[i].id == id) {
            found = 1;
            printf("\nUpdating student: %s\n", students[i].name);
            int j;
            for (j = 0; j < students[i].subjectCount; j++) {
                printf("Enter new score for %s: ", students[i].subjects[j].subjectName);
                scanf("%f", &students[i].subjects[j].score);
            }
            calculateAverageAndGrade(&students[i]);
            setColor(GREEN);
            printf("\nStudent updated successfully.\n");
            break;
        }
    }

    if (!found) {
        setColor(RED);
        printf("\nStudent ID not found.\n");
    }
}

void calculateGrades(Student *students, int count) {
    int i;
    for (i = 0; i < count; i++) {
        calculateAverageAndGrade(&students[i]);
    }
    setColor(GREEN);
    printf("\nGrades calculated successfully.\n");
}

void calculateAverageAndGrade(Student *student) {
    float total = 0;
    int i;
    for (i = 0; i < student->subjectCount; i++) {
        total += student->subjects[i].score;
    }
    student->average = total / student->subjectCount;

    if (student->average >= 90)
        student->grade = 'A';
    else if (student->average >= 80)
        student->grade = 'B';
    else if (student->average >= 70)
        student->grade = 'C';
    else if (student->average >= 60)
        student->grade = 'D';
    else
        student->grade = 'F';
}

void displayResults(Student *students, int count) {
    setColor(BLUE);
    printf("\nAll Students:\n");
    int i;
    for (i = 0; i < count; i++) {
        setColor(RESET);
        printf("ID: %d, Name: %s, Average: %.2f, Grade: %c\n",
               students[i].id, students[i].name, students[i].average, students[i].grade);
    }

    Student *topPerformer = findTopPerformer(students, count);
    if (topPerformer) {
        setColor(GREEN);
        printf("\nTop Performer: %s with Average: %.2f\n", topPerformer->name, topPerformer->average);
    }

    setColor(YELLOW);
    printf("\nClass Average: %.2f\n", calculateClassAverage(students, count));

    setColor(GREEN);
    printf("\nStudents who passed:\n");
    displayPassFail(students, count, 1);

    setColor(RED);
    printf("\nStudents who failed:\n");
    displayPassFail(students, count, 0);
}

float calculateClassAverage(Student *students, int count) {
    float total = 0;
    int i;
    for (i = 0; i < count; i++) {
        total += students[i].average;
    }
    return total / count;
}

Student *findTopPerformer(Student *students, int count) {
    if (count == 0) return NULL;
    Student *top = &students[0];
    int i;
    for (i = 1; i < count; i++) {
        if (students[i].average > top->average) {
            top = &students[i];
        }
    }
    return top;
}

void displayPassFail(Student *students, int count, int pass) {
    int i;
    for (i = 0; i < count; i++) {
        if ((pass && students[i].grade != 'F') || (!pass && students[i].grade == 'F')) {
            setColor(RESET);
            printf("ID: %d, Name: %s, Grade: %c\n",
                   students[i].id, students[i].name, students[i].grade);
        }
    }
}

void saveDataToFile(Student *students, int count) {
    FILE *file = fopen("students.dat", "wb");
    if (file) {
        fwrite(&count, sizeof(int), 1, file);
        fwrite(students, sizeof(Student), count, file);
        fclose(file);
    }
}

void loadDataFromFile(Student *students, int *count) {
    FILE *file = fopen("students.dat", "rb");
    if (file) {
        fread(count, sizeof(int), 1, file);
        fread(students, sizeof(Student), *count, file);
        fclose(file);
    }
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define ATTENDANCE_FILE "attendance.txt"

struct Student {
    int id;
    char name[100];
    char department[50];
    int year;
};

// Function declarations
void addStudent();
void viewStudents();
void markAttendance();
void attendanceReport();
void searchStudent();
int studentExists(int id);

// Check whether student ID already exists
int studentExists(int id) {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;

    if (fp == NULL)
        return 0;

    while (fscanf(fp, "%d|%99[^|]|%49[^|]|%d\n",
                  &s.id, s.name, s.department, &s.year) == 4) {
        if (s.id == id) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// Add a new student
void addStudent() {
    FILE *fp;
    struct Student s;

    printf("\nEnter Student ID: ");
    scanf("%d", &s.id);

    if (studentExists(s.id)) {
        printf("Student ID already exists!\n");
        return;
    }

    getchar(); // Clear newline

    printf("Enter Student Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Department: ");
    fgets(s.department, sizeof(s.department), stdin);
    s.department[strcspn(s.department, "\n")] = '\0';

    printf("Enter Year (1-4): ");
    scanf("%d", &s.year);

    fp = fopen(STUDENT_FILE, "a");

    if (fp == NULL) {
        printf("Error opening student file!\n");
        return;
    }

    fprintf(fp, "%d|%s|%s|%d\n",
            s.id, s.name, s.department, s.year);

    fclose(fp);

    printf("\nStudent added successfully!\n");
}

// Display all students
void viewStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;

    if (fp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\n========== STUDENT LIST ==========\n");
    printf("%-10s %-25s %-15s %-5s\n",
           "ID", "Name", "Department", "Year");
    printf("---------------------------------\n");

    while (fscanf(fp, "%d|%99[^|]|%49[^|]|%d\n",
                  &s.id, s.name, s.department, &s.year) == 4) {
        printf("%-10d %-25s %-15s %-5d\n",
               s.id, s.name, s.department, s.year);
    }

    fclose(fp);
}

// Mark attendance for a particular date
void markAttendance() {
    FILE *studentFp, *attendanceFp;
    struct Student s;
    char date[20];
    char status;

    studentFp = fopen(STUDENT_FILE, "r");

    if (studentFp == NULL) {
        printf("\nNo students available. Add students first.\n");
        return;
    }

    attendanceFp = fopen(ATTENDANCE_FILE, "a");

    if (attendanceFp == NULL) {
        printf("Error opening attendance file!\n");
        fclose(studentFp);
        return;
    }

    printf("\nEnter date (DD-MM-YYYY): ");
    scanf("%19s", date);

    printf("\nMark attendance for %s\n", date);
    printf("P = Present, A = Absent\n");

    while (fscanf(studentFp, "%d|%99[^|]|%49[^|]|%d\n",
                  &s.id, s.name, s.department, &s.year) == 4) {

        printf("\nStudent: %s (ID: %d)\n", s.name, s.id);
        printf("Enter attendance (P/A): ");
        scanf(" %c", &status);

        if (status == 'p' || status == 'P')
            status = 'P';
        else if (status == 'a' || status == 'A')
            status = 'A';
        else {
            printf("Invalid input! Marking as absent.\n");
            status = 'A';
        }

        fprintf(attendanceFp, "%d|%s|%c\n",
                s.id, date, status);
    }

    fclose(studentFp);
    fclose(attendanceFp);

    printf("\nAttendance marked successfully!\n");
}

// Display attendance report for all students
void attendanceReport() {
    FILE *studentFp, *attendanceFp;
    struct Student s;
    int id, present, absent, total;
    char date[20], status;

    studentFp = fopen(STUDENT_FILE, "r");

    if (studentFp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\n========== ATTENDANCE REPORT ==========\n");

    while (fscanf(studentFp, "%d|%99[^|]|%49[^|]|%d\n",
                  &s.id, s.name, s.department, &s.year) == 4) {

        present = 0;
        absent = 0;
        total = 0;

        attendanceFp = fopen(ATTENDANCE_FILE, "r");

        if (attendanceFp == NULL) {
            printf("\nNo attendance records available.\n");
            fclose(studentFp);
            return;
        }

        while (fscanf(attendanceFp, "%d|%19[^|]|%c\n",
                      &id, date, &status) == 3) {

            if (id == s.id) {
                total++;

                if (status == 'P')
                    present++;
                else if (status == 'A')
                    absent++;
            }
        }

        fclose(attendanceFp);

        printf("\nStudent ID   : %d\n", s.id);
        printf("Student Name : %s\n", s.name);
        printf("Present      : %d\n", present);
        printf("Absent       : %d\n", absent);
        printf("Total Classes: %d\n", total);

        if (total > 0) {
            printf("Attendance %% : %.2f%%\n",
                   (present * 100.0) / total);
        } else {
            printf("Attendance %% : No attendance recorded\n");
        }
    }

    fclose(studentFp);
}

// Search student using ID
void searchStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student s;
    int searchID, found = 0;

    if (fp == NULL) {
        printf("\nNo student records found.\n");
        return;
    }

    printf("\nEnter Student ID to search: ");
    scanf("%d", &searchID);

    while (fscanf(fp, "%d|%99[^|]|%49[^|]|%d\n",
                  &s.id, s.name, s.department, &s.year) == 4) {

        if (s.id == searchID) {
            printf("\nStudent Found!\n");
            printf("ID         : %d\n", s.id);
            printf("Name       : %s\n", s.name);
            printf("Department : %s\n", s.department);
            printf("Year       : %d\n", s.year);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nStudent not found!\n");

    fclose(fp);
}

// Main function
int main() {
    int choice;

    do {
        printf("\n====================================\n");
        printf(" STUDENT ATTENDANCE MANAGEMENT SYSTEM\n");
        printf("====================================\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Mark Attendance\n");
        printf("4. Attendance Report\n");
        printf("5. Search Student\n");
        printf("6. Exit\n");
        printf("------------------------------------\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent();
                break;

            case 2:
                viewStudents();
                break;

            case 3:
                markAttendance();
                break;

            case 4:
                attendanceReport();
                break;

            case 5:
                searchStudent();
                break;

            case 6:
                printf("\nThank you for using the system!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}

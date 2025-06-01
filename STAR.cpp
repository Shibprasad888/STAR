#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <limits>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <termios.h>
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
#endif

using namespace std;

class Student;
class STARSystem;

struct Subject {
    string name;
    int marks;
    int totalClasses;
    int attendedClasses;
    
    Subject() : marks(0), totalClasses(0), attendedClasses(0) {}
    Subject(string n) : name(n), marks(0), totalClasses(0), attendedClasses(0) {
        if (name.empty()) name = "Unnamed Subject";
    }
    
    bool setMarks(int m) {
        if (m >= 0 && m <= 100) {
            marks = m;
            return true;
        }
        return false;
    }
    
    bool setAttendance(int total, int attended) {
        if (total >= 0 && attended >= 0 && attended <= total) {
            totalClasses = total;
            attendedClasses = attended;
            return true;
        }
        return false;
    }
    
    double getAttendancePercentage() const {
        return (totalClasses == 0) ? 0.0 : 
               (static_cast<double>(attendedClasses) / totalClasses) * 100.0;
    }
};

class Student {
private:
    string name;
    string studentClass;
    int rollNumber;
    string mobileNumber;
    string address;
    string password;
    vector<Subject> subjects;

public:
    Student() : rollNumber(0), password("123") {}
    
    Student(string n, string cls, int roll, string mobile, string addr, vector<string> subjectNames)
        : name(n), studentClass(cls), rollNumber(roll), mobileNumber(mobile), address(addr), password("123") {
        for (const string& subName : subjectNames) {
            subjects.push_back(Subject(subName));
        }
    }
    
    
    int getRollNumber() const { return rollNumber; }
    string getName() const { return name; }
    string getStudentClass() const { return studentClass; }
    string getMobileNumber() const { return mobileNumber; }
    string getAddress() const { return address; }
    string getPassword() const { return password; }
    vector<Subject>& getSubjects() { return subjects; }
    const vector<Subject>& getSubjects() const { return subjects; }
    
    static bool isValidName(const string& n) {
        return !n.empty() && any_of(n.begin(), n.end(), [](char c) { return !isspace(c); });
    }

    static bool isValidMobile(const string& m) {
        return m.length() == 10 && all_of(m.begin(), m.end(), ::isdigit);
    }

    void setName(const string& n) { 
        if (isValidName(n)) name = n; 
    }

    void setMobileNumber(const string& mobile) { 
        if (isValidMobile(mobile)) mobileNumber = mobile; 
    }

    void setStudentClass(const string& cls) { 
        if (!cls.empty()) studentClass = cls; 
    }
    void setAddress(const string& addr) { address = addr; }
    void setPassword(const string& pass) { password = pass; }
    
    void updateSubjectMarks(const string& subjectName, int marks) {
        for (Subject& sub : subjects) {
            if (sub.name == subjectName) {
                sub.marks = marks;
                break;
            }
        }
    }
    
    void updateSubjectAttendance(const string& subjectName, int total, int attended) {
        for (Subject& sub : subjects) {
            if (sub.name == subjectName) {
                sub.totalClasses = total;
                sub.attendedClasses = attended;
                break;
            }
        }
    }
    
    void displayRecord() const {
        cout << "\n========================================\n";
        cout << "           STUDENT RECORD\n";
        cout << "========================================\n";
        cout << "Name: " << name << endl;
        cout << "Class: " << studentClass << endl;
        cout << "Roll Number: " << rollNumber << endl;
        cout << "Mobile: " << mobileNumber << endl;
        cout << "Address: " << address << endl;
        cout << "\n--- ACADEMIC PERFORMANCE ---\n";
        cout << left << setw(15) << "Subject" << setw(10) << "Marks" << endl;
        cout << "-------------------------\n";
        for (const Subject& sub : subjects) {
            cout << left << setw(15) << sub.name << setw(10) << sub.marks << endl;
        }
        cout << "========================================\n";
    }
    
    void displayAttendance() const {
        cout << "\n========================================\n";
        cout << "         ATTENDANCE RECORD\n";
        cout << "========================================\n";
        cout << "Student: " << name << " (Roll: " << rollNumber << ")\n";
        cout << "Class: " << studentClass << endl;
        cout << "\n--- ATTENDANCE DETAILS ---\n";
        cout << left << setw(15) << "Subject" << setw(12) << "Total" << setw(12) << "Attended" << setw(12) << "Percentage" << endl;
        cout << "---------------------------------------------------\n";
        for (const Subject& sub : subjects) {
            cout << left << setw(15) << sub.name 
                 << setw(12) << sub.totalClasses 
                 << setw(12) << sub.attendedClasses 
                 << setw(12) << fixed << setprecision(2) << sub.getAttendancePercentage() << "%" << endl;
        }
        cout << "========================================\n";
    }
};

class STARSystem {
private:
    vector<Student> students;
    string facultyPassword;

    void clearScreen() {
        system(CLEAR_SCREEN);
    }
    
    void pressAnyKey() {
        cout << "\nPress Enter to continue...";
        #ifdef _WIN32
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        #else
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        #endif
    }
    
    string getHiddenInput() {
        string password;
        #ifdef _WIN32
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            getline(cin, password);
        #else
            struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~ECHO;
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            getline(cin, password);
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            cout << endl;
        #endif
        return password;
    }
    
    int getValidatedInt(const string& prompt) {
        int value;
        string input;
        bool valid = false;
        
        do {
            cout << prompt;
            getline(cin, input);
            
            try {
                size_t pos;
                value = stoi(input, &pos);
                
                if (pos == input.length() && value >= 0) {
                    valid = true;
                } else {
                    cout << "Invalid input! Please enter a positive number.\n";
                }
            } catch (const exception&) {
                cout << "Invalid input! Please enter a valid number.\n";
            }
        } while (!valid);
        
        return value;
    }
    
    Student* findStudentByRoll(int rollNumber) {
        for (Student& student : students) {
            if (student.getRollNumber() == rollNumber) {
                return &student;
            }
        }
        return nullptr;
    }

    bool confirmAction(const string& message) {
        cout << message << " (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return tolower(choice) == 'y';
    }

public:
    STARSystem() : facultyPassword("axid") {}
    
    void displayMainMenu() {
        clearScreen();
        cout << "\n========================================\n";
        cout << "      S.T.A.R SYSTEM - MAIN MENU\n";
        cout << "  Student Tracking & Academic Record\n";
        cout << "========================================\n";
        cout << "1. Faculty Login\n";
        cout << "2. Student Login\n";
        cout << "3. Exit\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";
    }
    
    bool facultyLogin() {
        clearScreen();
        cout << "\n========================================\n";
        cout << "           FACULTY LOGIN\n";
        cout << "========================================\n";
        cout << "Enter Faculty Password: ";
        string inputPassword = getHiddenInput();
        
        if (inputPassword == facultyPassword) {
            cout << "\n✓ Login Successful!\n";
            pressAnyKey();
            return true;
        } else {
            cout << "\n✗ Wrong Password! Access Denied.\n";
            pressAnyKey();
            return false;
        }
    }
    
    void facultyControlPanel() {
        int choice;
        do {
            clearScreen();
            cout << "\n========================================\n";
            cout << "         FACULTY CONTROL PANEL\n";
            cout << "========================================\n";
            cout << "1. Manage Student Records\n";
            cout << "2. Change Password\n";
            cout << "3. Exit to Main Menu\n";
            cout << "========================================\n";
            choice = getValidatedInt("Enter your choice: ");
            
            switch (choice) {
                case 1:
                    manageStudentRecords();
                    break;
                case 2:
                    changeFacultyPassword();
                    break;
                case 3:
                    cout << "\nReturning to Main Menu...\n";
                    pressAnyKey();
                    break;
                default:
                    cout << "\nInvalid choice! Please try again.\n";
                    pressAnyKey();
            }
        } while (choice != 3);
    }
    
    void manageStudentRecords() {
        int choice;
        do {
            clearScreen();
            cout << "\n========================================\n";
            cout << "       MANAGE STUDENT RECORDS\n";
            cout << "========================================\n";
            cout << "1. Add New Student\n";
            cout << "2. Update Student Records\n";
            cout << "3. List All Students\n";
            cout << "4. Back to Faculty Panel\n";
            cout << "========================================\n";
            choice = getValidatedInt("Enter your choice: ");
            
            switch (choice) {
                case 1:
                    addNewStudent();
                    break;
                case 2:
                    updateStudentRecords();
                    break;
                case 3:
                    listAllStudents();
                    break;
                case 4:
                    cout << "\nReturning to Faculty Panel...\n";
                    pressAnyKey();
                    break;
                default:
                    cout << "\nInvalid choice! Please try again.\n";
                    pressAnyKey();
            }
        } while (choice != 4);
    }
    
    void addNewStudent() {
        clearScreen();
        cout << "\n========================================\n";
        cout << "           ADD NEW STUDENT\n";
        cout << "========================================\n";
        
        string name, studentClass, mobile, address;
        int rollNumber;
        
        try {
            cout << "Enter Student Name: ";
            getline(cin, name);
            if (name.empty()) {
                cout << "\n✗ Error: Name cannot be empty!\n";
                pressAnyKey();
                return;
            }
            
            cout << "Enter Class: ";
            getline(cin, studentClass);
            if (studentClass.empty()) {
                cout << "\n✗ Error: Class cannot be empty!\n";
                pressAnyKey();
                return;
            }
            
            rollNumber = getValidatedInt("Enter Roll Number: ");
            if (rollNumber <= 0) {
                cout << "\n✗ Error: Invalid roll number!\n";
                pressAnyKey();
                return;
            }
            
            if (findStudentByRoll(rollNumber) != nullptr) {
                cout << "\n✗ Error: Student with Roll Number " << rollNumber << " already exists!\n";
                pressAnyKey();
                return;
            }
            
            cout << "Enter Mobile Number: ";
            getline(cin, mobile);
            if (mobile.length() != 10 || !all_of(mobile.begin(), mobile.end(), ::isdigit)) {
                cout << "\n✗ Error: Invalid mobile number! Must be 10 digits.\n";
                pressAnyKey();
                return;
            }
            
            cout << "Enter Address: ";
            getline(cin, address);
            if (address.empty()) {
                cout << "\n✗ Error: Address cannot be empty!\n";
                pressAnyKey();
                return;
            }
            
            int numSubjects = getValidatedInt("Enter number of subjects: ");
            vector<string> subjectNames;
            
            cout << "\nEnter subject names:\n";
            cin.ignore();
            for (int i = 0; i < numSubjects; i++) {
                string subjectName;
                cout << "Subject " << (i + 1) << ": ";
                getline(cin, subjectName);
                subjectNames.push_back(subjectName);
            }
            
            Student newStudent(name, studentClass, rollNumber, mobile, address, subjectNames);
            students.push_back(newStudent);
            
            cout << "\n✓ Student added successfully!\n";
            cout << "Default password set to: 123\n";
            pressAnyKey();
        } catch (const exception& e) {
            cout << "\n✗ Error occurred: " << e.what() << "\n";
            pressAnyKey();
            return;
        }
    }
    
    void updateStudentRecords() {
        clearScreen();
        cout << "\n========================================\n";
        cout << "        UPDATE STUDENT RECORDS\n";
        cout << "========================================\n";
        
        int rollNumber = getValidatedInt("Enter Roll Number: ");
        Student* student = findStudentByRoll(rollNumber);
        
        if (student == nullptr) {
            cout << "\n✗ Student with Roll Number " << rollNumber << " not found!\n";
            pressAnyKey();
            return;
        }
        
        int choice;
        do {
            clearScreen();
            cout << "\n========================================\n";
            cout << "    UPDATE RECORDS - " << student->getName() << "\n";
            cout << "========================================\n";
            cout << "1. Update Name\n";
            cout << "2. Update Mobile Number\n";
            cout << "3. Update Address\n";
            cout << "4. Update Subject Marks\n";
            cout << "5. Update Attendance\n";
            cout << "6. Save and Exit\n";
            cout << "7. Discard and Exit\n";
            cout << "========================================\n";
            choice = getValidatedInt("Enter your choice: ");
            
            switch (choice) {
                case 1: {
                    cout << "Current Name: " << student->getName() << endl;
                    cout << "Enter New Name: ";
                    string newName;
                    cin.ignore();
                    getline(cin, newName);
                    student->setName(newName);
                    cout << "✓ Name updated successfully!\n";
                    pressAnyKey();
                    break;
                }
                case 2: {
                    cout << "Current Mobile: " << student->getMobileNumber() << endl;
                    cout << "Enter New Mobile Number: ";
                    string newMobile;
                    cin.ignore();
                    getline(cin, newMobile);
                    student->setMobileNumber(newMobile);
                    cout << "✓ Mobile number updated successfully!\n";
                    pressAnyKey();
                    break;
                }
                case 3: {
                    cout << "Current Address: " << student->getAddress() << endl;
                    cout << "Enter New Address: ";
                    string newAddress;
                    cin.ignore();
                    getline(cin, newAddress);
                    student->setAddress(newAddress);
                    cout << "✓ Address updated successfully!\n";
                    pressAnyKey();
                    break;
                }
                case 4: {
                    updateStudentMarks(*student);
                    break;
                }
                case 5: {
                    updateStudentAttendance(*student);
                    break;
                }
                case 6:
                    cout << "\n✓ All changes saved successfully!\n";
                    pressAnyKey();
                    return;
                case 7:
                    if (confirmAction("\nAre you sure you want to discard all changes?")) {
                        cout << "\n⚠ Changes discarded!\n";
                        pressAnyKey();
                        return;
                    }
                    break;
                default:
                    cout << "\nInvalid choice! Please try again.\n";
                    pressAnyKey();
            }
        } while (choice != 6 && choice != 7);
    }

    void updateStudentMarks(Student& student) {
        clearScreen();
        cout << "\n========================================\n";
        cout << "          UPDATE MARKS\n";
        cout << "========================================\n";
        
        vector<Subject>& subjects = student.getSubjects();
        
        if (subjects.empty()) {
            cout << "No subjects found for this student!\n";
            pressAnyKey();
            return;
        }
        
        cout << "Available subjects:\n";
        for (size_t i = 0; i < subjects.size(); i++) {
            cout << (i + 1) << ". " << subjects[i].name << " (Current marks: " << subjects[i].marks << ")\n";
        }
        
        int subjectChoice = getValidatedInt("\nSelect subject to update (number): ");
        
        if (subjectChoice < 1 || subjectChoice > static_cast<int>(subjects.size())) {
            cout << "Invalid subject selection!\n";
            pressAnyKey();
            return;
        }
        
        int newMarks = getValidatedInt("Enter new marks (0-100): ");
        
        if (newMarks < 0 || newMarks > 100) {
            cout << "Invalid marks! Please enter marks between 0-100.\n";
            pressAnyKey();
            return;
        }
        
        subjects[subjectChoice - 1].marks = newMarks;
        cout << "\n✓ Marks updated successfully!\n";
        pressAnyKey();
    }
    
    
    void updateStudentAttendance(Student& student) {
        clearScreen();
        cout << "\n========================================\n";
        cout << "        UPDATE ATTENDANCE\n";
        cout << "========================================\n";
        
        vector<Subject>& subjects = student.getSubjects();
        
        if (subjects.empty()) {
            cout << "No subjects found for this student!\n";
            pressAnyKey();
            return;
        }
        
        cout << "Available subjects:\n";
        for (size_t i = 0; i < subjects.size(); i++) {
            cout << (i + 1) << ". " << subjects[i].name 
                 << " (Attended: " << subjects[i].attendedClasses 
                 << "/" << subjects[i].totalClasses << ")\n";
        }
        
        int subjectChoice = getValidatedInt("\nSelect subject to update (number): ");
        
        if (subjectChoice < 1 || subjectChoice > static_cast<int>(subjects.size())) {
            cout << "Invalid subject selection!\n";
            pressAnyKey();
            return;
        }
        
        int totalClasses = getValidatedInt("Enter total number of classes: ");
        int attendedClasses = getValidatedInt("Enter attended classes: ");
        
        if (attendedClasses > totalClasses || totalClasses < 0 || attendedClasses < 0) {
            cout << "Invalid attendance data! Attended classes cannot exceed total classes.\n";
            pressAnyKey();
            return;
        }
        
        subjects[subjectChoice - 1].totalClasses = totalClasses;
        subjects[subjectChoice - 1].attendedClasses = attendedClasses;
        
        double percentage = subjects[subjectChoice - 1].getAttendancePercentage();
        cout << "\n✓ Attendance updated successfully!\n";
        cout << "Attendance Percentage: " << fixed << setprecision(2) << percentage << "%\n";
        pressAnyKey();
    }
    
    
    void listAllStudents() {
        clearScreen();
        cout << "\n========================================\n";
        cout << "          ALL STUDENTS LIST\n";
        cout << "========================================\n";
        
        if (students.empty()) {
            cout << "No students found in the system!\n";
            pressAnyKey();
            return;
        }
        
        cout << left << setw(10) << "Roll No." << setw(20) << "Name" << setw(15) << "Class" << setw(15) << "Mobile" << endl;
        cout << "----------------------------------------------------------------\n";
        
        for (const Student& student : students) {
            cout << left << setw(10) << student.getRollNumber() 
                 << setw(20) << student.getName() 
                 << setw(15) << student.getStudentClass() 
                 << setw(15) << student.getMobileNumber() << endl;
        }
        
        cout << "\nTotal Students: " << students.size() << endl;
        pressAnyKey();
    }
    

    void changeFacultyPassword() {
        clearScreen();
        cout << "\n========================================\n";
        cout << "         CHANGE FACULTY PASSWORD\n";
        cout << "========================================\n";
        
        int attempts = 0;
        const int maxAttempts = 3;
        
        while (attempts < maxAttempts) {
            cout << "Enter current password: ";
            string currentPassword = getHiddenInput();
            
            if (currentPassword == facultyPassword) {
                cout << "Enter new password: ";
                string newPassword = getHiddenInput();
                
                cout << "Confirm new password: ";
                string confirmPassword = getHiddenInput();
                
                if (newPassword == confirmPassword) {
                    facultyPassword = newPassword;
                    cout << "\n✓ Password changed successfully!\n";
                    pressAnyKey();
                    return;
                } else {
                    cout << "\n✗ Passwords do not match! Please try again.\n";
                    pressAnyKey();
                    return;
                }
            } else {
                attempts++;
                cout << "\n✗ Wrong current password! Attempts remaining: " << (maxAttempts - attempts) << endl;
                pressAnyKey();
            }
        }
        
        cout << "\n⚠ Warning: Too many failed attempts! Relogin as Faculty.\n";
        pressAnyKey();
    }
    

    void studentLogin() {
        clearScreen();
        cout << "\n========================================\n";
        cout << "           STUDENT LOGIN\n";
        cout << "========================================\n";
        
        int rollNumber = getValidatedInt("Enter Student ID (Roll Number): ");
        Student* student = findStudentByRoll(rollNumber);
        
        if (student == nullptr) {
            cout << "\n✗ Student ID not found!\n";
            pressAnyKey();
            return;
        }
        
        cout << "Enter Password: ";
        string inputPassword = getHiddenInput();
        
        if (inputPassword == student->getPassword()) {
            cout << "\n✓ Login Successful! Welcome " << student->getName() << "!\n";
            pressAnyKey();
            studentPanel(*student);
        } else {
            cout << "\n✗ Wrong Password! Access Denied.\n";
            pressAnyKey();
        }
    }
    
    void studentPanel(Student& student) {
        int choice;
        do {
            clearScreen();
            cout << "\n========================================\n";
            cout << "         STUDENT PANEL\n";
            cout << "    Welcome, " << student.getName() << "!\n";
            cout << "========================================\n";
            cout << "1. View Records\n";
            cout << "2. View Attendance\n";
            cout << "3. Change Password\n";
            cout << "4. Exit to Main Menu\n";
            cout << "========================================\n";
            choice = getValidatedInt("Enter your choice: ");
            
            switch (choice) {
                case 1:
                    clearScreen();
                    student.displayRecord();
                    pressAnyKey();
                    break;
                case 2:
                    clearScreen();
                    student.displayAttendance();
                    pressAnyKey();
                    break;
                case 3:
                    changeStudentPassword(student);
                    break;
                case 4:
                    cout << "\nReturning to Main Menu...\n";
                    pressAnyKey();
                    break;
                default:
                    cout << "\nInvalid choice! Please try again.\n";
                    pressAnyKey();
            }
        } while (choice != 4);
    }
    
    void changeStudentPassword(Student& student) {
        clearScreen();
        cout << "\n========================================\n";
        cout << "         CHANGE PASSWORD\n";
        cout << "========================================\n";
        
        cout << "Enter current password: ";
        string currentPassword = getHiddenInput();
        
        if (currentPassword == student.getPassword()) {
            cout << "Enter new password: ";
            string newPassword = getHiddenInput();
            
            cout << "Confirm new password: ";
            string confirmPassword = getHiddenInput();
            
            if (newPassword == confirmPassword) {
                student.setPassword(newPassword);
                cout << "\n✓ Password changed successfully!\n";
                pressAnyKey();
            } else {
                cout << "\n✗ Passwords do not match! Please try again.\n";
                pressAnyKey();
            }
        } else {
            cout << "\n✗ Wrong current password!\n";
            pressAnyKey();
        }
    }
    
    void run() {
        int choice;
        cout << "\n========================================\n";
        cout << "  Welcome to S.T.A.R System v1.0\n";
        cout << "  Student Tracking & Academic Record\n";
        cout << "========================================\n";
        pressAnyKey();
        
        do {
            displayMainMenu();
            choice = getValidatedInt("");
            
            switch (choice) {
                case 1:
                    if (facultyLogin()) {
                        facultyControlPanel();
                    }
                    break;
                case 2:
                    studentLogin();
                    break;
                case 3:
                    clearScreen();
                    cout << "\n========================================\n";
                    cout << "    Thank you for using S.T.A.R System!\n";
                    cout << "              Goodbye!\n";
                    cout << "========================================\n";
                    break;
                default:
                    cout << "\nInvalid choice! Please select 1, 2, or 3.\n";
                    pressAnyKey();
            }
        } while (choice != 3);
    }
};

int main() {  //This projrct was made by AxiD. Github:Shibprasad888
    STARSystem system;
    system.run();
    return 0; 
}
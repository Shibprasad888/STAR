# S.T.A.R. — Student Tracking & Academic Record

S.T.A.R. is a menu-driven C++ console application for managing student profiles, subject marks, and attendance. Faculty can manage records, while students can sign in to view their own academic information.

## Features

- Separate faculty and student login flows
- Add students with a roll number, class, contact details, and subjects
- List student profiles and update student details
- Record subject marks (0–100) and attendance
- Calculate and display attendance percentages
- Let faculty and students change their passwords during a session
- Validate roll numbers, mobile numbers, marks, and attendance values

## Requirements

- A C++ compiler with C++11 support or newer
- A terminal or console

The program includes platform-specific console handling for Windows and POSIX-style systems (including Linux and macOS).

## Build and run

### Linux and macOS

```sh
g++ -std=c++11 -Wall -Wextra -pedantic STAR.cpp -o star
./star
```

### Windows (MinGW)

```powershell
g++ -std=c++11 -Wall -Wextra -pedantic STAR.cpp -o star.exe
.\star.exe
```

## Login defaults

- **Faculty password:** `axid`
- **New student password:** `123`

Students sign in with their roll number and password. Change the default passwords when trying the application.

## Data and security notes

This is a console demonstration, not a production student-record system. Student records and password changes are held in memory only; they are not saved to disk and are lost when the program exits. The built-in default passwords and lack of persistent, secure credential storage make this unsuitable for real personal or academic data.

## Usage overview

1. Start the program and choose **Faculty Login** or **Student Login**.
2. Faculty can add students, list students, update profile details, enter marks and attendance, or change the faculty password.
3. Students can view their records and attendance or change their password.
4. Choose **Exit** from the main menu to close the program.

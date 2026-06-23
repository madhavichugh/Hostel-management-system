#include<iostream>
#include<vector>
#include<iomanip>
#include<algorithm>
#include<fstream>
#include<stdexcept>
using namespace std;

/* ================= PERSON CLASS ================= */
class Person
{
protected:
    int personId;
    string name;

public:
    void setId(int id) { personId = id; }
    void setName(string n) { name = n; }

    int getId() { return personId; }
    string getName() { return name; }
};

/* ================= STUDENT CLASS ================= */
class Student : public Person
{
private:
    int roomNumber;
    float feePaid;
    float totalFee;

public:
    Student()
    {
        roomNumber = -1;
        feePaid = 0;
        totalFee = 50000;
    }

    void addStudent()
    {
        int id;
        string n;

        cout << "Enter Student ID: ";
        cin >> id;
        setId(id);

        cout << "Enter Student Name: ";
        cin.ignore();
        getline(cin, n);
        setName(n);

        cout << "Enter Room Number: ";
        cin >> roomNumber;

        cout << "Enter Fee Paid: ";
        cin >> feePaid;
    }

    void displayStudent()
    {
        cout << "---------------------------\n";
        cout << "Student ID : " << getId() << endl;
        cout << "Name       : " << getName() << endl;
        cout << "Room No    : " << roomNumber << endl;
        cout << "Fee Paid   : " << feePaid << endl;
        cout << "Balance    : " << getRemainingFee() << endl;
        cout << "---------------------------\n";
    }

    int getRoom() { return roomNumber; }
    void setRoom(int r) { roomNumber = r; }

    void setFeePaid(float f) { feePaid = f; }

    void updateStudent()
    {
        string n;
        cout << "Enter New Name: ";
        cin.ignore();
        getline(cin, n);
        setName(n);
    }

    void payFee()
    {
        float amount;
        cout << "Enter Amount: ";
        cin >> amount;

        feePaid += amount;
        if (feePaid > totalFee)
            feePaid = totalFee;
    }

    float getFeePaid() { return feePaid; }
    float getRemainingFee() { return totalFee - feePaid; }
};

/* ================= VISITOR CLASS ================= */
class Visitor
{
private:
    string visitorName;
    int studentId;

public:
    void addVisitor()
    {
        cout << "Enter Visitor Name: ";
        cin.ignore();
        getline(cin, visitorName);

        cout << "Enter Student ID visiting: ";
        cin >> studentId;
    }

    void displayVisitor()
    {
        cout << "Visitor Name : " << visitorName << endl;
        cout << "Student ID   : " << studentId << endl;
        cout << "---------------------------\n";
    }
};

/* ================= ADMIN LOGIN ================= */
bool adminLogin()
{
    string username, password;
    int attempts = 3;

    while (attempts > 0)
    {
        cout << "\n===== ADMIN LOGIN =====\n";

        cout << "Username : ";
        cin >> username;

        cout << "Password : ";
        cin >> password;

        if (username == "admin" && password == "1234")
        {
            cout << "Login Successful\n";
            return true;
        }
        else
        {
            attempts--;
            cout << "Wrong credentials. Attempts left: " << attempts << endl;
        }
    }
    return false;
}

/* ================= FILE HANDLING ================= */
void saveStudents(vector<Student> &students)
{
    ofstream fout("students.txt");

    for (auto &s : students)
    {
        fout << s.getId() << endl;
        fout << s.getName() << endl;
        fout << s.getRoom() << endl;
        fout << s.getFeePaid() << endl;
    }
}

void loadStudents(vector<Student> &students)
{
    ifstream fin("students.txt");
    if (!fin) return;

    int id, room;
    float fee;
    string name;

    while (fin >> id)
    {
        fin.ignore();
        getline(fin, name);
        fin >> room >> fee;

        Student s;
        s.setId(id);
        s.setName(name);
        s.setRoom(room);
        s.setFeePaid(fee);

        students.push_back(s);
    }

    fin.close();
}

/* ================= HELPER ================= */
int findStudent(vector<Student> &students, int id)
{
    for (int i = 0; i < students.size(); i++)
    {
        if (students[i].getId() == id)
            return i;
    }
    throw runtime_error("Student not found!");
}

void displayStudents(vector<Student> &students)
{
    if (students.empty())
    {
        cout << "No students found.\n";
        return;
    }

    for (auto &s : students)
        s.displayStudent();
}

void sortStudents(vector<Student> &students)
{
    sort(students.begin(), students.end(),
         [](Student a, Student b)
         {
             return a.getId() < b.getId();
         });

    cout << "Students sorted by ID.\n";
}

/* ================= BILL ================= */
void generateBill(Student s)
{
    cout << "\n=============================\n";
    cout << "        HOSTEL RECEIPT\n";
    cout << "=============================\n";

    cout << "Student ID : " << s.getId() << endl;
    cout << "Name       : " << s.getName() << endl;
    cout << "Total Fee  : 50000\n";
    cout << "Paid       : " << s.getFeePaid() << endl;
    cout << "Balance    : " << s.getRemainingFee() << endl;

    cout << "=============================\n";
}

/* ================= ROOM SYSTEM ================= */
const int TOTAL_ROOMS = 20;
bool rooms[TOTAL_ROOMS] = {false};

void syncRooms(vector<Student> &students)
{
    for (auto &s : students)
    {
        if (s.getRoom() >= 0 && s.getRoom() < TOTAL_ROOMS)
            rooms[s.getRoom()] = true;
    }
}

void showAvailableRooms()
{
    cout << "Available Rooms:\n";
    for (int i = 0; i < TOTAL_ROOMS; i++)
        if (!rooms[i])
            cout << "Room " << i << endl;
}

void allocateRoom(vector<Student> &students)
{
    int id, room;
    cout << "Enter Student ID: ";
    cin >> id;

    int index = findStudent(students, id);

    cout << "Enter Room Number: ";
    cin >> room;

    if (room < 0 || room >= TOTAL_ROOMS)
        throw runtime_error("Invalid room number!");

    if (rooms[room])
        throw runtime_error("Room unavailable!");

    rooms[room] = true;
    students[index].setRoom(room);

    cout << "Room allocated\n";
}

void changeRoom(vector<Student> &students)
{
    int id, newRoom;
    cout << "Enter Student ID: ";
    cin >> id;

    int index = findStudent(students, id);

    cout << "Enter New Room: ";
    cin >> newRoom;

    if (newRoom < 0 || newRoom >= TOTAL_ROOMS)
        throw runtime_error("Invalid room!");

    if (rooms[newRoom])
        throw runtime_error("Room occupied!");

    rooms[newRoom] = true;
    students[index].setRoom(newRoom);

    cout << "Room changed\n";
}

/* ================= FEE REPORT ================= */
void feeReport(vector<Student> &students)
{
    cout << "\n===== Fee Status Report =====\n";

    for (auto &s : students)
    {
        cout << "Student ID : " << s.getId() << endl;
        cout << "Name       : " << s.getName() << endl;
        cout << "Balance    : " << s.getRemainingFee() << endl;
        cout << "-----------------------------\n";
    }
}

/* ================= MAIN ================= */
int main()
{
    if (!adminLogin())
        return 0;

    vector<Student> students;
    vector<Visitor> visitors;

    loadStudents(students);
    syncRooms(students);

    int choice;

    while (true)
    {
        cout << "\n========= HOSTEL MENU =========\n";
        cout << "1. Add Student\n2. Display Students\n3. Update Student\n4. Delete Student\n";
        cout << "5. Search Student\n6. Pay Fee\n7. Generate Bill\n8. Sort Students\n";
        cout << "9. Add Visitor\n10. Display Visitors\n11. Allocate Room\n";
        cout << "12. Change Room\n13. Show Available Rooms\n14. Fee Status Report\n";
        cout << "15. Admin Logout\n16. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        try
        {
            if (choice == 1)
            {
                Student s;
                s.addStudent();
                students.push_back(s);
                saveStudents(students);
            }
            else if (choice == 2)
                displayStudents(students);

            else if (choice == 3)
            {
                int id;
                cout << "Enter ID: ";
                cin >> id;

                int index = findStudent(students, id);
                students[index].updateStudent();
                saveStudents(students);
            }

            else if (choice == 4)
            {
                int id;
                cout << "Enter ID: ";
                cin >> id;

                int index = findStudent(students, id);
                students.erase(students.begin() + index);
                saveStudents(students);

                cout << "Deleted\n";
            }

            else if (choice == 5)
            {
                int id;
                cout << "Enter ID: ";
                cin >> id;

                int index = findStudent(students, id);
                students[index].displayStudent();
            }

            else if (choice == 6)
            {
                int id;
                cout << "Enter ID: ";
                cin >> id;

                int index = findStudent(students, id);
                students[index].payFee();
                saveStudents(students);
            }

            else if (choice == 7)
            {
                int id;
                cout << "Enter ID: ";
                cin >> id;

                int index = findStudent(students, id);
                generateBill(students[index]);
            }

            else if (choice == 8)
                sortStudents(students);

            else if (choice == 9)
            {
                Visitor v;
                v.addVisitor();
                visitors.push_back(v);
            }

            else if (choice == 10)
            {
                for (auto &v : visitors)
                    v.displayVisitor();
            }

            else if (choice == 11)
            {
                allocateRoom(students);
                saveStudents(students);
            }

            else if (choice == 12)
            {
                changeRoom(students);
                saveStudents(students);
            }

            else if (choice == 13)
                showAvailableRooms();

            else if (choice == 14)
                feeReport(students);

            else if (choice == 15)
            {
                cout << "Logging out...\n";
                if (!adminLogin())
                    break;
            }

            else if (choice == 16)
            {
                cout << "Exiting Program\n";
                break;
            }

            else
                cout << "Invalid choice\n";
        }
        catch (exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}

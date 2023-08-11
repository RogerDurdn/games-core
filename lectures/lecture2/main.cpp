#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
//namespace can be used to incorporate that syntax without the need of prefix

// practice of different concepts
void strings();

void vectors();

void classes();

void studentsFile();

class Student {
    string m_first = "First";
    string m_last = "Last";
    int m_id = 0;
    float m_avg = 0;

public:
    Student() {}

    Student(string first, string last, int id, float avg)
            : m_first(first), m_last(last), m_id(id), m_avg(avg) {
    }

    float getAvg() {
        return m_avg;
    }

    float getId() {
        return m_id;
    }

    void print() const {
        cout << "student: " << m_first << " " << m_last << " id:" << m_id << " avg:" << m_avg << endl;
    }

};

class Course {
    string m_name = "course";
    vector<Student> m_students;
public:
    Course() {}

    Course(const string &name)
            : m_name(name) {}

    void addStudent(const Student &s) {
        m_students.push_back(s);
    }

    const vector<Student> &getStudents() const {
        return m_students;
    }

    void loadFromFile(const string &fileName) {
        ifstream fileIn(fileName);
        string first, last;
        int id;
        float avg;

        while (fileIn >> first) {
            fileIn >> last >> id >> avg;
            addStudent(Student(first, last, id, avg));
        }
    }

    void print() const {
        cout << "course students " << m_name << endl;
        for (const auto s: m_students) s.print();
    }

};


int main() {

//    strings();
//    vectors();
//    classes();
    studentsFile();

    return 0;
}

void studentsFile() {
    auto games = Course("games");
    games.loadFromFile("students.txt");
    games.print();
}

void strings() {
    int age = 30;
    string first = "roger";
    cout << "hey im: " << first << " and have age of " << age << endl;
}

void vectors() {
    cout << "vectors" << endl;
    vector<int> intVec;

    intVec.push_back(10);
    intVec.push_back(11);
    intVec.push_back(22);


    vector<float> floatVec;

    floatVec.push_back(10.1f);
    floatVec.push_back(11.2f);
    floatVec.push_back(22.44f);

    // iterate by index
    for (size_t i = 0; i < intVec.size(); ++i) // we should use size_t because size() returns this type
        cout << "by index - vec val:" << i << endl;

    // iterate by range
    for (int intVal: intVec)
        cout << "by range - vec vala:" << intVal << endl;

    for (auto val: floatVec) { // we can use auto to delegate the type inference to te compiler
        cout << "by range auto - vec vala:" << val << endl;
    }
}

void classes() {

    Student s1;
    Student s2("roger", "vl", 20, 33);
    const Student s3("jane", "dole", 2, 55);
    // in order to use a function from a const variable the function has to be const also

    s1.print();
    s2.print();
    s3.print();

    auto games = Course("games");
    games.addStudent(s1);
    games.addStudent(s2);
    games.addStudent(s3);
    games.print();
}
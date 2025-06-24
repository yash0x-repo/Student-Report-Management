#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

struct Course{
    string name;
    int marks;
    int credit;
    int GradePoint(){
        if(marks>=90) return 10;
        else if(marks>=80) return 9;
        else if(marks>=70) return 8;
        else if(marks>=60) return 7;
        else if(marks>=50) return 6;
        else if(marks>=40) return 5;
        else return 0;
    }
};

struct Semester{
    int sem_no;
    vector<Course> courses;
    float spi;
    Semester(){
        sem_no = 0;
        spi=0;
    }
    Semester(int no, vector<Course> c) {
        sem_no = no;
        courses = c;
        calculateSPI();
    }

    void calculateSPI() {
        int total_credits = 0;
        int total_gradepoints = 0;
        for (auto &c : courses) {
            total_credits += c.credit;
            total_gradepoints += c.GradePoint() * c.credit;
        }
        spi = (total_credits == 0) ? 0 : (float)total_gradepoints / total_credits;
    }

    void updateCourse(string course_name, int new_marks, int new_credits) {
        for (auto &c : courses) {
            if (c.name == course_name) {
                c.marks = new_marks;
                c.credit = new_credits;
                break;
            }
        }
        calculateSPI();
    }
};

struct Student {
    string name;
    int roll;
    vector<Semester> semesters;
    float cpi;
    int total;
    float average;

    void calculate_cpi() {
        float sum_cpi = 0;
        for (auto &s : semesters) sum_cpi += s.spi;
        cpi = (semesters.size() == 0) ? 0 : sum_cpi / semesters.size();
    }

    void calculate_Marks() {
        int sum = 0;
        int count = 0;
        for (auto &sem : semesters) {
            for (auto &c : sem.courses) {
                sum += c.marks;
                count++;
            }
        }
        total = sum;
        average = (count == 0) ? 0 : (float)sum / count;
    }

    void recalculateAll() {
        for (auto &sem : semesters) sem.calculateSPI();
        calculate_cpi();
        calculate_Marks();
    }

    void addSemester(const Semester &sem) {
        semesters.push_back(sem);
        recalculateAll();
    }
    void updateCourseMarks(int sem_no, string course_name, int marks, int credits) {
        for (auto &sem : semesters) {
            if (sem.sem_no == sem_no) {
                sem.updateCourse(course_name, marks, credits);
                recalculateAll();
                return;
            }
        }
        cout << "Semester " << sem_no << " not found.\n";
    }

    void displayTranscript() {
        cout << "\nTranscript for: " << name << " (Roll: " << roll << ")\n";
        if (semesters.empty()) {
            cout << "No semesters recorded.\n";
            return;
        }
        for (auto &sem : semesters) {
            cout << "\nSemester " << sem.sem_no << ":\n";
            cout << left << setw(20) << "Course" << setw(10) << "Marks" << setw(10) << "Credits" << endl;
            for (auto &c : sem.courses) {
                cout << left << setw(20) << c.name << setw(10) << c.marks << setw(10) << c.credit << endl;
            }
            cout << "Semester SPI: " << sem.spi << "\n";
        }
        cout << "Total Marks: " << total << "\n";
        cout << "Average Marks: " << average << "\n";
        cout << "Overall CPI: " << cpi << "\n";
    }
};


struct Branch{
    string name;
    vector<Student> students;
};

vector<Branch> branches;

void add_branch(){
    Branch b;
    cout<<"Enter Branch Name: ";
    cin.ignore();
    getline(cin,b.name);
    branches.push_back(b);
    cout<<"Branch Added! ";
}

bool existing_roll(int roll){
    for(const auto& br:branches){
        for(const auto& s:br.students){
            if(s.roll==roll) return true;
        }
    }
    return false;
}

void register_student(){
    if(branches.empty()){
        cout<<"No Branches Available\n Add a new branch \n";
        return;
    }
    cout<<"Select Branch: \n";
    for(int i=0; i<branches.size(); i++){
        cout<< i+1 <<". "<< branches[i].name << endl; 
    }
    int br;
    cout<<"select branch index: ";
    cin>>br;
    if(br<1 || br>branches.size()){
        cout<<"Invalid Branch Index: ";
        return;
    }
    Student s;
    cout<<"Enter student name: ";
    cin.ignore();
    getline(cin,s.name);
    cout<<"Enter Roll No.: ";
    cin>>s.roll;

    if (existing_roll(s.roll)) {
        cout << "A student with this roll number already exists!\n";
        return;
    }

    branches[br-1].students.push_back(s);
    cout<<"Student Registered Successfully.\n";
}

void addSemCourse(){
    int roll;
    cout<<"Enter Roll No.: ";
    cin>>roll;

    for(auto& br:branches){
        for(auto& s:br.students){
            if(s.roll==roll){
                Semester sem;
                cout<<"Enter Semester Number: ";
                cin>>sem.sem_no;

                int n;
                cout<<"Enter Number of courses: ";
                cin>>n;
                for(int i=0; i<n; i++){
                    Course c;
                    cout<<"Enter Course Name: ";
                    cin.ignore();
                    getline(cin, c.name);
                    cout<<"Enter Marks (0-100): ";
                    cin>>c.marks;
                    while(c.marks<0 || c.marks>100){
                        cout<<"Invalid marks. Enter again (0-100): ";
                        cin>>c.marks;
                    }
                    cout<<"Enter Credits: ";
                    cin>>c.credit;
                    while(c.credit<1){
                        cout<<"Invalid credits, Enter again: ";
                        cin>>c.credit;
                    }
                    sem.courses.push_back(c);
                }
                sem.calculateSPI();
                s.semesters.push_back(sem);

                s.recalculateAll();

                cout<<"Semester Added.\n";
                cout<<"SPI: "<<sem.spi<<endl;
                return;       
            }
        }
    }
    cout<<"Student Not Found!! \n";
}

void view_transcript(){
    int roll;
    cout<<"Enter Roll No.: ";
    cin>>roll;
    for(auto& br:branches){
        for(auto& s:br.students){
            if(s.roll == roll){
                s.displayTranscript();
                return;
            }
        }
    }
    cout<<"Student Not Found.\n";
}

void updateMarks(){
    int roll;
    int sem_num;
    cout<<"Enter Student Roll No.";
    cin>>roll;

    for(auto& br:branches){
        for(auto s:br.students){
            if(s.roll==roll){
                cout<<"Enter Semester No.";
                cin>>sem_num;

                if(sem_num<1 || sem_num>s.semesters.size()){
                     cout<<"Invalid Semester!"<<endl;
                     return;
                }
                Semester& sem = s.semesters[sem_num-1];

                string courseName;
                cout<<"Enter Course Name: ";
                cin.ignore();
                getline(cin, courseName);
                for(auto& c:sem.courses){
                    if(c.name==courseName){
                        cout<<"Current Marks: "<<c.marks<<"\n";
                        cout<<"Credits"<<c.credit<<endl;

                        cout<<"Enter New Marks: ";
                        cin>>c.marks;
                        cout<<"Enter New course credits: ";
                        cin>>c.credit;

                        // sem.calculateSPI;
                        cout<<"New SPI"<<sem.spi<<endl;
                        return;
                    }
                }
                cout<<"Course Not found in the semester."<<endl;
                cout<<"Would you like to add the course? (yes/no) ";
                string ch;
                cin>>ch;
                if(ch=="yes"){
                    addSemCourse();
                }
                return; 
            }
        }
    }
    cout<<"Student with Roll No. "<<roll<<"not found!!"<<endl;
}

void deleteCourse(){
    int roll;
    cout<<"Enter Roll No.: ";
    cin>>roll;
    for(auto& br:branches){
        for(auto& s:br.students){
            if(s.roll == roll){
                int sem_num;
                cout<<"Enter Semester Number: ";
                cin>>sem_num;
                if(sem_num<1 || sem_num>s.semesters.size()){
                    cout<<"Invalid Semester! \n";
                    return;
                }
                Semester& sem = s.semesters[sem_num-1];

                cout<<"Courses in Semester"<<sem.sem_no<<"\n";
                for (auto & c:sem.courses){
                    cout<<"-"<<c.name<<endl;
                }

                string courseName;
                cout<<"Enter Course Name: ";
                cin.ignore();
                getline(cin, courseName);

                bool found=false;
                for(int i=0; i<sem.courses.size(); i++){
                    if(sem.courses[i].name==courseName){
                        sem.courses.erase(sem.courses.begin()+i);
                        found = true;
                        break;
                    }
                }
                if(!found){
                    cout<<"Course Not Found! \n";
                    cout<<"Would you like to add the course? (yes/no) ";
                    string ch;
                    cin>>ch;
                    if(ch=="yes"){
                        addSemCourse();
                    }
                    return;
                }
                sem.calculateSPI();
                s.recalculateAll();
                cout<<"Course Deleted Successfully!\n";
                cout<<"SPI and CPI updated. \n";
                return;
            }
        }
    }
    cout<<"Student with Roll No: "<<roll<<"not found! \n";
}

void searchStudent(){
    int roll;
    cout<<"Enter Roll No. to search: ";
    cin>>roll;

    for(auto& br:branches){
        for(auto& s:br.students){
            if(s.roll == roll){
                cout<<"\nStudent Found!\n";
                cout<<"Name: "<<s.name<<"\n";
                cout<<"Branch: "<<br.name<<"\n";
                s.displayTranscript();
                return;
            }
        }
    }
    cout<<"No Student with roll no.: "<<roll<<".\n";
    cout<<"Do you want to add student?  (yes/no)";
    string ch;
    cin>>ch;
    if(ch=="yes"){
        register_student();
    }
}

void removeStudent(){
    int roll;
    cout<<"Enter Roll No. to search: ";
    cin>>roll;

    for(auto& br:branches){
        for(int i=0; i<br.students.size(); i++){
            if(br.students[i].roll == roll){
                br.students[i].displayTranscript();

                string confirm;
                cout<<"Are you sure you want to delete this student?  (yes/no)";
                cin>>confirm;
                if(confirm=="yes"){
                    br.students.erase(br.students.begin() + i);
                    cout<<"student deleted successfully";
                }
                else{
                    cout<<"deletion cancelled.\n";
                }
                return;
            }
        }
    }
    cout<<"Student with Roll No.: "<<roll<<"not found!\n";
}


// void Rank_students(){
//     if(branches.empty()){
//         cout<<"No branches available.\n";
//         return;
//     }
//     cout<<"select branch to view ranking: \n";
//     for(int i=0; i<branches.size(); ++i){
//         cout<<i+1<<". "<<branches[i].name<<"\n";
//     }
//     cout<<branches.size()+1<<". "<<"All branches"<<"\n";
//     int choice;
//     cout<<"Enter choice: ";
//     cin>>choice;
//     vector<Student> students;
// }

void export_CSV(){
    int roll;
    cout << "Enter Roll No. of student: ";
    cin >> roll;
     for (auto& br : branches) {
        for (auto& s : br.students) {
            if (s.roll == roll) {
                s.recalculateAll();
                
                string file_name = "transcript_"+to_string(roll)+".csv";
                ofstream fout(file_name);
                if(!fout.is_open()){
                    cout << "Error opening file!\n";  
                    return;
                }

                fout << "Student Name,Roll Number,Semester No,Course Name,Marks,Credits,SPI,CPI\n";
                for (auto& sem : s.semesters) {
                    for (auto& c : sem.courses) {
                        fout << s.name << "," << s.roll << "," << sem.sem_no << ","
                             << c.name << "," << c.marks << "," << c.credit << ","
                             << fixed << setprecision(2) << sem.spi << "," << s.cpi << "\n";
                    }
                }
                fout.close();
                cout<<"Transcript exported to "<<file_name<<" successfully!\n";
                return;
            }
        }
    }
    cout<<"Student with Roll No. "<<roll<< "not found!\n";
}

void showMenu() {
    cout << "\n===== STUDENT RECORD SYSTEM MENU =====\n";
    cout << "1. Add Branch\n";
    cout << "2. Register Student\n";
    cout << "3. Add Semester and Courses\n";
    cout << "4. Update Marks\n";
    cout << "5. Delete Course\n";
    cout << "6. Delete Student\n";
    cout << "7. Search Student\n";
    cout << "8. View Transcript\n";
    cout << "9. Export Transcript to CSV\n";
    cout << "10. Rank Students (Branch-wise / Overall)\n";
    cout << "11. Export Report Card as Image\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

void export_image(){
    int roll;
    cout<<"Enter Roll Number: ";
    cin>> roll;
    
    string command = "python ../python/generate_report.py " + to_string(roll);
    int result = system(command.c_str());
    if(result == 0){
        cout<<"Report Card image generated successfully. \n";
    }
    else {
        cout<<"Failed to generate the report card image. \n";
    }
}

// void search_student(const vector<Student>& students, int roll){
//     for(const auto& s:students){
//         if(s.roll == roll){
//             cout<<"\n Found Student: "<<s.name<<"\n";
//             cout<<"Courses: \n";
//             for(const auto& c:s.courses){
//                 cout<<" - "<<c.name<<"  | Marks: "<<c.marks<<"  | Credits: "<<c.credit<<"\n";
//             }
//             cout<<"Total: "<<s.total<<"\n";
//             cout<<"Average: "<< s.average<<"\n";
//             cout<<"CPI: "<<s.cpi<<"\n";
//             return;
//         }
//     }
//     cout<<"Student with Roll No. "<<roll<<"not found!\n";
// }
void saveData(const string& file_name){
    ofstream fout(file_name);
    if(!fout.is_open()){
        cout<<"Failed to open file for saving.\n";
        return;
    }
    fout<<branches.size()<<"\n";
    for(auto & br:branches){
        fout<<br.name<<"\n"<<br.students.size()<<"\n";
        for(auto& s:br.students){
            fout<<s.name<<"\n"<<s.roll<<"\n"<<s.semesters.size()<<"\n";
            for(auto& sem:s.semesters){
                fout<<sem.sem_no<<"\n"<<sem.courses.size()<<"\n";
                for(auto& c:sem.courses){
                    fout<<c.name<<"\n"<<c.marks<<"\n"<<c.credit<<"\n";
                }
            }
        }
    }
    fout.close();
    cout<<"Data Saved Successfully.\n";
}

void loadData(const string & file_name){
    ifstream fin(file_name);
    if(!fin.is_open()){
        cout<<"No existing data found";
        return;
    }
    branches.clear();
    int branchCount;
    fin>>branchCount;
    fin.ignore();
    while(branchCount--){
        Branch br;
        getline(fin, br.name);
        int studentCount;
        fin>>studentCount;
        fin.ignore();
        while(studentCount--){
            Student s;
            getline(fin, s.name);
            fin>>s.roll;
            int semCount;
            fin>>semCount;
            fin.ignore();
            while(semCount--){
                Semester sem;
                fin>>sem.sem_no;
                int courseCount;
                fin>>courseCount;
                fin.ignore();
                while(courseCount--){
                    Course c;
                    getline(fin, c.name);
                    fin>>c.marks>>c.credit;
                    fin.ignore();
                    sem.courses.push_back(c);
                }
                sem.calculateSPI();
                s.semesters.push_back(sem);
            }
            s.recalculateAll();
            br.students.push_back(s);
        }
        branches.push_back(br);
    }
    cout<<"Data loaded Successfully.\n";
}


int main() {
    loadData("data_backup.txt");

    int choice;
    do {
        showMenu();
        cin >> choice;
        switch(choice) {
            case 1: add_branch(); break;
            case 2: register_student(); break;
            case 3: addSemCourse(); break;  
            case 4: updateMarks(); break;
            case 5: deleteCourse(); break;
            case 6: removeStudent(); break;
            case 7: searchStudent(); break;
            case 8: view_transcript(); break;
            case 9: export_CSV(); break;
            // case 10: rankStudents(); break;1
            case 11: export_image(); break;
            case 0: cout << "Exiting Program. Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while(choice != 0);
    saveData("data_backup.txt");
    cout << "Exiting...\n";
    return 0;
}

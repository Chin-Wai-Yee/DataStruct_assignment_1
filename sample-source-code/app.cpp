#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"


using namespace std;

bool ReadFile(string, List *);
bool DeleteRecord(List *, char *);
bool Display(List, int, int);
bool InsertBook(string, List *);
bool SearchStudent(List *, char *id, LibStudent &);
bool computeAndDisplayStatistics(List *);
bool printStuWithSameBook(List *, char *);
bool displayWarnedStudent(List *, List *, List *);
int menu();


int main() {
	
	// Test
	List student_list;
	if (!ReadFile("../sample-text-files/student.txt", &student_list)) {
		cout << "Error while opening the file" << endl;
		cout << "Current working directory : ";
		system("cd");
		cout << "Do check the code to make sure that you entered the right path for the function call" << endl;
	}

	cout << "\n\n";
	system("pause");
	return 0;
}

bool ReadFile(string filename, List *list) {
	fstream file(filename, ios::in);
	if (!file) {
		file.close();
		return false;
	}

	LibStudent student;
	string dummy;
	while (!file.eof()) {
		file >> dummy >> dummy;
		file >> student.id;
		file >> dummy >> dummy;
		file.ignore();
		// ^ got a space before the name,
		//   getline will get the whitespace
		file.getline(student.name, 30);
		file >> dummy >> dummy;
		file >> student.course;
		file >> dummy >> dummy;
		file >> student.phone_no;
		file >> dummy >> dummy;
		list->insert(student);
	}

	return true;
}
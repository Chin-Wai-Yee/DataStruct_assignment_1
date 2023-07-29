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

// extra functions
int getPosition(List*, char*);
int split(string, const char*, char*[]);

int main() {
	
	/* Uncomment this block to test the functions

	// Test for file handling
	List* student_list = new List();
	if (!ReadFile("../sample-text-files/student.txt", student_list)) {
		cout << "Error while opening the file" << endl;
		cout << "Current working directory : ";
		system("cd");
		cout << "Do check the code to make sure that you entered the right path for the function call" << endl;
		return -1;
	}

	if (!InsertBook("../sample-text-files/book.txt", student_list)) {
		cout << "Insert book failed" << endl;
		return -1;
	}

	// Print out all students with their books
	for (Node* cur = student_list->head; cur != NULL; cur = cur->next) {
		cur->item.print(cout);
		cout << endl;
		cout << "Books:" << endl;
		for (int i = 0; i < cur->item.totalbook; i++) {
			cur->item.book[i].print(cout);
		}
	}
	
	*/

	system("pause");
	return 0;
}

int getPosition(List* list, char* id) {

	Node* cur = list->head; // current node

	for (int count = 1; count <= list->size(); count++) {
		if (strcmp(cur->item.id, id) == 0) {
			// found, return
			return count;
		}
		cur = cur->next;
	}

	return -1;
}

int split(string str, const char* delim, char* output[]) {
	
	int count = 0; // total number of tokens

	char* token = strtok((char*)str.c_str(), delim);
	while (token) {
		// allocate memory for each token
		output[count] = new char[strlen(token) + 1];
		// copy token to output
		strcpy(output[count], token);
		count++;

		// get next token
		token = strtok(NULL, delim);
	}

	return count;
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

		// read id
		file >> dummy >> dummy >> dummy;
		file >> student.id;

		// read name
		file >> dummy >> dummy;
		file.ignore();
		// ^ got a space before the name,
		//   getline will get the whitespace
		file.getline(student.name, 30);

		// read course and phone no
		file >> dummy >> dummy;
		file >> student.course;
		file >> dummy >> dummy >> dummy;
		file >> student.phone_no;

		// insert to list
		list->insert(student);
	}

	return true;
}

void AddBook(LibStudent& student, LibBook book) {

	int count = student.totalbook;
	student.book[count] = book;
	student.totalbook = count + 1;

}

void ReadDate(string date_str, Date& date) {

	char* date_arr[3]; // store date temporarily
	split(date_str, "/", date_arr);

	// convert to int
	date.day = atoi(date_arr[0]);
	date.month = atoi(date_arr[1]);
	date.year = atoi(date_arr[2]);

	// free memory
	for (int i = 0; i < 3; i++) {
		delete date_arr[i];
	}
}

bool InsertBook(string filename, List* list) {
	fstream file(filename, ios::in);

	if (!file) {
		file.close();
		return false;
	}

	char id[10]; // student id
	LibStudent student; // student according to id
	string dummy; // input buffer
	LibBook book; // current book

	while (!file.eof()) {

		// get student id
		file >> id;
		int position = getPosition(list, id);
		if (position == -1) {
			cout << "Unable to find student with id : " << id << endl;
			getline(file, dummy);
			file.clear();
			file.ignore();
			continue;
		}

		// the student is in list, get it out
		list->get(position, student);

		// read book's authors
		file >> dummy;
		split(dummy, "/", book.author);

		// other book's info
		file >> book.title;
		file >> book.publisher;
		file >> book.ISBN;
		file >> book.yearPublished;
		file >> book.callNum;

		// read book's date
		file >> dummy;
		ReadDate(dummy, book.borrow);
		file >> dummy;
		ReadDate(dummy, book.due);

		// modify student's book and put it back
		student.book[student.totalbook] = book;
		student.totalbook++;
		list->set(position, student);
	}
}
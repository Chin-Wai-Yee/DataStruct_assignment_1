#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"

// additional include
#include <iomanip> // for decoration purpose

using namespace std;

bool ReadFile(string, List *);
bool DeleteRecord(List *, char *);
bool Display(List*, int, int);
bool InsertBook(string, List *);
bool SearchStudent(List *, char *id, LibStudent &);
bool computeAndDisplayStatistics(List *);
bool printStuWithSameBook(List *, char *);
bool displayWarnedStudent(List *, List *, List *);
int menu();

// extra functions
int getPosition(List*, char*);
int split(string, const char*, char*[]);
void DisplayStudents(ostream&, List*, int);
void DisplayBooks(ostream&, LibStudent, int);
bool existBook(LibStudent, LibBook);
int toJulianDate(Date);
double calculateFine(LibBook);
int calculateTotalFineBooks(LibStudent&);
void updateCourseStatistics(List*, const char*, int&, int&, int&, double&);

int main() {
	
	// Uncomment this block to test the functions

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
	// Display(student_list, 1, 1);
	// Display(student_list, 1, 2);
	// Display(student_list, 2, 1);
	// Display(student_list, 2, 2);
	computeAndDisplayStatistics(student_list);

	system("pause");
	return 0;
}

bool Display(List* list, int source, int detail)
{

	if (list->empty()) {
		cout << "Error : the list is empty.\n" << endl;
		return false;
	}

	if (source == 1)
	{
		ofstream outFile;
		if (detail == 1) {
			outFile.open("student_booklist.txt");
		}
		else {
			outFile.open("student_info.txt");
		}
		if (!outFile) {
			cout << "Cannot open the file for writing. " << endl;
			return false;
		}
		DisplayStudents(outFile, list, detail);
	}
	else {
		DisplayStudents(cout, list, detail);
	}

	return true;
}

void DisplayStudents(ostream& out, List* list, int detail) {
	int i = 1; // student number
	Node* cur = list->head;
	while (cur != NULL)
	{
		out << "\nSTUDENT " << i << endl;
		cur->item.print(out);
		DisplayBooks(out, cur->item, detail);
		out << "\n************************************************" << endl;
		i++;
		cur = cur->next;
	}
}

void DisplayBooks(ostream& out, LibStudent student, int detail) {
	// early return on dont display books
	if (detail == 2) {
		return;
	}

	out << "\nBOOK LIST: " << endl;
	for (int j = 0; j < student.totalbook; j++)
	{
		out << "Book " << j + 1 << endl;
		student.book[j].print(out);
		out << endl;
	}
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
		if (getPosition(list, student.id) == -1) {
			// the student is not in the list
			list->insert(student);			
		}
		else {
			cout << "Error : duplicate student id " << student.id << endl;
		}
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

int toJulianDate(Date date) {
	int a = (14 - date.month) / 12;
	int y = date.year + 4800 - a;
	int m = date.month + 12 * a - 3;
	int jdn = date.day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
	return jdn;
}

double calculateFine(LibBook book) {
	
	// fine rate
	const double fine_rate = 0.5;

	// assume today is 29/3/2020
	Date today;
	today.day = 29;
	today.month = 3;
	today.year = 2020;


	// calculate fine
	double fine = 0;
	int days = toJulianDate(today) - toJulianDate(book.due);
	if (days > 0) {
		fine = days * fine_rate;
	}
	return fine;
}

bool existBook(LibStudent student, LibBook book) {
	for (int i = 0; i < student.totalbook; i++) {
		if (student.book[i].compareCallNum(book)) {
			return true;
		}
	}
	return false;
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

		// calculate fine
		book.fine = calculateFine(book);

		if (existBook(student, book)) {
			cout << "Error : duplicate book " << book.title << " for student " << student.id << endl;
			continue;
		}

		// modify student's book and put it back
		student.book[student.totalbook] = book;
		student.totalbook++;
		student.calculateTotalFine();
		list->set(position, student);
	}
}

bool computeAndDisplayStatistics(List* list)
{
	int numCS = 0, numIA = 0, numIB = 0, numCN = 0, numCT = 0;
	int totalBooksCS = 0, totalBooksIA = 0, totalBooksIB = 0, totalBooksCN = 0, totalBooksCT = 0;
	int overdueBooksCS = 0, overdueBooksIA = 0, overdueBooksIB = 0, overdueBooksCN = 0, overdueBooksCT = 0;
	double totalFineCS = 0.0, totalFineIA = 0.0, totalFineIB = 0.0, totalFineCN = 0.0, totalFineCT = 0.0;


	//if list is empty
	if (list->empty())
		return false;

	// Update statistics for each course using the new function
	updateCourseStatistics(list, "CS", numCS, totalBooksCS, overdueBooksCS, totalFineCS);
	updateCourseStatistics(list, "IA", numIA, totalBooksIA, overdueBooksIA, totalFineIA);
	updateCourseStatistics(list, "IB", numIB, totalBooksIB, overdueBooksIB, totalFineIB);
	updateCourseStatistics(list, "CN", numCN, totalBooksCN, overdueBooksCN, totalFineCN);
	updateCourseStatistics(list, "CT", numCT, totalBooksCT, overdueBooksCT, totalFineCT);


	// Print the statistics in a tabular format
	cout << setprecision(2) << fixed;
	cout << "\nCourse\tNumber of Students\tTotal Books Borrowed\tTotal Overdue Books\tTotal Overdue Fine (RM)\n";
	cout << "  CS\t\t" << numCS << "\t\t\t" << totalBooksCS << "\t\t\t" << overdueBooksCS << "\t\t\t" << setw(5) << totalFineCS << endl;
	cout << "  IA\t\t" << numIA << "\t\t\t" << totalBooksIA << "\t\t\t" << overdueBooksIA << "\t\t\t" << setw(5) << totalFineIA << endl;
	cout << "  IB\t\t" << numIB << "\t\t\t" << totalBooksIB << "\t\t\t" << overdueBooksIB << "\t\t\t" << setw(5) << totalFineIB << endl;
	cout << "  CN\t\t" << numCN << "\t\t\t" << totalBooksCN << "\t\t\t" << overdueBooksCN << "\t\t\t" << setw(5) << totalFineCN << endl;
	cout << "  CT\t\t" << numCT << "\t\t\t" << totalBooksCT << "\t\t\t" << overdueBooksCT << "\t\t\t" << setw(5) << totalFineCT << endl;

	return true;
}

//extra function to calculate the total fine books of the student
int calculateTotalFineBooks(LibStudent &student)
{
	int totaloverduebooks = 0;

	for (int i = 1; i <= student.totalbook; i++)
	{
		if (student.book[i].fine > 0)
			totaloverduebooks++;
	}
	return totaloverduebooks;
}

//extra function to calculate the statistic of wach course
void updateCourseStatistics(List* list, const char* courseCode, int& numStudents, int& totalBooks, int& overdueBooks, double& totalFine)
{
	for (int i = 1; i <= list->size(); i++)
	{
		LibStudent student;

		if (list->get(i, student))
		{
			if (strcmp(student.course, courseCode) == 0)
			{
				numStudents++;
				totalBooks += student.totalbook;
				overdueBooks += calculateTotalFineBooks(student);
				totalFine += student.total_fine;
			}
		}
	}
}
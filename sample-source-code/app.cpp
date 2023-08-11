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
int  menu();

// extra functions

bool mainMenu(List&);
int  getPosition(List*, char*);
void DisplayStudents(ostream&, List*, int);
void DisplayBooks(ostream&, LibStudent, int);
int  split(string, const char*, char*[]);
bool existBook(LibStudent, LibBook);
int  toJulianDate(Date);
int  getOverdueDays(Date);
double calculateFine(LibBook);
int  calculateTotalFineBooks(LibStudent&);
void updateCourseStatistics(List*, const char*, int&, int&, int&, double&);

int main() {
    List student_list;

    while (mainMenu(student_list)) {
        system("pause");
    }

	return 0;
}

bool mainMenu(List& student_list) {

	system("cls");

	switch (menu()) {
		case 1: { // read file
			if (!ReadFile("../sample-text-files/student.txt", &student_list)) {
				cout << "\nError while opening the file" << endl;
				cout << "Current working directory : ";
				system("cd");
				cout << "Do check the code to make sure that you entered the right path for the function call" << endl;
			}
			else {
				cout << "\nFile read successfully" << endl;
			}
			break;
		}
		case 2: { // delete record 
			cout << "Enter the student ID to delete:";
			char student_id[10];
			cin >> student_id;
			if (!DeleteRecord(&student_list, student_id)) {
				cout << "\nThe student id is not found, fail to delete.\n";
			}
			break;
		}
		case 3: { // search student with id
			cout << "Enter the student ID to search:";
			char student_id[10];
			cin >> student_id;
			LibStudent student;
			if (SearchStudent(&student_list, student_id, student)) {
				student.print(cout);
			}
			else {
				cout << "\nStudent not found" << endl;
			}
			break;
		}
		case 4: { // insert book from file
			if (!InsertBook("../sample-text-files/book.txt", &student_list)) {
				cout << "\nInsert book failed, the input file cannot be found" << endl;
				cout << "Current working directory : ";
				system("cd");
				cout << "Do check the code to make sure that you entered the right path for the function call" << endl;
			}
			else {
				cout << "\nInsert book successfully" << endl;
			}
			break;
		}
		case 5: { // display 
			cout << "Enter the output source (1 - file, 2 - screen):";
			int output_source;
			cin >> output_source;
			cout << "Do you want to display the book list (1 - yes, 2 - no):";
			int display_book_list;
			cin >> display_book_list;
			Display(&student_list, output_source, display_book_list);
			break;
		}
		case 6: { // compute and display statistics
			if(!computeAndDisplayStatistics(&student_list)) {
				cout << "\nThe list is empty" << endl;
			}
			break;
		}
		case 7: { // print student with same book
			cout << "Enter the book call number to find students: ";
			char bookCallNumToFind[20];
			cin >> bookCallNumToFind;
			printStuWithSameBook(&student_list, bookCallNumToFind); // Pass the bookCallNumToFind to the function
			break;
		}
		case 8: { // display warned student
			List type1;
			List type2;
			if (displayWarnedStudent(&student_list, &type1, &type2)) {
				system("cls");
				cout << "\nType 1: " << endl;
				Display(&type1, 2, 1);
				cout << "\nType 2: " << endl;
				Display(&type2, 2, 1);
			}
			else {
				cout << "\nCannot perform the analysis with an empty list\n";
			}
			break;
		}
		case 9: { // exit
			cout << "\nExiting the program. Goodbye!" << endl;
			return false;
		}
		default: { // invalid 
			cout << "\nInvalid choice" << endl;
			break;
		}
	}

	return true;
}

int menu() {
    int choice;
	// choices
	cout << "Menu :" << endl;
	cout << endl;
	cout << "1. Read file" << endl;
	cout << "2. Delete record" << endl;
	cout << "3. Search student" << endl;
	cout << "4. Insert book" << endl;
	cout << "5. Display output" << endl;
	cout << "6. Compute and Display Statistics" << endl;
	cout << "7. Student with Same Book" << endl;
	cout << "8. Display Warned Student" << endl;
	cout << "9. Exit" << endl;
	cout << endl;
	cout << "Enter your choice: ";
	cin >> choice;

    return choice;
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
		// ^ got a space before the name, cin ignore it but
		//   getline will get the whitespace so ignore it first
		file.getline(student.name, 30);

		// read course and phone no
		file >> dummy >> dummy;
		file >> student.course;
		file >> dummy >> dummy >> dummy;
		file >> student.phone_no;

		// insert to list
		if (!SearchStudent(list, student.id, student)) {
			// the student is not in the list
			list->insert(student);			
		}
		else {
			cout << "Error : duplicate student id " << student.id << endl;
		}
	}

	return true;
}

bool Display(List* list, int source, int detail)
{

	if (list->empty()) {
		return false;
	}

	if (source == 1)
	{
		// output is file
		ofstream outFile;
		// open respective file according to detail
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
		// output is screen
		DisplayStudents(cout, list, detail);
	}

	return true;
}

void DisplayStudents(ostream& out, List* list, int detail) {
	int i = 1; // student number
	Node* cur = list->head;
	while (cur != NULL)
	{
		// print student info
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
		// print book info
		out << "Book " << j + 1 << endl;
		student.book[j].print(out);
		out << endl;
	}
}

int getPosition(List* list, char* id) {

	Node* cur = list->head; // current node

	// Traverse the list to find the student
	for (int count = 1; count <= list->size(); count++) {
		if (strcmp(cur->item.id, id) == 0) {
			// Student with matching ID found
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

// insert book to student
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

// convert date to julian date
int toJulianDate(Date date) {
	int a = (14 - date.month) / 12;
	int y = date.year + 4800 - a;
	int m = date.month + 12 * a - 3;
	int jdn = date.day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
	return jdn;
}

int getOverdueDays(LibBook book) {
	// assume today is 29/3/2020
	Date today;
	today.day = 29;
	today.month = 3;
	today.year = 2020;

	int days = toJulianDate(today) - toJulianDate(book.due);
	return days;
}

double calculateFine(LibBook book) {
	
	// fine rate
	const double fine_rate = 0.5;

	// calculate fine
	double fine = 0;
	int overdue = getOverdueDays(book);
	if (overdue > 0) {
		fine = overdue * fine_rate;
	}
	return fine;
}

// check if the book is already in the student's book list
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

		int position = getPosition(list, id);
		if (!SearchStudent(list, id, student)) {
			// the student is not in list, skip this book
			cout << "Unable to find student with id : " << id << endl;
			continue;
		}

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

	return true;
}

bool DeleteRecord(List* list, char* id) {
	if (list->empty()) {
		return false;
	}

	LibStudent student;
	SearchStudent(list, id, student);
	int pos = getPosition(list, id);
	// student not found
	if (pos == -1) {
		return false;
	}

	for (int i = 0; i < student.totalbook; i++) {
		// delete the title of the book since it is dynamically allocated
		for (char* author : student.book[i].author) {
			delete[] author;
		}
	}
	list->remove(pos);
	list->count--;
	return true;
}

bool SearchStudent(List* list, char* id, LibStudent &stu) {
	if (list->empty()) {
		return false;
	}

	struct Node *current = list->head;
	// Traverse the list to find the student
	int pos = getPosition(list, id);
	if (pos != -1) {
		// Student with matching ID found
		list->get(pos, stu);
		return true;
	}
	
	// not found
	return false;
}

bool computeAndDisplayStatistics(List* list)
{
	// early return if list is empty
	if (list->empty())
		return false;

	int numCS = 0, numIA = 0, numIB = 0, numCN = 0, numCT = 0;
	int totalBooksCS = 0, totalBooksIA = 0, totalBooksIB = 0, totalBooksCN = 0, totalBooksCT = 0;
	int overdueBooksCS = 0, overdueBooksIA = 0, overdueBooksIB = 0, overdueBooksCN = 0, overdueBooksCT = 0;
	double totalFineCS = 0.0, totalFineIA = 0.0, totalFineIB = 0.0, totalFineCN = 0.0, totalFineCT = 0.0;

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

bool printStuWithSameBook(List *list, char *callNum) {
	if (list->empty()) {
		cout << "The list is empty.\n";
		return false; // Return false if the list is empty
	}

	bool found = false;

	Node* currentNode = list->head;
	while (currentNode != NULL) {
		LibStudent student = currentNode->item;

		for (int i = 0; i < student.totalbook; i++) {
			if (strcmp(student.book[i].callNum, callNum) == 0) {
				if (!found) {
					cout << "There are students that borrow the book with call number " << callNum << " as shown below:\n\n";
					found = true;
				}

				cout << "Student Id = " << student.id << "\n";
				cout << "Name = " << student.name << "\n";
				cout << "Course = " << student.course << "\n";
				cout << "Phone Number = " << student.phone_no << "\n";
				cout << "Borrow Date: ";
				student.book[i].borrow.print(cout);
				cout << "\nDue Date: ";
				student.book[i].due.print(cout);
				cout << "\n\n";
			}
		}

		currentNode = currentNode->next;
	}

	if (!found) {
		cout << "There is no students found who borrowed the book with the specified call number.\n";
	}

	return found;
}

bool displayWarnedStudent(List* list, List* type1, List* type2) {

    if (list->empty()) {
        return false;
    }

    Node* cur;
    cur = list->head;
    LibStudent Alvin;
    while (cur != NULL) {
        int count = 0;
        int y = 0;
        // check if the student has 2 or more books overdue for 10 days
        for (y = 0; y < cur->item.totalbook; y++) {
            // overdue 10 days so fine is 10 * 0.5 = 5
            if (cur->item.book[y].fine > 5) {
                count++;
                if (count >= 2) {
                    Alvin = cur->item;
                    type1->insert(Alvin);
                    break;
                }
            }
        }
        count = calculateTotalFineBooks(cur->item);
        if (cur->item.total_fine >= 50 && count == cur->item.totalbook) {
            Alvin = cur->item;
            type2->insert(Alvin);
        }
        cur = cur->next;
    }
    
    return true;
}
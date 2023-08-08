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

//extra function
int calculateTotalFineBooks(LibStudent &student);
void updateCourseStatistics(List* list, const char* courseCode, int& numStudents, int& totalBooks, int& overdueBooks, double& totalFine);


int main() {
	

	cout << "\n\n";
	system("pause");
	return 0;
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
	cout << "\nCourse\tNumber of Students\tTotal Books Borrowed\tTotal Overdue Books\tTotal Overdue Fine (RM)\n";
	cout << "CS\t" << numCS << "\t" << totalBooksCS << "\t" << overdueBooksCS << "\t" << totalFineCS << endl;
	cout << "IA\t" << numIA << "\t" << totalBooksIA << "\t" << overdueBooksIA << "\t" << totalFineIA << endl;
	cout << "IB\t" << numIB << "\t" << totalBooksIB << "\t" << overdueBooksIB << "\t" << totalFineIB << endl;
	cout << "CN\t" << numCN << "\t" << totalBooksCN << "\t" << overdueBooksCN << "\t" << totalFineCN << endl;
	cout << "CT\t" << numCT << "\t" << totalBooksCT << "\t" << overdueBooksCT << "\t" << totalFineCT << endl;

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

/*older version replace by update function	
	int numFineBooks = 0;
	for (int i = 1; i <= list->size(); i++)
	{
		LibStudent student;



		if (list->get(i, student))
		{
			if (strcmp(student.course, "CS") == 0)
			{
				numCS++;
				totalBooksCS += student.totalbook;
				numFineBooks = calculateTotalFineBooks(student);
				overdueBooksCS += numFineBooks;
				totalFineCS += student.total_fine;
			}

			else if (strcmp(student.course, "IA") == 0)
			{
				numIA++;
				totalBooksIA += student.totalbook;
				numFineBooks = calculateTotalFineBooks(student);
				overdueBooksIA += numFineBooks;
				totalFineIA += student.total_fine;
			}

			else if (strcmp(student.course, "IB") == 0)
			{
				numIB++;
				numFineBooks = calculateTotalFineBooks(student);
				overdueBooksIB += numFineBooks;
				totalFineIB += student.total_fine;
			}

			else if (strcmp(student.course, "CN") == 0)
			{
				numCN++;
				numFineBooks = calculateTotalFineBooks(student);
				overdueBooksCN += numFineBooks;
				totalFineCN += student.total_fine;
			}

			else if (strcmp(student.course, "CT") == 0)
			{
				numCT++;
				numFineBooks = calculateTotalFineBooks(student);
				overdueBooksCT += numFineBooks;
				totalFineCT += student.total_fine;
			}
		}
	}
	*/
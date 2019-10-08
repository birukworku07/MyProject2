// Project2_.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Author: Biruk Kassahun Worku
// Date  : 2/23/2019



#include "pch.h"
#include <iostream>
#include<vector>
#include<ostream>
using namespace std;

// class declaration for the ExceptionAdd 
template<class T>
class ExceptionAdd {};

// class declaration for the ExceptionMultiply
template<class T>
class ExceptionMultiply {};

// class declaration for the ExceptionCV
template <class T>
class ExceptionCV {};

// This is the sparse row class that assists in the 
// creation of the sparse matrix representation structure

template <class T>
class SparseRow {
protected:
	int row; // row# 
	int col; // col#
	T value; //value 
public:
	SparseRow(); //default constructor
	SparseRow(int r, int c, T& v); // parametrized constructor
	virtual ~SparseRow(); // destructor
	void display(); // display method
	int getRow(); // gets the row#
	int getCol(); // gets the col#
	T getValue(); //gets the value
};
// implementation of the default constructor. Setting row# = -1, col# = -1, and value = 0
template<class T>
SparseRow<T>::SparseRow() 
{ 
	row = -1;
	col = -1;
	value = 0;
}
// implementation the parametrized constructor
template<class T>
SparseRow<T>::SparseRow(int r, int c, T& v) {
	row = r;
	col = c;
	value = v;
}

template<class T>
SparseRow<T>::~SparseRow() {

}
// implementation the display method to print a single sparse row with "row, col, value" format
template<class T>
void SparseRow<T>::display() {
	cout << row << ", " << col << ", " << value << endl;
}
// a getter for the row#
template <class T>
int SparseRow<T>::getRow() {
	return row;
}
// a getter for the col#
template <class T>
int SparseRow<T>::getCol() {
	return col;
}
// a getter for the value
template <class T>
T SparseRow<T>::getValue() {
	return value;
}


// This is a SparseMatrix class that creates a sparse matrix representation of the input matrices -
// and performs simple mathematical matrix operations

template <class T>
class SparseMatrix 
{
protected:
	int noRows; // number of rows of original matrix
	int noCols; // number of column of original matrix
	int commonValue; // The common value
	vector<SparseRow<T> >* myMatrix; // a dynamically created vector
public:
	SparseMatrix (); // default constructor
	virtual ~SparseMatrix (); // destructor
	SparseMatrix(int n, int m, int cv); // parametrized constructor
	void setSparseRow(int r, int c, T& v); // a sparseRow setter
	SparseMatrix<T>* operator*(SparseMatrix<T>M); // multiplication(*) operator overload
	SparseMatrix<T>* operator+ (SparseMatrix<T>M); // addition (+) operator overload
	SparseMatrix<T>* operator!(); // not(!) operator overload
	void display(); // displays the matrix in a sparse matrix form
	void displayMatrix(); // displays the matrix in a normal form
	friend ostream& operator<< (ostream& out, SparseMatrix<T>& M) // a friend function for the insertion (<<) operator overloading
	{
		 M.display();
		 return out;
		
	}
	int getNoRows(); // gets the number of rows of the original matrix
	int getNoCols(); // gets the number of columns of the original matrix
	int getCommonValue(); // gets the common value of the matrix
	T getMyValue(int r, int c); // a helper method used to extract value given its row and column

};


template <class T>
SparseMatrix<T> ::SparseMatrix ()
{
}
//implementation of the destructor to perform a deep delete to every sparse row object stored in myMatrix
template <class T>
SparseMatrix <T>::~SparseMatrix ()
{
	for (unsigned int i = 0; i < myMatrix->size(); ++i) {
		SparseRow<T>* s = new SparseRow<T>(myMatrix->at(i));
		delete s;
	}
	
}
// implementation of the SparseMatrix parametrized constructor
template <class T>
SparseMatrix<T> ::SparseMatrix(int n, int m, int cv)
{
	noRows = n;
	noCols = m;
	commonValue = cv;
	myMatrix = new vector<SparseRow<T> >();
}
//implementation of the setSparseRow method
template <class T>
void SparseMatrix<T>::setSparseRow(int r, int c, T& v) {
	SparseRow<T>* row = new SparseRow<T>(r, c, v); // setting the sparseRow with the given arguments
	(*myMatrix).push_back(*row);  // storing it in the myMatrix vector
	delete row; // deleting the dynamically created object, row.

}
// implementation of the multiplication(*) operator
template <class T>
SparseMatrix<T>* SparseMatrix<T>::operator*(SparseMatrix<T> M) throw(ExceptionMultiply<int>)
{
	// checking if the number of columns of the first matrix is equal to the number of rows of the second
	if ((*this).noCols != M.noRows) {
		throw ExceptionMultiply<int>();
	}
	
	SparseMatrix<T>* temp = new SparseMatrix<T>(noRows, M.noCols, commonValue);
	T productVal;

	// looping through the number of rows
	for (int i = 0; i < noRows; ++i) {
		for (int j = 0; j < noCols; ++j) {
			productVal = 0;
			for (int k = 0; k < M.noRows; ++k) {
				productVal += (*this).getMyValue(i, k) * M.getMyValue(k, j);

			}

			// checking if the product is not a common vlaue
			if (productVal != commonValue) {

				// setting the row
				(*temp).setSparseRow(i, j, productVal);
				
			}

		}
	}
	return temp;
	
	

	
}
// implementation of the addition(+) operator
template <class T>
SparseMatrix<T>* SparseMatrix<T>::operator+(SparseMatrix<T> M) throw(ExceptionAdd<int>)
{
	// checking if the row and col of both matrices are the same
	if (((*this).noRows != M.noRows) || ((*this).noCols != M.noCols)) {
		throw ExceptionAdd<int>();
	}

	T resValue;
	SparseMatrix<T>* temp = new SparseMatrix<T>(noRows, noCols, commonValue);
	T valueToAdd = commonValue;
	bool found;
	
	// A boolean array that checks if the row number and column numbers are present in both matrices
	bool* checkArray = new bool[M.myMatrix->size()];
	// setting all elements in the checkArray to false
	for (unsigned int k = 0; k < M.myMatrix->size(); ++k) {
		checkArray[k] = false;
	}
	// looping through the firstMatrix row
	for (unsigned int i = 0; i < myMatrix->size(); ++i) {
		found = false;
		valueToAdd = commonValue; // initially assigning valueToAdd to common value

		for (unsigned int j = 0; (j < M.myMatrix->size() && (!found)); ++j) {

			
			if (myMatrix->at(i).getRow() == M.myMatrix->at(j).getRow() && myMatrix->at(i).getCol() == M.myMatrix->at(j).getCol()) {
				found = true; // setting found to true
				valueToAdd = M.myMatrix->at(j).getValue(); // setting value to be added to the value of the second matrix at this particular position
				checkArray[j] = true;


			}


		}
	
		resValue = (*this).myMatrix->at(i).getValue() + valueToAdd;
		if (resValue != commonValue) // checking if the resValue is not a common value
		{
			(*temp).setSparseRow((*this).myMatrix->at(i).getRow(), (*this).myMatrix->at(i).getCol(), resValue);
			
		}
		
	}

	// for loop that goes through the rows of the second sparse matrix
	for (unsigned int h = 0; h < M.myMatrix->size(); ++h) {
		// going through the checkArray
		if (!checkArray[h]) {
			T resultant = M.myMatrix->at(h).getValue() + commonValue;
			if (resultant != commonValue)  // checking if the resultant is not a common value
			{
				(*temp).setSparseRow(M.myMatrix->at(h).getRow(), M.myMatrix->at(h).getCol(), resultant); // setting the sparse row
			}
		}

	}
	delete[] checkArray; // deleting the dynamically created checkArray 
	return temp;

}
// implementing the (!) operator overload
template <class T>
SparseMatrix<T>* SparseMatrix<T>::operator!() {
	int colT, rowT;
	T valueT;
	SparseMatrix<T>* temp = new SparseMatrix<T>(noRows, noCols, commonValue);
	// going through my first matrix row
	for (unsigned int i = 0; i < ((*myMatrix).size()); ++i) {
		rowT = myMatrix->at(i).getCol();
		colT = myMatrix->at(i).getRow();
		valueT = myMatrix->at(i).getValue();
		(*temp).setSparseRow(rowT, colT, valueT); // setting the sparseRow
	}
	return temp;
}
// implementing the display method
template <class T>
void SparseMatrix<T>::display() {
	for (unsigned int i = 0; i < ((*myMatrix).size()); ++i) {
		(*myMatrix).at(i).display();
	}
}
// implementing the display matrix method
template <class T>
void SparseMatrix<T>::displayMatrix() {
	for (int i = 0; i < noRows; ++i) {
		for (int j = 0; j < noCols; ++j) {
			cout << (*this).getMyValue(i, j) << '\t';

		}
		cout << endl;

	}
}
// getting the number of rows
template <class T>
int SparseMatrix<T>:: getNoRows() {
	return noRows;
}
// getting the number of cols
template <class T>
int SparseMatrix<T>::getNoCols() {
	return noCols;
}
// getting the common value
template <class T>
int SparseMatrix<T>::getCommonValue() {
	return commonValue;
}
// implementing the helper method to get the value at a specific row and column 
template <class T>
T SparseMatrix<T>::getMyValue(int r, int c) {
	int result = commonValue;
	for (unsigned int i = 0; i < ((*myMatrix).size()); ++i) // going through the first matrix row
	{ 
		if (myMatrix->at(i).getRow() == r && myMatrix ->at(i).getCol() == c) {
			result = myMatrix->at(i).getValue();
		}
	}
	return result;
	
}



int main()
{
	// declaring integers for number of rows, number of coloumn, commonvalue, number of non sparsevalue and for the value the user inputs
	int n, m, cv, val;

	// temporary SparseMatrix
	SparseMatrix<int>* temp;

	// storing in the dimensions of the first matrix from the user
	cin >> n >> m >> cv ;

	// creating the first matrix
	SparseMatrix<int>* firstOne = new SparseMatrix<int>(n, m, cv);
	


	// loop through the number of rows and number of columns and put value in accordance from the user
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			cin >> val;
			

			// checking if the value is not a common value
			if (val != cv) {
				(*firstOne).setSparseRow(i, j, val);
				
			}
		}

		cout << endl;
	}
	// displaying the first one in sparse matrix format
	cout << "First one in sparse matrix format" << endl;
	cout << (*firstOne);

	// displaying the first one in sparse matrix format
	cout << "First one in normal matrix format" << endl;
	(*firstOne).displayMatrix();

	// getting input dimension for the second matrix
	cin >> n >> m >> cv;
	// creating the second matrix
	SparseMatrix<int>* secondOne = new SparseMatrix<int>(n, m, cv);

	// a try-catch block to check if the common value of the two matrices meet
	try {
		if ((*secondOne).getCommonValue() != (*firstOne).getCommonValue()) {
			throw ExceptionCV<int>();
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				cin >> val;


				// checking if the value is not a common value
				if (val != cv) {
					(*secondOne).setSparseRow(i, j, val);

				}
			}

			cout << endl;
		}

		// displaying second matrix in sparse matrix format
		cout << "Second one in sparse matrix format" << endl;
		cout << (*secondOne);

		// displaying second matrix in normal format
		cout << "Second one in normal matrix format" << endl;
		(*secondOne).displayMatrix();

		
		cout << "After Transpose first one in normal format" << endl;
		// transposing the first matrix and displaying it in normal format
		temp = !(*firstOne);
		(*temp).displayMatrix();
		delete temp;

		cout << "After Transpose second one in normal format" << endl;
		// transposing the second matrix and displaying it in normal format
		temp = !(*secondOne);
		(*temp).displayMatrix();
		delete temp;

		cout << "Multiplication of matrices in sparse matrix form: " << endl;
		// nested try-catch block to catch a multiplication exception
		try {
			temp = (*firstOne) * (*secondOne);
			cout << (*temp);
			delete temp;
		}
		catch (ExceptionMultiply<int> e) {
			cout << "EXCEPTION THROWN: Number of columns of the first matrix does not match the number of rows of the second matrix" << endl;
		}
		cout << "Addition of matrices in sparse matrix form:" << endl;
		// nested try-catch block to catch an addition exception
		try {
			temp = (*firstOne) + (*secondOne);
			cout << (*temp);
			delete temp;
		}
		catch (ExceptionAdd<int> e) {
			cout << "EXCEPTION THROWN: Number of rows and columns of both matrices don't match" << endl;
		}

	}
	catch (ExceptionCV<int> e) {
		cout << "EXCEPTION THROWN: The common value of the first matrix doesn't match with the second matrix" << endl;
	}

	delete firstOne; // deleting the dynamically created object - firstOne
	delete secondOne; // deleting the dynamically created object - secondOne
	

	return 0;
	
   
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

//================================================
// Name        : CS 300 Final
// Author      : Cory Graham
// Version     : 1.0
// Description : CS300 Project 2
//================================================

#include<iostream>
#include<fstream>
#include<vector>
#include<string>


using namespace std;



// structure to define a course
struct Course {
	string courseId;
	string courseName;
	vector<string> preReq;
};



//Internal structure for a tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor 
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//initialize with a coures
	Node(Course aCourse) : Node() {
		course = aCourse;
	}
};



// Class to create a BST
class BinarySearchTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	Course Search(string courseId);
};



// Default constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}


// Sends info to be organized
void BinarySearchTree::InOrder() {
	inOrder(root);
}


// Program will insert nodes upon upload
void BinarySearchTree::Insert(Course course) {

	if (root == nullptr) {       // if no nodes yet exist, then node becomes root
		root = new Node(course);
	}
	else {                       // else it's sent to be placed 
		this->addNode(root, course);
	}
}


// Locate specific nodes
Course BinarySearchTree::Search(string courseId) {

	Node* cur = root;

	while (cur != nullptr) {
		if (cur->course.courseId == courseId) {
			return cur->course;
		}

		else if (courseId < cur->course.courseId) {
			cur = cur->left;
		}

		else {
			cur = cur->right;
		}
	}

	// courseId not located
	Course course;
	return course;
}


// Organizing new nodes
void BinarySearchTree::addNode(Node* node, Course course) {

	if ((node != nullptr) && (course.courseId < node->course.courseId)) { // if new node is less than the cur node go left

		if (node->left == nullptr) { // if left is vacant place here

			node->left = new Node(course);

			return;
		}
		else {
			this->addNode(node->left, course); // else cur node is now the left node
		}
	}
	else if ((node != nullptr) && (course.courseId > node->course.courseId)) { // if new node is greater than the cur node go right

		if (node->right == nullptr) {  // if right is vacant place here

			node->right = new Node(course);

			return;
		}
		else {                                       // else cur node is now the right node
			this->addNode(node->right, course);
		}
	}
}



// navigates the BST to print from the left to the right
void BinarySearchTree::inOrder(Node* node) {  //no node specified so start at root
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseId << ", " << node->course.courseName << endl;
		inOrder(node->right);
	}

}



vector<string> Split(string lineFeed) { // takes the line from the excel sheet and diveds them by "," and puts the results into vectors
	
	char delim = ',';

	lineFeed += delim;

	vector<string> lineTokens;
	string temp = "";

	for (int i = 0; i < lineFeed.length(); i++) {

		if (lineFeed[i] == delim) {

			lineTokens.push_back(temp);
			temp = "";
			i++;
		}

		temp += lineFeed[i];
	}

	return lineTokens;
}


void printPreReq(Course course) {  // Takes the vectors of pre reqs and prints them out by course

	cout << course.courseId << ", " << course.courseName << endl;
	cout << "Prerequisites: ";

	if (course.preReq.empty()) {
		cout << "none" << endl;
	}
	else {
		for (int i = 0; i < course.preReq.size(); i++) {
			cout << course.preReq.at(i);

			if (course.preReq.size() > 1 && i < course.preReq.size() - 1) {
				cout << ", ";
			}
		}

		cout << endl;
		cout << endl;
	}
}



void caseCheck(string &courseKey) {  // Makes sure that the input is in the correct format

	for (int i = 0; i < courseKey.length(); i++) {

		if (isalpha(courseKey[i])) {

			courseKey[i] = toupper(courseKey[i]);

		}
	}
}



//Load Data
void fileInput(string csvPath, BinarySearchTree* courseList) {
	
	ifstream inFS;
	string line;
	vector<string> stringTokens;

	inFS.open(csvPath);

	bool trigger = true;

	if (!inFS.is_open()) {
		cout << "File failed to open. Please check file name or file location." << endl;
		return;
	}

	while (trigger) {
		Course aCourse;

		getline(inFS, line);
		stringTokens = Split(line); // Line is split by delimiter

		if (stringTokens.size() < 2) { //checks to make sure format is correct 
			if (inFS.eof()) { // error may be due to the end of file
				trigger = false;
			}
			else {
				cout << "ERROR. CHECK FORMATTING" << endl;
			}
		}

		else {
			aCourse.courseId = stringTokens.at(0);
			aCourse.courseName = stringTokens.at(1);

			for (int i = 2; i < stringTokens.size(); i++) {
				
				aCourse.preReq.push_back(stringTokens.at(i)); // takes prerequisits and keeps them by the couse
			}

			courseList->Insert(aCourse);
		}
	}

	inFS.close();

}




void showMenu() {
	cout << "1. Load Data Structure." << endl;
	cout << "2. Print Course List." << endl;
	cout << "3. Print Course." << endl;
	cout << "9. Exit" << endl;
}



int main() {

	int choice = 0;
	string courseKey;
	string fileName;
	string testName;

	// BST to hold all entries
	BinarySearchTree* courseList = new BinarySearchTree();
	Course course;

	testName = "CS 300 ABCU_Advising_Program_Input.csv";


	cout << "Welcome to the course planner." << endl;
	cout << endl;

	while (choice != 9) {

		showMenu();
		cout << endl;
		cout << "What would you like to do? ";

		
		cin >> choice;


		switch (choice) {

		case 1:
			cout << "Enter file name:" << endl;
			cin >> fileName;

			if (fileName == "Cory") {  //Easy way to test the file currently in the program
				fileName = testName;
			}

			fileInput(fileName, courseList);

			cout << "Upload successful." << endl;

			break;

		case 2:

			cout << endl;
			cout << "Here is a sample schedule:" << endl;

			courseList->InOrder();

			cout << endl;

			break;

		case 3:

			cout << "What course would you like to know about? Enter course ID: ";
			cin >> courseKey;

			caseCheck(courseKey);

			course = courseList->Search(courseKey);

			if (!course.courseId.empty()) {
				printPreReq(course);
			}
			else {
				cout << "Course not found." << endl;
			}

			break;

		default:

			cout << choice << " is not a valid option." << endl;
			cout << endl;
			break;
		}

	}
	cout << "Thank you for using the course planner!" << endl;
	return 0;


}
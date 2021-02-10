/*
Gotta make this boy a library later down the line.
nlemus@umich.edu
Version 1. 2/9/21 
Issues:
-Convert to functions
-Make code work for general cases
-Verify logic with other cipher sets
*/
/*
-Read cipher text from a file and copy it into a local array
	>Count how many total elements there are
-Sort the cipher text based on letter order
-Count how many of each letter there are
-Calculate the percentage of each letter's expression (letter/total # of letters)
-Perform dot product operation with the standard english letter frequencies set
-Shift frequencies
	>record shift amount each time
-Find highest dot product operation result
	>Print out the shift amount which resulted in the highest value
	>This will be the shift amount which the ciphertext should be shifted by
	>Analyze the shifted ciphertext by hand. 
		>Break up into words to find plain text message.
*/
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <algorithm> //for the sort function. http://www.cplusplus.com/reference/algorithm/sort/

using namespace std;

/*  FIX ISSUES
void shiftCipher(float *letterFreq[], int sizeOfArray);

void shiftCipher(float *letterFreq[], int sizeOfArray) { //Shift array one element to the left

	float temp = *letterFreq[0]; //Store First element to place at the end of the array after shift
	for (int i = 0; i < sizeOfArray; i++) {
		*letterFreq[i] = *letterFreq[i + 1]; //move every element one to the left
	}
	*letterFreq[sizeOfArray - 1] = temp; //Place the stored element at the end of the array
}
*/

void sortArray(char inputText[], int inputSize);

void sortArray(char inputText[], int inputSize) { //Sort array least to greatest
	sort(inputText, inputText+inputSize); //sort from beginning of array to the end of the array. 
	/*If the cipher text is much longer, there is performance to be gained by breaking up the sort range into smaller sections.
	ie: if length = 1000, then sort(0,99), sort(100,199) ... sort(900, 1000), followed by sort(0,1000).
	For the length of < 150, one sort should not leave much performance on the table also considering only one text is being deciphered.*/
	for (int i = 0; i < inputSize; i++) {
		cout << inputText[i]; //This is just to verify the text has been sorted.
	}
}

float dotProduct(float letterFreq[], float stdFreq[], int arraySize);

float dotProduct(float letterFreq[], float stdFreq[], int arraySize) { //calculate dot product between two arrays
	float DP = 0;

	for (int i = 0; i < arraySize; i++) {
		DP += (letterFreq[i] * stdFreq[i]); //dot product = a1*b1 + a2*b2 + .... an*bn
	}

	return DP;
}


int main() {

	char inputText[100] = {}; //blank array of 100 elements for the cipher text
	int inputSize = 0; //will be used to track how long the cipher text actually is

	float enAlphaFreq[] = {8.15,1.44,2.76,3.79,13.11,2.92,1.99,5.26,6.35,0.13,0.42,3.39,2.54,7.10,8.00,1.98,0.12,6.83,6.10,10.47,2.46,0.92,1.54,0.17,1.98,0.08}; //based on lecture notes
	float cipherFreq[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //set them all to zero to start with
	float cipherCounts[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; //" "

	//cout << "Enter the name of the input file (cipher text)." << endl;
	//string textFile;
	//cin >> textFile;
	ifstream cipherText;
	ofstream decipheredText;
	decipheredText.open("decipheredText.txt"); 
	//cipherText.open(textFile);
	cipherText.open("CeaserCipherText.txt");


	cout << "Cipher text:        ";
	if (cipherText.is_open()) {
		int i = 0;
		while (!cipherText.eof()) { //Collecting ciphertext. This assumes the letters are all together.
			cipherText >> inputText[i];
			cout << inputText[i];
			i++;
			inputSize++;
		}
		cipherText.close();
	}
	else cout << "Unable to open file.";

	cout << endl << "Sorted cipher text: "; //The sorted cipher text is printed in the function call.

	sortArray(inputText, inputSize); //sort input cipher text by letter value

	int currentArrayIndex = 0;
	int currentLetter = 0;
	int letterCount = 0;

	for (int i = 1; i < inputSize; i++) { //loop through array of sorted cipher text
		int inputChar = (int)inputText[i] - 65; //cast to int then offset based on ascii value.

		if (inputChar == currentLetter) { //Increases count of current letter. (When there are multiples of a letter)
			letterCount++;
		}
		else if (inputChar == currentLetter + 1) { //Start counting the next letter
			letterCount = 1;
			currentArrayIndex++;
			currentLetter++; 
		}
		else if (inputChar > currentLetter) { //This will catch the case that there are letters with no expression (A letter is not in the cipher text) inbetween letters.
			currentLetter = inputChar;
			letterCount = 1;
			currentArrayIndex = inputChar;
			currentLetter = inputChar;
		}
		else { //For when a letter has no expression
			currentArrayIndex++;
			currentLetter++;
			letterCount = 0;
		}
		cipherCounts[currentArrayIndex] = letterCount; //Update the array with the counts of each letter
	}

	for (int i = 0; i < 26; i++) {
		float tempCount = cipherCounts[i];
		cipherFreq[i] = tempCount / inputSize;
	}

	float dotProductResult = 0;
	float maxDotProduct = 0;
	float previousMax = 0;
	int shiftAmount = 0;

	for (int i = 0; i < 26; i++) {

		dotProductResult = dotProduct(cipherFreq, enAlphaFreq, 26);
		maxDotProduct = max(dotProductResult, maxDotProduct);

		float temp = cipherFreq[0]; //Store First element to place at the end of the array after shift
		for (int i = 0; i < 26; i++) {
			//shiftCipher(&cipherFreq[], 26); //FIX ISSUES
			if (i < 25) {
				cipherFreq[i] = cipherFreq[i + 1]; //move every element one to the left
			}
			else {
				cipherFreq[25] = temp;
			}
		}

		if (maxDotProduct != previousMax) { //For recording the shift amount of the highest dot product result
			shiftAmount = i;
			previousMax = maxDotProduct;
		}
	}

	cout << endl << endl << "The highest dot product result is: " << maxDotProduct;
	cout << endl << "The shift amount that results in the highest dot product result is: " << shiftAmount;
	cout << endl << endl;

	return 0;
}


/*
cout << endl << endl << "The cipher text following a shift by " << shiftAmount << " :" << endl;

	float tempVal = inputCipher[0]; //Store First element to place at the end of the array after shift
	for (int i = 0; i <= 9; i++) {
		for (int i = 0; i <= inputSize; i++) {
			//shiftCipher(&cipherFreq[], 26); //FIX ISSUES
			if (i < inputSize - 1) {
				inputCipher[i] = inputCipher[i + 1]; //move every element one to the left
			}
			inputCipher[inputSize - 1] = tempVal;
		}
	}

	for (int i = 0; i < inputSize; i++) {
		cout << inputCipher[i]; //This is just to verify the text has been sorted.
	}

	cout << endl << endl << "The plain text following a shift transformation:" << endl;
*/
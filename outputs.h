#ifndef OUTPUTS_H
#define OUTPUTS_H
#include <iostream>
using namespace std;


void firstWindow() {
    cout << "1: Manage Tournament" << endl << "2: View Past Tournament"<< endl << "3.Exit" << "\nEnter: ";
}

void createTourWindow(){
    cout << "1: Create new tournament" << endl << "2.Edit a ongoing tournament" << endl << "3.Exit"<< "\nEnter: ";
}


/*
This window is for the user to pick a tournment they have main i think to ideally do this they shouold get the tournament from a
cvsfile and then but would rather pick out a tournment from a list they have from a website if we want to convert this to a website
*/

void editTourWindow() {
    cout << "1: Add player" << endl << " 2:Edit scores" << endl << "3: Create Pairings" << endl << "4: Exit" << "\nEnter:  ";
}



void errorMessageValue(){
    cout << "Invalid number, Please re-enter:  ";
}



#endif // OUTPUTS_H

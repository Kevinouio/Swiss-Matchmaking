#ifndef EDITTOURMAIN_H
#define EDITTOURMAIN_H
#include "outputs.h"
#include "string"
#include <fstream>
#include <vector>
#include "person.h"
#include "createPairings.h"

using namespace std;

void addPlayer(string tourName, vector<Person> &people, int rounds) {
    Person player;
    string rating;
    string name;
    string USCFID;
    double score = 0.0;
    string fileInput = "";
    vector<string> stringlines;
    ifstream txtinFile(tourName + "Players.txt");

    //Gets each player that was already in the list
    while (!txtinFile.eof()) {
        getline(txtinFile,fileInput);
        stringlines.push_back(fileInput);
    }

    txtinFile.close();
    ofstream txtFile(tourName + "Players.txt");
    //Rewrites the players in the txt file
    for (int i = 0; i < stringlines.size(); i++) {
        if (i == stringlines.size()-1) {
            txtFile << stringlines.at(i);
            continue;
        }
        txtFile << stringlines.at(i) << endl;
    }

    cin.ignore();
    cout << "Name of Player?\nEnter:  ";
    getline(cin, name);
    player.setName(name);
    txtFile << name << endl;


    bool notNum = true;
    while (notNum) {
        cout <<"Rating of Player?\nEnter:  ";
        cin >> rating;
        for (int i = 0; i < rating.size(); i++) {
            if (!(isdigit(rating.at(i)))) {
                errorMessageValue();
                notNum = true;
                break;
            }
            notNum = false;
        }
    }
    player.setRating(stoi(rating));
    txtFile << stoi(rating) << endl;

    cin.ignore();
    cout <<"USCFID of Player?(Type NA if player does not have one)\nEnter:  ";
    getline(cin,USCFID);
    if ("NA" == USCFID) {
        USCFID = "00000000";
    }
    player.setUSCFID(USCFID);
    txtFile << USCFID << endl;

    player.setScore(score);
    txtFile << score << endl;

    player.setMatchHistory(rounds);
    for (int i = 0; i < rounds; i++) {
        txtFile << "DNE" << endl;
    }

    people.push_back(player);

}

void getCurrentPlayers(string tourName, vector<Person> &people, int &rounds) {
    ifstream inFile(tourName + "Players.txt");
    string fileInput;

    while (!inFile.eof()) {
        Person player;
        vector<string> matches;

        getline(inFile, fileInput);
        player.setName(fileInput);
        if (inFile.eof()){
            break;
        }
        getline(inFile,fileInput);
        player.setRating(stoi(fileInput));

        getline(inFile, fileInput);
        player.setUSCFID(fileInput);


        getline(inFile, fileInput);
        player.setScore(stod(fileInput));



        for (int i = 0; i < rounds; i++) {
            getline(inFile, fileInput);
            matches.push_back(fileInput);
        }

        player.setCurHistory(matches);

        people.push_back(player);

    }
    inFile.close();
    // Insert a sorting algorithm right here for scores first and then by ratings
}

void viewLeaderboard(vector<Person> &people, string tourName, int rounds) {
    string csvLine;

    ofstream cvsLeaderboard(tourName + "Leaderboard.csv");

    csvLine = "Placement,Name,Rating,Score,";
    for(int i = 0; i < rounds; i++) {
        if(i == rounds - 1) {
            csvLine += "Round " + to_string(i+1);
            break;
        }
        csvLine += "Round " + to_string(i+1) + ",";
    }
    cvsLeaderboard << csvLine << endl;

    // I don't really know how you want to display this because this is just the leaderboard all of the infor for this is going to be in the leaderboard file
    //Also put sorting algorithm in here as well

    for (int i = 0; i < people.size(); i++) {
        vector<string> curPlayerHistory;

        csvLine = to_string(i+1) + ",";
        csvLine += people.at(i).getName() + ",";
        csvLine += to_string(people.at(i).getRating()) + ",";
        csvLine += to_string(people.at(i).getScore()) + ",";

        curPlayerHistory = people.at(i).getMatchHistory();

        for(int i = 0; i < curPlayerHistory.size(); i++) {
            if(i == rounds - 1) {
                csvLine += curPlayerHistory.at(i);
                break;
            }

            csvLine += curPlayerHistory.at(i) + ",";
        }
        cvsLeaderboard << csvLine << endl;
    }
}

int getRounds(string tourName) {
    ifstream inFile(tourName + ".csv");
    string fileLine = "";
    vector<string> tokens;

    getline(inFile, fileLine);
    while (getline(inFile, fileLine, ',')) {
        tokens.push_back(fileLine);
    }

    return stoi(tokens.at(5));


}

int getCurrRounds(string tourName) {
    ifstream inFile(tourName + ".csv");
    string fileLine = "";
    vector<string> tokens;

    getline(inFile, fileLine);
    while (getline(inFile, fileLine, ',')) {
        tokens.push_back(fileLine);
    }

    return stoi(tokens.at(6));


}

void updateCurrRound(int &currRound, string tourName) {
    vector<string> resultList;
    string fileLine;
    string tourInfo;
    bool allMatchesDone = true;
    ifstream inFile(tourName+ "Leaderboard.csv");
    getline(inFile, fileLine);
    while(!inFile.eof()) {
        for (int i = 0; i < currRound + 3; i++){
            getline(inFile, fileLine, ',');
        }
        getline(inFile, fileLine, ',');
        resultList.push_back(fileLine);
        getline(inFile, fileLine);
    }

    inFile.close();
    for (int i = 0; i < resultList.size(); i++) {
        if (resultList.at(i) == "DNE") {
            allMatchesDone = false;
        }

    }


    if (allMatchesDone) {
        currRound += 1;
    }
    resultList.clear();
    inFile.open(tourName + ".csv");


    getline(inFile, fileLine);
    resultList.push_back(fileLine);

    getline(inFile, fileLine,'+');
    tourInfo = fileLine + "+";

    getline(inFile, fileLine,',');
    tourInfo += fileLine + ",";

    getline(inFile, fileLine,',');
    tourInfo += fileLine + ",";

    tourInfo += to_string(currRound);
    resultList.push_back(tourInfo);

    inFile.close();

    ofstream outFile(tourName + ".csv");
    for (int i = 0; i < resultList.size(); i++) {
        outFile << resultList.at(i) << endl;
    }










}

vector<vector<Person>> getPairings(string tourName, int currRound, vector<Person> people) {
    vector<vector<Person>> matches;
    vector<string> csvLine;
    string fileLine;
    Person player;
    ifstream inFile (tourName + "Round_" + to_string(currRound) + ".csv");
    vector<string> names;
    vector<Person> matchedPeople;

    getline(inFile, fileLine);
    getline(inFile, fileLine);

    while (!inFile.eof()) {

        getline(inFile,fileLine, ',');
        getline(inFile,fileLine, ',');
        getline(inFile,fileLine, ',');
        getline(inFile,fileLine, '(');
        names.push_back(fileLine);
        getline(inFile,fileLine, ',');
        getline(inFile,fileLine, ',');
        getline(inFile,fileLine, ',');
        getline(inFile, fileLine, '(');
        names.push_back(fileLine);
        getline(inFile,fileLine, ',');
        getline(inFile,fileLine, ',');

    }
    inFile.close();



    for (int i = 0;i < names.size();i++) {
        for (int j = 0; j < people.size(); j++) {
            if (people.at(j).getName() == names.at(i)) {
                matchedPeople.push_back(people.at(j));
                break;
            }
        }
    }
    while (matchedPeople.size() != 0) {
        vector<Person> pairs;
        pairs.push_back(matchedPeople.at(0));
        matchedPeople.erase(matchedPeople.begin());
        pairs.push_back(matchedPeople.at(0));
        matchedPeople.erase(matchedPeople.begin());
        matches.push_back(pairs);
    }
    return matches;
}

void removePlayer(vector<Person> &people) {


}

void updatePlayers(vector<Person> &people, string tourName) {
    string fileInput;
    vector<string> stringlines;
    ofstream txtFile(tourName + "Players.txt");
    //Rewrites the players in the txt file

    for (int i = 0; i < people.size(); i++ ) {
        txtFile << people.at(i).getName() << endl;

        txtFile << people.at(i).getRating() << endl;

        txtFile << people.at(i).getUSCFID() << endl;

        txtFile << people.at(i).getScore() << endl;

        vector<string> matchHistory = people.at(i).getMatchHistory();

        for (int j = 0; j < matchHistory.size(); j++) {
            txtFile << matchHistory.at(j) << endl;
        }
    }
    txtFile.close();
}

void updateScore(vector<Person> &people, string tourName, vector<vector<Person>> &matches, int currRound) {
    string color = "";
    int index;
    string matchNum = "0";
    string result;
    string tempResult;
    bool notNum = true;
    while (notNum) {

        cout << "Which match would you like to update? \nEnter:  ";
        for (int i = 0; i < matches.size(); i++) {
            for (int j = 0; j < matches.at(i).size(); j++) {
                cout << matches.at(i).at(j).getName() + "\t";
            }
            cout << endl;
        }



        cin >> matchNum;
        for (int i = 0; i < matchNum.size(); i++) {
            if (!(isdigit(matchNum.at(i)))) {
                errorMessageValue();
                notNum = true;
                break;
            }
            notNum = false;
        }


    }

    while (!((tempResult == "W") || (tempResult == "D") || (tempResult == "B"))) {
        cout << "Who won? (W=White D= Draw B=Black) \nEnter:  ";
        cin >> tempResult;
        if (((tempResult != "W") || (tempResult != "D") || (tempResult != "B"))) {
            //Insert error message
            continue;
        }
        else {
            break;
        }
    }


    for (int i = 0; i < matches.at(stoi(matchNum) - 1).size(); i++) {

        //
        for (int j = 0; people.size(); j++) {
            if (matches.at(stoi(matchNum) - 1).at(i).getName() == people.at(j).getName()) {
                index = j;
                string roundResult = people.at(j).getMatchHistory().at(currRound-1);
                if(roundResult.size() < 4) break;
                result = roundResult.at(0);

                if (result == "W") {
                    people.at(j).setScore(people.at(j).getScore()-1);
                }
                else if (result == "D") {
                    people.at(j).setScore(people.at(j).getScore()-0.5);
                }
                break;
            }

        }





        if (((i + stoi(matchNum)+1) % 2) == 0) {
            color = "WH";
            if (tempResult == "W") {
                result = "W";
            }
            else if (tempResult == "B") {
                result = "L";
            }
        }
        else {
            color = "BL";
            if (tempResult == "B") {
                result = "W";
            }
            else if (tempResult == "W") {
                result = "L";
            }
        }


        people.at(index).updateMatchHistory(currRound, result, stoi(matchNum), color);
    }
    updatePlayers(people, tourName);





}

void editTourMain() {
    ifstream inFile;
    vector<Person> people;
    string tourName;
    string userInput = "";
    int rounds;
    int currRound;
    vector<vector<Person>> matches;

    string result = "";

    cin.ignore();
    while (true) {
        cout << "What is the name of the tournmanet? \nEnter:  ";
        getline(cin,tourName);
        inFile.open(tourName + ".csv");

        if (!inFile.is_open()){
            cout << "Error opening file. Try again" << endl;
        }
        else {
            break;
        }

    }


    inFile.close();

    rounds = getRounds(tourName);
    currRound = getCurrRounds(tourName);
    getCurrentPlayers(tourName, people, rounds);



    while(1) {

        bool notNum = true;
        while (notNum) {
            editTourWindow();
            cin >> userInput;
            for (int i = 0; i < userInput.size(); i++) {
                if (!(isdigit(userInput.at(i)))) {
                    errorMessageCharacter();
                    notNum = true;
                    break;
                }
                notNum = false;
            }
        }
        if ((1 > stoi(userInput)) || (stoi(userInput) >5)) {
            errorNotAnOption();
            continue;
        }


        if (stoi(userInput) ==1) {
            addPlayer(tourName, people, rounds);
        }
        else if (stoi(userInput) ==2) {
            matches = getPairings(tourName, currRound, people);
            updateScore(people, tourName, matches, currRound);
        }
        else if (stoi(userInput) ==3) {
            viewLeaderboard(people, tourName, rounds);
            updateCurrRound(currRound, tourName);
            matches = createPairings(people, tourName, currRound);
            updatePlayers(people, tourName);
        }
        else if (stoi(userInput) == 4) {
            pigeionHoleSort(currRound + 2,people);
            viewLeaderboard(people, tourName, rounds);
        }
        else if (stoi(userInput) ==5) {
            break;
        }

    }


}


#endif // EDITTOURMAIN_H

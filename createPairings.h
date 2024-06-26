#ifndef CREATEPAIRINGS_H
#define CREATEPAIRINGS_H
#include "person.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;


// Bubble sort
void sortRatings(vector<Person> &people)
{
    for(int i = 0; i < people.size() - 1; i++)
    {
        for(int j = 0; j < people.size() - i - 1; j++)
        {
            if(people.at(j).getRating() < people.at(j+1).getRating())
            {
                Person temp = people[j];

                people[j] = people[j + 1];
                people[j + 1] = temp;
            }
        }
    }
}


// So this is going to create a pairing file in which it creaes a csv list for the pairing for the current round

//This is going to seperate the players into different vectors depending on their score.
vector<vector<Person>> pigeionHoleSort(int currRound, vector<Person> &people) {

    vector<vector<Person>> scores;
    string vectorName = "";


    for (int i = 0; i < (2 * (currRound-1) ) + 1; i++) {
        vector<Person> scoreAmount;
        scores.push_back(scoreAmount);
    }


    for(int i = 0; i < people.size(); i++) {
        //hardest line of code to write tbh but this seperates each player into their respective score group

        scores.at((people.at(i).getScore() / 0.5)).push_back(people.at(i));
    }

    for (int i = 0; i < scores.size(); i++) {
        if (scores.at(i).size() == 0) {
            continue;
        }
        sortRatings(scores.at(i));
    }


    people.clear();
    for (int i = (scores.size()-1); i >= 0; i--) {
        people.insert(people.end(), scores.at(i).begin(), scores.at(i).end());
    }


    return scores;

}

void giveBye(Person &player, int currRound){
    string result = "BYE";
    player.updateMatchHistory(currRound, result,0,"");
}


int findRank(vector<Person> people, Person player){
    int rank = 0;
    for (int i = 0; i < people.size(); i++) {
        if (player.getName() == people.at(i).getName()) {
            rank = i;
            break;
        }
    }
    return rank + 1 ;
}


bool conditions(vector<Person> &pair, int currRound, vector<Person> people) {
    // Same color poggers just swap the players in the pair list

    if (pair.size() != 2) {
        return true;
    }

    if (currRound > 2) {
        vector<vector<bool>> pairSameColor;
        vector<vector<string>> playersHistory;

        // gets the history for each person into the player's history vector.
        for(int i = 0; i < pair.size(); i++) {
            playersHistory.push_back(pair.at(i).getMatchHistory());
        }
        for (int i = 0; i < playersHistory.size(); i++) {
            vector<bool> sameColor;
            sameColor.push_back(false);
            sameColor.push_back(false);
            if((playersHistory.at(i).at(currRound -2) == "BYE") || (playersHistory.at(i).at(currRound -3) == "BYE")){
                pairSameColor.push_back(sameColor);
                continue;
            }
            char prevColor = playersHistory.at(i).at(currRound -2).at(5);
            char prevPrevColor = playersHistory.at(i).at(currRound -3).at(5);


            if(prevColor == prevPrevColor) {
                if (prevColor == 'W') {
                    sameColor.at(0) = true;
                }
                else if (prevColor == 'B') {
                    sameColor.at(1) = true;
                }
            }
            pairSameColor.push_back(sameColor);
        }
        for (int i = 0; i < pairSameColor.size(); i++) {
            if ((pairSameColor.at(0).at(i)) && (pairSameColor.at(1).at(i))) {
                return true;
            }
        }

        //Write the code for swapping the two colors
        for (int i = 0; i < pairSameColor.size(); i++) {
            if (pairSameColor.at(i).at(i)) {
                reverse(pair.begin(), pair.end());
            }
        }
    }
    // same player

    for (int i = 0; i < currRound - 1; i++ ) {
        string playerOneMatch = pair.at(0).getMatchHistory().at(i);
        string playerTwoMatch = pair.at(1).getMatchHistory().at(i);
        if(playerOneMatch.at(2) == 'E') {
            break;
        }
\
        if (playerOneMatch.at(2) == playerTwoMatch.at(2)) {
            return true;
        }
    }
    return false;
}






vector<vector<Person>> createPairings(vector<Person> &people,string tourName, int currRound) {
    //sorting algorithm for the people vector
    ofstream csvPairings(tourName + "Round_" + to_string(currRound) + ".csv");
    string csvLine;
    vector<vector<Person>> scores;
    vector<vector<Person>> tempScores;
    vector<vector<Person>> matches;
    vector<vector<vector<Person>>> scoresHistory;
    int prevShift = 0;
    int count =0;

    csvLine = ",,,White,,,Black";
    csvPairings << csvLine << endl;

    csvLine = "Match No.,Rank,Result,Player 1,Result,Rank,Player 2";
    csvPairings << csvLine << endl;


    scores = pigeionHoleSort(currRound + 2, people);
    reverse(scores.begin(),scores.end());
    tempScores = scores;

    //Round one with no conditions are being broken

    int temps = people.size()/2;
    while(matches.size() != temps) {
        int firstShift = 0;


        // iterates through the scores with their respective score starting from the top
        for (int i = 0; i < tempScores.size(); i++) {

            for(int k = 0; k < tempScores.size();k++) {
                cout << tempScores.at(k).size() << " SIZE" << endl;
            }

            //If the score is empty go to the next list of scores
            if (tempScores.at(i).size() == 0) {
                continue;
            }
            int repeats = tempScores.at(i).size()/2;
            if (tempScores.at(i).size() % 2 == 1) {
                repeats++;
            }
            if ((firstShift == (tempScores.at(i).size()-1)) && (i != tempScores.size()-1)) {
                Person player = tempScores.at(i).at(tempScores.at(i).size()-1);
                tempScores.at(i+1).push_back(player);
                tempScores.at(i).erase(tempScores.at(i).begin()+tempScores.at(i).size()-1);
                firstShift = 0;
                i-=2;
                continue;
            }

            for(int j = 0; j < repeats; j++) {

                vector<Person> pairs;
                Person player;
                int index = 0;
                bool noPairsExist = false;

                if ((tempScores.at(i).size() == 1) || ((i == tempScores.size()-1) && (tempScores.at(i).size() % 2 == 1))) {
                    if (i == tempScores.size()-1) {


                        player = tempScores.at(tempScores.size()-1).at(0);
                        for (int k = 0; k < people.size(); k++) {
                            if (player.getName() == people.at(k).getName()) {
                                index = k;
                            }
                        }
                        giveBye(people.at(index), currRound);

                        tempScores.at(i).erase(tempScores.at(i).begin());
                        continue;
                    }
                    else {
                        player = tempScores.at(i).at(0);

                        tempScores.at(i+1).push_back(player);
                        tempScores.at(i).erase(tempScores.at(i).begin());

                        continue;
                    }
                }


                int shift;
                while(conditions(pairs,currRound,people)) {
                    shift = 0;
                    pairs.clear();
                    pairs.push_back(tempScores.at(i).at(0));
                    pairs.push_back(tempScores.at(i).at((tempScores.at(i).size())-1 -firstShift));
                    while (conditions(pairs,currRound,people)){
                        pairs.clear();
                        shift++;
                        pairs.push_back(tempScores.at(i).at(0));
                        pairs.push_back(tempScores.at(i).at(tempScores.at(i).size()-shift-firstShift));
                        if (!conditions(pairs,currRound,people)) {
                            break;
                        }
                        if (shift == (tempScores.at(i).size() -1) || (shift + firstShift == (tempScores.at(i).size()-1))){

                            if (prevShift >= tempScores.at(i).size()) {
                                Person player = tempScores.at(i).at(tempScores.at(i).size()-1);
                                tempScores.at(i+1).push_back(player);
                                tempScores.at(i).erase(tempScores.at(i).begin()+tempScores.at(i).size()-1);
                                player = tempScores.at(i).at(tempScores.at(i).size()-1);
                                tempScores.at(i+1).push_back(player);
                                tempScores.at(i).erase(tempScores.at(i).begin()+tempScores.at(i).size()-1);
                                repeats--;
                                noPairsExist = true;
                                break;
                            }
                            if (scoresHistory.size() != 0) {
                                tempScores = scoresHistory.at(scoresHistory.size()-1);
                                scoresHistory.pop_back();
                                matches.pop_back();
                            }
                            noPairsExist = true;
                            firstShift +=1;
                            i =0;
                            break;
                        }

                    }

                    if (noPairsExist) {
                        break;
                    }

                }

                if ((prevShift > 0)&& (noPairsExist)) {
                    continue;
                }
                if (noPairsExist){
                    break;
                }


                scoresHistory.push_back(tempScores);
                tempScores.at(i).erase(tempScores.at(i).begin());
                prevShift = shift + firstShift;
                if (shift > 0)shift--;
                tempScores.at(i).erase(tempScores.at(i).begin()+tempScores.at(i).size()-shift-1-firstShift);
                matches.push_back(pairs);
                if (firstShift > 0) {
                    firstShift = 0;
                }
            }
        }
    }
    for (int i = 0; i < matches.size() ; i++) {
        if (i % 2 == 0) {
            csvLine = to_string(i+1) + ".," + to_string(findRank(people, matches.at(i).at(0))) + ",,";
            csvLine += matches.at(i).at(0).getName() +"(" + to_string(matches.at(i).at(0).getRating()) + "),,";

            csvLine += to_string(findRank(people, matches.at(i).at(1))) + ",";
            csvLine += matches.at(i).at(1).getName() +"(" + to_string(matches.at(i).at(1).getRating()) + "),,";
            csvPairings << csvLine << endl;
        }
        else if (i % 2 == 1) {
            csvLine = to_string(i+1) + ".," + to_string(findRank(people, matches.at(i).at(1))) + ",,";
            csvLine += matches.at(i).at(1).getName() +"(" + to_string(matches.at(i).at(1).getRating()) + "),,";


            csvLine += to_string(findRank(people, matches.at(i).at(0))) + ",";
            csvLine += matches.at(i).at(0).getName() +"(" + to_string(matches.at(i).at(0).getRating()) + "),,";
            csvPairings << csvLine << endl;
        }
    }

















    csvPairings.close();



    return matches;


}




#endif // CREATEPAIRINGS_H

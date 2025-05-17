/* 
 * File:   main.cpp
 * Author: Zayd Abu-Ghazaleh
 * Created on September 9th, 10:24 AM
 * Purpose:  Template which is to be copied for all future
 *           Homework, Labs, Projects, Test, etc...
 */

//System Libraries
#include <iostream>  //I/O Library

#include <cmath>

#include <iomanip>

#include <string>

#include <fstream>

#include <list>

using namespace std;

//User Libraries
#include "Property.h"

#include "Piece.h"

#include "Player.h"

class Dice : protected Player{
private:
    int value;
    static int rolls;
public:
    Dice(){
        value=0;
       
    }
    Dice(int input){
        value=input;
    }
    
    Dice(Dice &obj){
        value=obj.value;
    }
    int getValue(){
        return value;
    }

    virtual void setValue(int inputval){
        if(inputval <0){
            throw "ERROR: The value cannot be zero.";
        }
        else{
        value=inputval;
        rolls++;
        }
    }
    
    void operator = (const Dice &val){
        value=val.value;
    }
    
    int operator + (const Dice &val){
        return value+val.value;
    }
    
    int operator - (const Dice &val){
        return value-val.value;
    }
    
    bool operator == (const Dice& val){
        if(value==val.value){
            return true;
        }
        return false;
    }
    
    ~Dice(){
        cout<<"Thanks for playing. This game ended in "<<rolls/2<<" rolls"<<endl;
    }

   void rollCount();
 
};

int Dice::rolls=0;


enum Players {
  PLAYER1,
  PLAYER2
};

//Global Constants Only
//Well known Science, Mathematical and Laboratory Constants
bool roll(Dice[]);
Property** createMap(Property ** & );
void printMap(Property ** & , Player * );
void chanceCoin(Player * , int);
void communityChest(Player * , int);
bool gameOver(Property ** , Player * , int, bool, fstream & );
//Function Prototypes

//Execution of Code Begins Here

int main(int argc, char ** argv) {
  //Set the random number seed here
  string input;
  srand(static_cast < int > (time(NULL)));
  //Declare all variables for this function
  Dice diceroll[2];
  int jailcount[2] = {0,0}; //Counting the amount of turns a player has spent in jail
  int size = 0; //Amount of players
  char response[29] = "This does not fit the range."; //Response to multiple questions
  bool turn = true; //The player's turn
  bool game = true; //Whether the game is going or not
  bool dicedouble = false; //Whether the player rolls a double
  list<int> myList;
  string buyorno; //Whether the player wants to buy or not
  Property ** map; //The map
  createMap(map); //Function used to fill the map structure array
  fstream file; // File used to hold the leaderboard
  cout << "How many players are playing? This game is designed for 2 players." << endl;
  cin >> size; //Amount of players
  if (size != 2) {
    cout << response << endl;
    return 0;
  }
  else{
      myList.push_back(2);
  }
  cin.ignore();
  Player * players = new Player[size]; //Dynamically allocated array of players

  for (int i = 0; i < size; i++) { //For loop that fills the players array with information
    cout << "What piece would player #" << i + 1 << " like to play as?" << endl;
    cout << "Enter any object you would like, (1-10 letters), this will be used as a piece." << endl;
    getline(cin, players[i].token.name);
    if (players[i].token.name.length() < 1 || players[i].token.name.length() > 10) {
      cout << response << endl;
      return 0;
    }

    players[i].token.position[0] = 0; //Starting position of players' tokens
    players[i].token.position[1] = 9; //Starting position of players' tokens
    players[i].money = 1500; //Starting money
  }

  printMap(map, players); //Printing the original map
  while (game) { //While game is continuing

    for (int i = 0; i < size; i++) { //For loop for each player

      players[i].turn = true; //Sets players turn to true
      while (players[i].turn) {
        if (players[i].jail == true) { //Checks for jail
          cout << "Player #" << i + 1 << " is in jail. You need to roll a double to escape or wait " << 3 - jailcount[i] << " more turn(s)." << endl;

        }
        cout << "Is player #" << i + 1 << " ready to roll? Type anything to roll." << endl;
        getline(cin, input);
        if (roll(diceroll) == true) { //If the roll function returns true, player rolled a double
          dicedouble = true;
        } else {
          dicedouble = false;
        }
        if (players[i].jail == true) { //Goes through conditions of escaping jail
          if (dicedouble == true) {
            players[i].jail == false;
          } else if (jailcount[i] >= 2) {
            players[i].jail == false;
          } else {
            jailcount[i]++;
          }
        }

        players[i].turn = dicedouble; //Sets turn to whether player rolled a double or not
        if (players[i].jail == true) {

        } else {
          if (players[i].token.position[0] == 0) { //This entire if, else if, else statement moves the player's piece
            if (players[i].token.position[1] - (diceroll[0] + diceroll[1]) < 0) {
              players[i].token.position[0] = (diceroll[0] + diceroll[1]) - players[i].token.position[1];
              players[i].token.position[1] = 0;
            } else {
              players[i].token.position[1] = players[i].token.position[1] - (diceroll[0] + diceroll[1]);
            }
          } else if (players[i].token.position[0] == 9) {
            if (players[i].token.position[1] + (diceroll[0] + diceroll[1]) > 9) {
              players[i].token.position[0] = 9 - ((diceroll[0] + diceroll[1]) - (9 - players[i].token.position[1]));
              players[i].token.position[1] = 9;
            } else {
              players[i].token.position[1] = players[i].token.position[1] + (diceroll[0] + diceroll[1]);
            }
          } else {
            if (players[i].token.position[1] == 9) {
              if (players[i].token.position[0] - (diceroll[0] + diceroll[1]) < 0) {
                players[i].token.position[1] = 9 - ((diceroll[0] + diceroll[1]) - players[i].token.position[0]);
                players[i].token.position[0] = 0;
              } else {
                players[i].token.position[0] = players[i].token.position[0] - (diceroll[0] + diceroll[1]);
              }
            } else {
              if (players[i].token.position[0] + (diceroll[0] + diceroll[1]) > 9) {
                players[i].token.position[1] = (diceroll[0] + diceroll[1]) - (9 - players[i].token.position[0]);
                players[i].token.position[0] = 9;
              } else {
                players[i].token.position[0] = players[i].token.position[0] + (diceroll[0] + diceroll[1]);
              }
            }

          }
        }
        printMap(map, players); //Prints map again
        game = gameOver(map, players, i, game, file); //Checks whether game is over
        if (game) {
          cout << "You rolled a " << diceroll[0].getValue() << " and a " << diceroll[1].getValue() << endl;
          if (dicedouble == true) cout << "You rolled a double! This means you can roll again." << endl;
          if (map[players[i].token.position[0]][players[i].token.position[1]].owner == 0) {
            if (map[players[i].token.position[0]][players[i].token.position[1]].type != "Government" && map[players[i].token.position[0]][players[i].token.position[1]].type != "Coin" && map[players[i].token.position[0]][players[i].token.position[1]].type != "Chest") {
              cout << "Would you like to buy " << map[players[i].token.position[0]][players[i].token.position[1]].name << " for $" << map[players[i].token.position[0]][players[i].token.position[1]].value << "? Type 'yes' to buy and 'no' to not buy." << endl;
              getline(cin, buyorno);

              while (buyorno != "yes" && buyorno != "no") {
                cout << "Enter 'yes' or 'no'" << endl;
                getline(cin, buyorno);
              }
              if (buyorno == "yes") {
                if (players[i].money < map[players[i].token.position[0]][players[i].token.position[1]].value) {
                  cout << "You do not have enough money!" << endl;
                } else {
                  map[players[i].token.position[0]][players[i].token.position[1]].owner = i + 1;
                  players[i].money = players[i].money - map[players[i].token.position[0]][players[i].token.position[1]].value;
                  cout << "Player #" << i + 1 << " now has $" << players[i].money << endl;
                }
              }
            } else {
              if (map[players[i].token.position[0]][players[i].token.position[1]].type == "Coin") {
                chanceCoin(players, i);
              } else if (map[players[i].token.position[0]][players[i].token.position[1]].type == "Chest") {
                communityChest(players, i);
              }
            }
          } else {
            if (map[players[i].token.position[0]][players[i].token.position[1]].owner == i + 1) {
              cout << "You own the property " << map[players[i].token.position[0]][players[i].token.position[1]].name << endl;
            } else {
              cout << "The property " << map[players[i].token.position[0]][players[i].token.position[1]].name << " is owned by player " << map[players[i].token.position[0]][players[i].token.position[1]].owner << "." << endl;
              cout << "You owe $" << map[players[i].token.position[0]][players[i].token.position[1]].value << endl;

            }
          }
        } else {
          return 0;
        }
      }
    }

  }

  
  for (int i = 0; i < 10; i++) {
    delete map[i];
  }
  delete map;
  return 0;
}

//Function Implementations

bool roll(Dice diceroll[]) {
  bool turn;
  diceroll[0].setValue(rand() % 4 + 1);
  diceroll[1].setValue(rand() % 4 + 1);
  
  if (diceroll[0]==diceroll[1]) turn = true;
  else turn = false;

  return turn;
}
Property** createMap(Property ** & map) { //This function creates the entire map in a structure array
  map = new Property * [10];
  for (int i = 0; i < 10; i++) {
    map[i] = new Property[10];
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (i == 0) {
        if (j == 0) {
          map[i][j].name = "JAIL";
          map[i][j].type = "Government";
          map[i][j].value = 0;
          map[i][j].owner = 0;

        } else if (j == 1) {
          map[i][j].name = "CONNECT. AVE";
          map[i][j].type = "Blue";
          map[i][j].value = 120;
          map[i][j].owner = 0;

        } else if (j == 2) {
          map[i][j].name = "VERMONT AVE";
          map[i][j].type = "Blue";
          map[i][j].value = 100;
          map[i][j].owner = 0;

        } else if (j == 3) {
          map[i][j].name = "CHANCE";
          map[i][j].type = "Coin";
          map[i][j].value = 0;
          map[i][j].owner = 0;

        } else if (j == 4) {
          map[i][j].name = "ORIENTAL AVE";
          map[i][j].type = "Blue";
          map[i][j].value = 100;
          map[i][j].owner = 0;

        } else if (j == 5) {
          map[i][j].name = "VAN RAILROAD";
          map[i][j].type = "Railroad";
          map[i][j].value = 200;
          map[i][j].owner = 0;

        } else if (j == 6) {
          map[i][j].name = "BALTIC AVE";
          map[i][j].type = "Brown";
          map[i][j].value = 60;
          map[i][j].owner = 0;

        } else if (j == 7) {
          map[i][j].name = "COMM. CHEST";
          map[i][j].type = "Chest";
          map[i][j].value = 0;
          map[i][j].owner = 0;

        } else if (j == 8) {
          map[i][j].name = "MED. AVE";
          map[i][j].type = "Brown";
          map[i][j].value = 60;
          map[i][j].owner = 0;

        } else if (j == 9) {
          map[i][j].name = "GO";
          map[i][j].type = "Government";
          map[i][j].value = 0;
          map[i][j].owner = 0;
        }
      } else if (i == 9) {
        if (j == 0) {
          map[i][j].name = "FREE PARKING";
          map[i][j].type = "Government";
          map[i][j].value = 0;
          map[i][j].owner = 0;
        } else if (j == 1) {
          map[i][j].name = "KENTUCKY AVE";
          map[i][j].type = "Red";
          map[i][j].value = 220;
          map[i][j].owner = 0;
        } else if (j == 2) {
          map[i][j].name = "CHANCE";
          map[i][j].type = "Coin";
          map[i][j].value = 0;
          map[i][j].owner = 0;

        } else if (j == 3) {
          map[i][j].name = "INDIANA AVE";
          map[i][j].type = "Red";
          map[i][j].value = 220;
          map[i][j].owner = 0;

        } else if (j == 4) {
          map[i][j].name = "ILLINOIS AVE";
          map[i][j].type = "Red";
          map[i][j].value = 240;
          map[i][j].owner = 0;

        } else if (j == 5) {
          map[i][j].name = "B&O RAILROAD";
          map[i][j].type = "Railroad";
          map[i][j].value = 200;
          map[i][j].owner = 0;

        } else if (j == 6) {
          map[i][j].name = "ATLANTIC AVE";
          map[i][j].type = "Yellow";
          map[i][j].value = 260;
          map[i][j].owner = 0;

        } else if (j == 7) {
          map[i][j].name = "VENTNOR AVE";
          map[i][j].type = "Yellow";
          map[i][j].value = 260;
          map[i][j].owner = 0;

        } else if (j == 8) {
          map[i][j].name = "MARV GARDENS";
          map[i][j].type = "Yellow";
          map[i][j].value = 280;
          map[i][j].owner = 0;

        } else if (j == 9) {
          map[i][j].name = "GO TO JAIL";
          map[i][j].type = "Government";
          map[i][j].value = 0;
          map[i][j].owner = 0;

        }
      } else {
        if (i == 1) {
          if (j == 0) {
            map[i][j].name = "CHARLES PLACE";
            map[i][j].type = "Purple";
            map[i][j].value = 140;
            map[i][j].owner = 0;
          } else if (j == 9) {
            map[i][j].name = "BOARDWALK";
            map[i][j].type = "Blue";
            map[i][j].value = 400;
            map[i][j].owner = 0;
          } else {
            map[i][j].name = "";
            map[i][j].type = "";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          }
        } else if (i == 2) {
          if (j == 0) {
            map[i][j].name = "STATES AVE";
            map[i][j].type = "Purple";
            map[i][j].value = 140;
            map[i][j].owner = 0;
          } else if (j == 9) {
            map[i][j].name = "PARK PLACE";
            map[i][j].type = "Blue";
            map[i][j].value = 350;
            map[i][j].owner = 0;
          } else {
            map[i][j].name = "";
            map[i][j].type = "";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          }
        } else if (i == 3) {
          if (j == 0) {
            map[i][j].name = "VIRGINIA AVE";
            map[i][j].type = "Purple";
            map[i][j].value = 160;
            map[i][j].owner = 0;
          } else if (j == 9) {
            map[i][j].name = "CHANCE";
            map[i][j].type = "Coin";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          } else {
            map[i][j].name = "";
            map[i][j].type = "";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          }
        } else if (i == 4) {
          if (j == 0) {
            map[i][j].name = "PA RAILROAD";
            map[i][j].type = "Railroad";
            map[i][j].value = 200;
            map[i][j].owner = 0;
          } else if (j == 9) {
            map[i][j].name = "LINE RAILROAD";
            map[i][j].type = "Railroad";
            map[i][j].value = 200;
            map[i][j].owner = 0;
          } else {
            map[i][j].name = "";
            map[i][j].type = "";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          }
        } else if (i == 5) {
          if (j == 0) {
            map[i][j].name = "JAMES PLACE";
            map[i][j].type = "Orange";
            map[i][j].value = 180;
            map[i][j].owner = 0;
          } else if (j == 9) {
            map[i][j].name = "PA AVE";
            map[i][j].type = "Green";
            map[i][j].value = 320;
            map[i][j].owner = 0;
          } else {
            map[i][j].name = "";
            map[i][j].type = "";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          }
        } else if (i == 6) {
          if (j == 0) {
            map[i][j].name = "COMM. CHEST";
            map[i][j].type = "Chest";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          } else if (j == 9) {
            map[i][j].name = "COMM. CHEST";
            map[i][j].type = "Chest";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          } else {
            map[i][j].name = "";
            map[i][j].type = "";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          }
        } else if (i == 7) {
          if (j == 0) {
            map[i][j].name = "TN AVE";
            map[i][j].type = "Orange";
            map[i][j].value = 180;
            map[i][j].owner = 0;
          } else if (j == 9) {
            map[i][j].name = "NC AVE";
            map[i][j].type = "Green";
            map[i][j].value = 300;
            map[i][j].owner = 0;
          } else {
            map[i][j].name = "";
            map[i][j].type = "";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          }
        } else {
          if (j == 0) {
            map[i][j].name = "NY AVE";
            map[i][j].type = "Orange";
            map[i][j].value = 200;
            map[i][j].owner = 0;
          } else if (j == 9) {
            map[i][j].name = "PACIFIC AVE";
            map[i][j].type = "Green";
            map[i][j].value = 300;
            map[i][j].owner = 0;
          } else {
            map[i][j].name = "";
            map[i][j].type = "";
            map[i][j].value = 0;
            map[i][j].owner = 0;
          }
        }
      }
    }
    
  }

  return map;
}

void printMap(Property ** & map, Player * players) { //This function prints the entire structure array (map) after changes
  int len;
  bool pos;
  string leng;
  for (int i = 0; i < 10; i++) { //Printing the top of the cubes at the top of the map
    cout << "|--------------|";
  }
  cout << endl;
  for (int i = 0; i < 10; i++) { 
    len = map[9][i].name.length(); //Getting the length of the map name for formatting
    if (len % 2 == 0) {

      cout << fixed << "|" << setw((14 - len) / 2 + len) << map[9][i].name << setw((14 - len) / 2 + 1) << "|"; //Printing the map name in the cube
    } else {
      cout << fixed << "|" << setw((14 - len) / 2 + len) << map[9][i].name << setw((14 - len) / 2 + 2) << "|";//Printing the map name in the cube
    }
  }
  cout << endl;

  for (int i = 0; i < 10; i++) {
    leng = "$" + to_string(map[9][i].value); //Creating a string of $ + the value of the map
    len = leng.length(); //Getting the length of that string
    if (map[9][i].type == "Government" || map[9][i].type == "Chest" || map[9][i].type == "Coin") { //Checking if the map does not have a value
      cout << "|              |";
    } else {
      if (len % 2 == 0) {

        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|"; //Printing the value of the map in the cube
      } else {
        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|"; //Printing the value of the map in the cube
      }
    }
  }
  cout << endl;
  pos = false; //Used to check if the player is on this cube of the map
  if (players[0].token.position[0] == players[1].token.position[0] && players[0].token.position[1] == players[1].token.position[1]) { //Checking whether both players are on the same part of the map
    for (int i = 0; i < 10; i++) {
      leng = players[0].token.name + ("(P1)"); //Creating a string of the token name + P1
      len = leng.length(); //length of that string
      if (map[players[0].token.position[0]][players[0].token.position[1]].name == map[9][i].name) { //Checking whether the player is on this cube

        pos = true; //Positive
        if (len % 2 == 0) {

          cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|"; //Printing the player's name
        } else {
          cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";//Printing the player's name
        }
      }

      if (pos == false) {
        cout << "|              |"; //Printing emptiness since player is not on this cube
      }
      pos = false;
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
      leng = players[1].token.name + ("(P2)"); //P2's turn
      len = leng.length();

      if (map[players[1].token.position[0]][players[1].token.position[1]].name == map[9][i].name) {

        pos = true;
        if (len % 2 == 0) {

          cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
        } else {
          cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
        }
      }

      if (pos == false) {
        cout << "|              |";
      }
      pos = false;
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
      cout << "|              |";
    }

  } else {
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 2; j++) {
        if (j == 0) {
          leng = players[0].token.name + ("(P1)");
          len = leng.length();
        } else {
          leng = players[1].token.name + ("(P2)");
          len = leng.length();
        }

        if (map[players[j].token.position[0]][players[j].token.position[1]].name == map[9][i].name) {

          pos = true;
          if (len % 2 == 0) {

            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
          } else {
            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
          }
        }

      }
      if (pos == false) {
        cout << "|              |";
      }
      pos = false;
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
      cout << "|              |";
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
      cout << "|              |";
    }
  }
  cout << endl;
  for (int i = 0; i < 10; i++) {
    leng = "Set:" + map[9][i].type; //Printing the set
    len = leng.length();
    if (map[9][i].type == "Government" || map[9][i].type == "Chest" || map[9][i].type == "Coin") {
      cout << "|              |";
    } else {
      if (len % 2 == 0) {

        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
      } else {
        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
      }
    }
  }
  cout << endl;
  for (int i = 0; i < 10; i++) {
    cout << "|--------------|";
  }

  cout << endl;
  for (int i = 0; i < 10; i++) {
    if (map[9][i].owner == 1 || map[9][i].owner == 2) leng = "Owner: P" + to_string(map[9][i].owner); //Printing the owner
    else leng = "Owner: None";
    len = leng.length();
    if (map[9][i].type == "Government" || map[9][i].type == "Chest" || map[9][i].type == "Coin") {
      cout << "|              |";
    } else {
      if (len % 2 == 0) {

        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
      } else {
        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
      }
    }
  }
  cout << endl;
  for (int i = 0; i < 10; i++) {
    cout << "|--------------|";
  }
  cout << endl;

  for (int i = 8; i > 0; i--) {
    for (int j = 0; j < 10; j++) {
      len = map[i][j].name.length();
      if (j == 0) {
        cout << fixed << "|--------------|" << setw(16 * 9) << "|--------------|" << endl;
        if (len % 2 == 0) {

          cout << fixed << "|" << setw((14 - len) / 2 + len) << map[i][j].name << setw((14 - len) / 2 + 1) << "|";
        } else {
          cout << fixed << "|" << setw((14 - len) / 2 + len) << map[i][j].name << setw((14 - len) / 2 + 2) << "|";
        }

      } else if (j == 9) {
        if (map[i][j].name.length() % 2 == 0) {

          cout << fixed << "|" << setw((14 - len) / 2 + map[i][j].name.length()) << map[i][j].name << setw((14 - len) / 2 + 1) << "|";
        } else {
          cout << fixed << "|" << setw((14 - len) / 2 + map[i][j].name.length()) << map[i][j].name << setw((14 - len) / 2 + 2) << "|";
        }
        cout << endl;

        leng = "$" + to_string(map[i][0].value);
        len = leng.length();
        if (map[i][0].type == "Government" || map[i][0].type == "Chest" || map[i][0].type == "Coin") {
          cout << "|              |";
        } else {
          if (len % 2 == 0) {

            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
          } else {
            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
          }
        }
        leng = "$" + to_string(map[i][j].value);
        len = leng.length();

        if (map[i][j].type == "Government" || map[i][j].type == "Chest" || map[i][j].type == "Coin") {
          cout << setw(16 * 9) << "|              |";
        } else {
          if (len % 2 == 0) {

            cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
          } else {
            cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
          }
        }
        cout << endl;

        if (players[0].token.position[0] == players[1].token.position[0] && players[0].token.position[1] == players[1].token.position[1]) {

          leng = players[0].token.name + ("(P1)");
          len = leng.length();
          if (map[players[0].token.position[0]][players[0].token.position[1]].name == map[i][0].name) {

            if (len % 2 == 0) {

              cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|" << setw(16 * 9) << "|              |" << endl;
              leng = players[1].token.name + ("(P2)");
              len = leng.length();
              cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|" << setw(16 * 9) << "|              |" << endl;
            } else {
              cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|" << setw(16 * 9) << "|              |" << endl;
              leng = players[1].token.name + ("(P2)");
              len = leng.length();
              cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|" << setw(16 * 9) << "|              |" << endl;
            }
          } else if (map[players[0].token.position[0]][players[0].token.position[1]].name == map[i][9].name) {
            if (len % 2 == 0) {

              cout << fixed << "|              |" << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|" << endl;
              leng = players[1].token.name + ("(P2)");
              len = leng.length();
              cout << fixed << "|              |" << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|" << endl;
            } else {
              cout << fixed << "|              |" << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|" << endl;
              leng = players[1].token.name + ("(P2)");
              len = leng.length();
              cout << fixed << "|              |" << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|" << endl;
            }
          } else {
            cout << "|              |" << setw(16 * 9) << "|              |" << endl;
            cout << "|              |" << setw(16 * 9) << "|              |" << endl;
          }

        } else {

          if (map[players[0].token.position[0]][players[0].token.position[1]].name == map[i][0].name) {
            leng = players[0].token.name + ("(P1)");
            len = leng.length();
            if (len % 2 == 0) {

              cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
            } else {
              cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
            }
          } else if (map[players[1].token.position[0]][players[1].token.position[1]].name == map[i][0].name) {
            leng = players[1].token.name + ("(P2)");
            len = leng.length();

            if (len % 2 == 0) {

              cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
            } else {
              cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
            }
          } else {
            cout << "|              |";
          }
          if (map[players[0].token.position[0]][players[0].token.position[1]].name == map[i][j].name) {
            leng = players[0].token.name + ("(P1)");
            len = leng.length();

            if (len % 2 == 0) {

              cout << fixed << setw(16 * 8) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|" << endl;
              cout << fixed << "|              |" << setw(16 * 9) << "|              |" << endl;
            } else {
              cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|" << endl;
              cout << fixed << "|              |" << setw(16 * 9) << "|              |" << endl;
            }
          } else if (map[players[1].token.position[0]][players[1].token.position[1]].name == map[i][j].name) {
            leng = players[1].token.name + ("(P2)");
            len = leng.length();
            if (len % 2 == 0) {

              cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|" << endl;
              cout << fixed << "|              |" << setw(16 * 9) << "|              |" << endl;
            } else {
              cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|" << endl;
              cout << fixed << "|              |" << setw(16 * 9) << "|              |" << endl;
            }
          } else {
            cout << fixed << setw(16 * 9) << "|              |" << endl;
            cout << fixed << "|              |" << setw(16 * 9) << "|              |" << endl;
          }

        }
        cout << fixed << "|              |" << setw(16 * 9) << "|              |" << endl;

        leng = "Set:" + map[i][0].type;
        len = leng.length();
        if (map[i][0].type == "Government" || map[i][0].type == "Chest" || map[i][0].type == "Coin") {
          cout << "|              |";
        } else {
          if (len % 2 == 0) {

            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
          } else {
            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
          }
        }
        leng = "Set:" + map[i][j].type;
        len = leng.length();
        if (map[i][j].type == "Government" || map[i][j].type == "Chest" || map[i][j].type == "Coin") {
          cout << setw(16 * 9) << "|              |";
        } else {
          if (len % 2 == 0) {

            cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
          } else {
            cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
          }
        }
        cout << endl;

        cout << fixed << "|--------------|" << setw(16 * 9) << "|--------------|" << endl;
        //Owner
        if (map[i][0].owner == 0) leng = "Owner: None";
        else leng = "Owner: P" + to_string(map[i][0].owner);

        len = leng.length();
        if (map[i][0].type == "Government" || map[i][0].type == "Chest" || map[i][0].type == "Coin") {
          cout << "|              |";
        } else {
          if (len % 2 == 0) {

            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
          } else {
            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
          }
        }
        if (map[i][j].owner == 0) leng = "Owner: None";
        else leng = "Owner: P" + to_string(map[i][j].owner);

        len = leng.length();
        if (map[i][j].type == "Government" || map[i][j].type == "Chest" || map[i][j].type == "Coin") {
          cout << setw(16 * 9) << "|              |";
        } else {
          if (len % 2 == 0) {

            cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
          } else {
            cout << fixed << setw(16 * 8 + 1) << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
          }
        }
        cout << endl;
        cout << fixed << "|--------------|" << setw(16 * 9) << "|--------------|" << endl;

      } else {
        cout << fixed << setw(16) << "";
      }
    }
  }

  for (int i = 0; i < 10; i++) {
    cout << "|--------------|";
  }
  cout << endl;
  for (int i = 0; i < 10; i++) {
    len = map[0][i].name.length();
    if (len % 2 == 0) {

      cout << fixed << "|" << setw((14 - len) / 2 + len) << map[0][i].name << setw((14 - len) / 2 + 1) << "|";
    } else {
      cout << fixed << "|" << setw((14 - len) / 2 + len) << map[0][i].name << setw((14 - len) / 2 + 2) << "|";
    }
  }
  cout << endl;
  for (int i = 0; i < 10; i++) {
    leng = "$" + to_string(map[0][i].value);
    len = leng.length();
    if (map[0][i].type == "Government" || map[0][i].type == "Chest" || map[0][i].type == "Coin") {
      cout << "|              |";
    } else {
      if (len % 2 == 0) {

        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
      } else {
        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
      }
    }
  }
  cout << endl;
  pos = false;
  if (players[0].token.position[0] == players[1].token.position[0] && players[0].token.position[1] == players[1].token.position[1]) {
    for (int i = 0; i < 10; i++) {
      leng = players[0].token.name + ("(P1)");
      len = leng.length();
      if (map[players[0].token.position[0]][players[0].token.position[1]].name == map[0][i].name) {

        pos = true;
        if (len % 2 == 0) {

          cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
        } else {
          cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
        }
      }

      if (pos == false) {
        cout << "|              |";
      }
      pos = false;
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
      leng = players[1].token.name + ("(P2)");
      len = leng.length();

      if (map[players[1].token.position[0]][players[1].token.position[1]].name == map[0][i].name) {

        pos = true;
        if (len % 2 == 0) {

          cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
        } else {
          cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
        }
      }

      if (pos == false) {
        cout << "|              |";
      }
      pos = false;
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
      cout << "|              |";
    }
  } else {
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 2; j++) {
        if (j == 0) {
          leng = players[0].token.name + ("(P1)");
          len = leng.length();
        } else {
          leng = players[1].token.name + ("(P2)");
          len = leng.length();
        }

        if (map[players[j].token.position[0]][players[j].token.position[1]].name == map[0][i].name) {

          pos = true;
          if (len % 2 == 0) {

            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
          } else {
            cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
          }
        }

      }
      if (pos == false) {
        cout << "|              |";
      }
      pos = false;
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
      cout << "|              |";
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
      cout << "|              |";
    }
  }
  cout << endl;
  for (int i = 0; i < 10; i++) { //Bot
    leng = "Set:" + map[0][i].type;
    len = leng.length();
    if (map[0][i].type == "Government" || map[0][i].type == "Chest" || map[0][i].type == "Coin") {
      cout << "|              |";
    } else {
      if (len % 2 == 0) {

        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
      } else {
        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
      }
    }
  }

  cout << endl;
  for (int i = 0; i < 10; i++) {
    cout << "|--------------|";
  }
  cout << endl;
  for (int i = 0; i < 10; i++) {
    if (map[0][i].owner == 1 || map[0][i].owner == 2) leng = "Owner: P" + to_string(map[0][i].owner);
    else leng = "Owner: None";
    len = leng.length();
    if (map[0][i].type == "Government" || map[0][i].type == "Chest" || map[0][i].type == "Coin") {
      cout << "|              |";
    } else {
      if (len % 2 == 0) {

        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 1) << "|";
      } else {
        cout << fixed << "|" << setw((14 - len) / 2 + len) << leng << setw((14 - len) / 2 + 2) << "|";
      }
    }
  }
  cout << endl;
  for (int i = 0; i < 10; i++) {
    cout << "|--------------|";
  }
  cout << endl;
}

void chanceCoin(Player * players, int i) {
  int chance = 0;
  string answer;
  cout << "You landed on a chance coin. Are you ready to pull your card? Enter anything to continue." << endl;
  getline(cin, answer);
  chance = rand() % 3;

  switch (chance) {
  case 0:
    cout << "Advance to GO. You gain $200." << endl;
    players[i].token.position[0] = 0;
    players[i].token.position[1] = 9;
    players[i].money = players[i].money + 200;
    cout << "Your new balance is $" << players[i].money << endl;
    break;

  case 1:
    cout << "Go directly to jail. You will not gain the $200 from GO." << endl;
    players[i].token.position[0] = 9;
    players[i].token.position[1] = 0;
    players[i].jail = true;
    break;

  case 2:
    cout << "You have been elected mayor. Give $50 to the other player." << endl;
    if (i == 0) {
      players[i].money = players[i].money - 50;
      players[i + 1].money = players[i + 1].money + 50;
    } else {
      players[i].money = players[i].money - 50;
      players[i - 1].money = players[i - 1].money + 50;
    }
    cout << "Your new balance is $" << players[i].money << endl;
    break;

  }
}

void communityChest(Player * players, int i) {
  int chest = 0;
  string answer;
  cout << "You landed on a community chest. Are you ready to pull your card? Enter anything to continue." << endl;
  getline(cin, answer);
  chest = rand() % 3;

  switch (chest) {
  case 0:
    cout << "Pay a hospital bill of $100." << endl;
    players[i].money = players[i].money - 100;
    cout << "Your new balance is $" << players[i].money << endl;
    break;

  case 1:
    cout << "You inherit $200 from a lost grandparent!" << endl;
    players[i].money += 200;
    cout << "Your new balance is $" << players[i].money << endl;
    break;

  case 2:
    cout << "You hacked into the other player's bank. You stole $50." << endl;
    if (i == 0) {
      players[i].money = players[i].money + 50;
      players[i + 1].money = players[i + 1].money - 50;
    } else {
      players[i].money = players[i].money + 50;
      players[i - 1].money = players[i - 1].money + 50;
    }
    cout << "Your new balance is $" << players[i].money << endl;
    break;

  }
}

bool gameOver(Property ** map, Player * players, int i, bool game, fstream & file) { //Checks whether game is over
  int ownercount = 0; //Used to count whether an entire row is owned by a player (win)
  int ownercount2 = 0; //Used to count whether an entire row is owned by a player (win)
  int railroadcount = 0; //Used to count whether all railroads are owned by a player (win)

  string winner;
  winner = players[i].token.name;
  file.open("leaderboard.txt", ios::out | ios::in | ios::binary);
  players[i].wins.open("wins.txt", ios::out);

  for (int g = 0; g < 9; g++) {
    if (g == 0) {
      ownercount = 0;
      railroadcount = 0;
      for (int l = 0; l < 10; l++) {
        if (map[g][l].type == "Government" || map[g][l].type == "Chest" || map[g][l].type == "Coin") {

        } else if (map[g][l].type == "Railroad") {
          if (map[g][l].owner == i + 1) {
            railroadcount++;
          }
        } else {
          if (map[g][l].owner == i + 1) {
            ownercount++;
          }
        }
      }
      if (ownercount == 4) {
        winner = players[i].token.name;
        players[i].wins << "+1";
        file.seekg(0L, ios::beg);
        file.write(reinterpret_cast < char * > ( & winner), sizeof( & winner));
        file.read(reinterpret_cast < char * > ( & winner), sizeof( & winner));

        cout << winner << " has won!" << endl;

        game = false;
      }

    } else if (g == 9) {
      ownercount = 0;

      for (int l = 0; l < 10; l++) {
        if (map[g][l].type == "Government" || map[g][l].type == "Chest" || map[g][l].type == "Coin") {

        } else if (map[g][l].type == "Railroad") {
          if (map[g][l].owner == i + 1) {
            railroadcount++;
          }
        } else {
          if (map[g][l].owner == i + 1) {
            ownercount++;
          }
        }
      }
      if (ownercount == 5) {
        winner = players[i].token.name;
        players[i].wins << "+1";
        file.seekg(0L, ios::beg);
        file.write(reinterpret_cast < char * > ( & winner), sizeof( & winner));
        file.read(reinterpret_cast < char * > ( & winner), sizeof( & winner));

        cout << winner << " has won!" << endl;
        game = false;
      }

    } else {
      ownercount = 0;
      ownercount2 = 0;
      for (int l = 0; l < 10; l = l + 9) {
        if (l == 0) {
          if (map[g][l].type == "Government" || map[g][l].type == "Chest" || map[g][l].type == "Coin") {

          } else if (map[g][l].type == "Railroad") {
            if (map[g][l].owner == i + 1) {
              railroadcount++;
            }
          } else {
            if (map[g][l].owner == i + 1) {
              ownercount++;
            }
          }
        }
        if (l == 9) {
          if (map[g][l].type == "Government" || map[g][l].type == "Chest" || map[g][l].type == "Coin") {

          } else if (map[g][l].type == "Railroad") {
            if (map[g][l].owner == i + 1) {
              railroadcount++;
            }
          } else {
            if (map[g][l].owner == i + 1) {
              ownercount2++;
            }
          }
        }
      }
      if (ownercount == 5 || ownercount2 == 4 || railroadcount == 3) {
        winner = players[i].token.name;
        players[i].wins << "+1";
        file.seekg(0L, ios::beg);
        file.write(reinterpret_cast < char * > ( & winner), sizeof( & winner));
        file.read(reinterpret_cast < char * > ( & winner), sizeof( & winner));

        cout << winner << " has won!" << endl;

        game = false;

      }
    }
  }
  if (players[i].money < 0) {
    game = false;
  }
  file.close();
  players[i].wins.close();
  return game;
}
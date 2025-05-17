#ifndef PLAYER_H
#define PLAYER_H

struct Player{
   Piece token; //The piece the player is moving
    int money; //The amount of money the player has
    bool turn; //Whether it is this player's turn
    bool jail; //Whether this player is in jail
    fstream wins; //A file holding the wins
};

#endif /* PLAYER_H */
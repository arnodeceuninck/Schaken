//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include <vector>
#include "SchaakStuk.h" // Moet geincluded zijn wegens gebruik enum

using namespace std;

class Positie;

class GeplaatstSchaakStuk{
public:
    Positie positie;

    GeplaatstSchaakStuk(const Positie &positie, SchaakStuk *schaakStuk);

    SchaakStuk* schaakStuk;
};

class Game {
// variabelen om de status van het spel/bord te bewaren
private:
    zw turn;
//    vector<GeplaatstSchaakStuk*> extra_undo;
//    bool previousMoveSpecialMove;
    vector<vector<GeplaatstSchaakStuk*>> undo_stack; // (oude_positie, nieuwe_positie, gedode_positie)
    vector<GeplaatstSchaakStuk*> redo_stack;
    vector<bool> firstTurn_stack;
    zw AIKleur;
public:
    zw getAIKleur() const;

public:
    bool makeMovement(const SchaakStuk*,Positie);
    void setTurn(zw turn);
    void undo();
    void redo();
    void switchTurn();
    void moveAI();
//    int rokadePossible(zw kleur) const;
//    void rokadeMove(zw kleur, int y);

private:
    bool game_over;
public:
    void setGame_over(bool game_over);

public:
    bool isGame_over() const;

public:
    zw getTurn() const;

private:
    Positie pieceToMove;
    bool movePieceBool;
    void destroyPiece(const Positie positie);
public:
    bool movePiece();
    const Positie &getPieceToMove() const;

    void setPieceToMove(const Positie &pieceToMove);

    void setSchaakstukken(const vector<GeplaatstSchaakStuk*> &schaakstukken);

private:
    vector<GeplaatstSchaakStuk*> schaakstukken;
    bool promotie;
public:
    void setPromotie(bool promotie);

private:
    GeplaatstSchaakStuk* promotiestuk;
public:
    void setPromotiestuk(SchaakStuk *promotiestuk);

public:
    GeplaatstSchaakStuk *getPromotiestuk() const;

public:
    bool isPromotie() const;

public:
    const vector<GeplaatstSchaakStuk*> &getSchaakstukken() const;

public:
    Game();
    ~Game();

    bool move(SchaakStuk*,Positie);

    bool schaak(zw kleur) const;
    bool schaakmat(zw kleur) const;
    bool pat(zw kleur) const;

    // Opdracht 1.2.3
    void setStartBord();

    // Opdracht 1.2.2
    bool plaatsSchaakStuk(SchaakStuk *stuk, Positie pos);
    SchaakStuk * getSchaakStuk(Positie positie) const;

    Positie getPositie(const SchaakStuk *schaakStuk) const;

    GeplaatstSchaakStuk* getGeplaatstSchaakStuk(SchaakStuk* stuk);
};


#endif //SCHAKEN_GAME_H

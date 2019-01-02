//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <chessboard.h>
#include "Positie.h"
//#include "game.h" // er worden functies hiervan gebruikt -> forward declarations

class Game;

enum zw{zwart,wit};

class SchaakStuk {
public:
    SchaakStuk(zw kleur): kleur(kleur), firstTime(true) {}

    virtual Piece piece() const=0;      // Deze functie krijg je cadeau
                                        // Je hebt die nodig om het stuk in de
                                        // GUI te tekenen
                                        // Meer info: kijk naar mainwindow.cpp, lijn 27
    zw getKleur() const { return kleur; }
    virtual vector<Positie> geldige_zetten(const Game &game, bool controleerVoorSchaak) const=0;
    vector<Positie> filterGeldigeZettenWaarbijKoningSchaakStaat(const Game &game, vector<Positie> zetten, const SchaakStuk *schaakStuk) const;
    bool isFirstTime() const {
        return firstTime;
    }

    void setFirstTime(bool firstTime) {
        SchaakStuk::firstTime = firstTime;
    }

    vector<Positie> rechteBewegingen(const Game &game) const;
    vector<Positie> schuineBewegingen(const Game &game) const;
private:
    zw kleur;
    bool firstTime;
    bool vorigeZetVertrokken;
};


class Pion:public SchaakStuk {
public:
    Pion(zw kleur):SchaakStuk(kleur) {}

    virtual Piece piece() const override {
        return Piece(Piece::Pawn,getKleur()==wit?Piece::White:Piece::Black);
    }

    vector<Positie> geldige_zetten(const Game &game, bool controleerVoorSchaak) const override;
};

class Toren:public SchaakStuk {
public:
    Toren(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Rook,getKleur()==wit?Piece::White:Piece::Black);
    }

    vector<Positie> geldige_zetten(const Game &game, bool controleerVoorSchaak) const override;
};

class Paard:public SchaakStuk {
public:
    Paard(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Knight,getKleur()==wit?Piece::White:Piece::Black);
    }

    vector<Positie> geldige_zetten(const Game &game, bool controleerVoorSchaak) const override;
};

class Loper:public SchaakStuk {
public:
    Loper(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black);
    }

    vector<Positie> geldige_zetten(const Game &game, bool controleerVoorSchaak) const override;
};

class Koning:public SchaakStuk {
public:
    Koning(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::King,getKleur()==wit?Piece::White:Piece::Black);
    }

    vector<Positie> geldige_zetten(const Game &game, bool controleerVoorSchaak) const override;
};

class Koningin:public SchaakStuk {
public:
    Koningin(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Queen,getKleur()==wit?Piece::White:Piece::Black);
    }

    vector<Positie> geldige_zetten(const Game &game, bool controleerVoorSchaak) const override;
};

#endif //SCHAKEN_SCHAAKSTUK_H

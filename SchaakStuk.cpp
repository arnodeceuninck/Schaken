//  Student: Arno Deceuninck
//  Rolnummer: 20181217
//  Opmerkingen: (bvb aanpassingen van de opgave) (en-passent en rokade niet gemaakt, de rest wel)
//

#include <iostream>
#include "SchaakStuk.h"
#include "game.h"
Positie getPositie(SchaakStuk *schaakStuk);

vector<Positie> Pion::geldige_zetten(const Game &game, bool controleerVoorSchaak) const{
    vector<Positie> geldige_zetten;
    vector<Positie> mogelijke_zetten;
    Positie currenPosition = game.getPositie(this);
    // Stijgen bij zwart, dalen bij wit
    int multiplyFactor = 1;
    if(getKleur() == zwart){
        multiplyFactor = -1;
    }

    try {
        mogelijke_zetten.emplace_back(Positie(currenPosition.get_x(), currenPosition.get_y()+1*multiplyFactor));
    } catch (exception e){} // positie buiten het veld kan een error geven

    if (isFirstTime() and
    game.getSchaakStuk(Positie(currenPosition.get_x(), currenPosition.get_y()+1*multiplyFactor)) == nullptr){
        try{
            mogelijke_zetten.emplace_back(Positie(currenPosition.get_x(), currenPosition.get_y()+2*multiplyFactor));
        } catch (exception e){}
    }

    for(Positie pos: mogelijke_zetten){
        if(game.getSchaakStuk(pos) == nullptr){
            geldige_zetten.emplace_back(pos);
        }
    }

    mogelijke_zetten = {};
    try {
        mogelijke_zetten.emplace_back(Positie(currenPosition.get_x()+1, currenPosition.get_y()+1*multiplyFactor));
    } catch (exception e){}

    try {
        mogelijke_zetten.emplace_back(Positie(currenPosition.get_x()-1, currenPosition.get_y()+1*multiplyFactor));
    } catch (exception e){}

    for(Positie pos: mogelijke_zetten){
        if(game.getSchaakStuk(pos) != nullptr and game.getSchaakStuk(pos)->getKleur() != getKleur()){
            geldige_zetten.emplace_back(pos);
        }
    }

    if(controleerVoorSchaak) {
        geldige_zetten = filterGeldigeZettenWaarbijKoningSchaakStaat(game, geldige_zetten, this);
    }
    return geldige_zetten;
}

vector<Positie> Toren::geldige_zetten(const Game &game, bool controleerVoorSchaak) const {
    vector<Positie> geldige_zetten = rechteBewegingen(game);


    if(controleerVoorSchaak) {
//        Positie currenPosition = game.getPositie(this);
//
//        int x = currenPosition.get_x();
//        int y = currenPosition.get_y();
//
//        int rokade = game.rokadePossible(getKleur());
//        switch (rokade){
//            case 0: break;
//            case 1: geldige_zetten.emplace_back(Positie(2, y)); break;
//            case 2: geldige_zetten.emplace_back(Positie(4, y)); break;
//            case 3:
//                geldige_zetten.emplace_back(Positie(2, y));
//                geldige_zetten.emplace_back(Positie(4, y));
//                break;
//        }
        geldige_zetten = filterGeldigeZettenWaarbijKoningSchaakStaat(game, geldige_zetten, this);
    }
    return geldige_zetten;
}

vector<Positie> Paard::geldige_zetten(const Game &game, bool controleerVoorSchaak) const {
    vector<Positie> geldige_zetten;
    vector<Positie> mogelijke_zetten;
    Positie currenPosition = game.getPositie(this); // TODO: this, *this or &this

    int x = currenPosition.get_x();
    int y = currenPosition.get_y();
    
    try {
        mogelijke_zetten.emplace_back(Positie(x + 2, y + 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x - 2, y + 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x + 2, y - 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x - 2, y - 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x + 1, y + 2));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x - 1, y + 2));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x + 1, y - 2));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x - 1, y - 2));
    } catch (exception e){}

    for(Positie pos: mogelijke_zetten){
        if(game.getSchaakStuk(pos) == nullptr or (game.getSchaakStuk(pos)->getKleur() != getKleur())){
            geldige_zetten.emplace_back(pos);
        }
    }

    if(controleerVoorSchaak) {
        geldige_zetten = filterGeldigeZettenWaarbijKoningSchaakStaat(game, geldige_zetten, this);
    }
    return geldige_zetten;
}

vector<Positie> Loper::geldige_zetten(const Game &game, bool controleerVoorSchaak) const {
    vector<Positie> geldige_zetten = schuineBewegingen(game);
    if(controleerVoorSchaak) {
        geldige_zetten = filterGeldigeZettenWaarbijKoningSchaakStaat(game, geldige_zetten, this);
    }
    return geldige_zetten;
}

vector<Positie> Koning::geldige_zetten(const Game &game, bool controleerVoorSchaak) const {
    vector<Positie> geldige_zetten;
    vector<Positie> mogelijke_zetten;
    Positie currenPosition = game.getPositie(this);

    int x = currenPosition.get_x();
    int y = currenPosition.get_y();

    try {
        mogelijke_zetten.emplace_back(Positie(x, y + 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x - 1, y));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x + 1, y));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x, y - 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x + 1, y - 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x - 1, y - 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x + 1, y + 1));
    } catch (exception e){}
    try {
        mogelijke_zetten.emplace_back(Positie(x - 1, y + 1));
    } catch (exception e){}

    for(Positie pos: mogelijke_zetten){
        if(game.getSchaakStuk(pos) == nullptr or (game.getSchaakStuk(pos)->getKleur() != getKleur())){
            geldige_zetten.emplace_back(pos);
        }
    }

    if(controleerVoorSchaak) {
//        int rokade = game.rokadePossible(getKleur());
//        switch (rokade){
//            case 0: break;
//            case 1: geldige_zetten.emplace_back(Positie(1, y)); break;
//            case 2: geldige_zetten.emplace_back(Positie(5, y)); break;
//            case 3:
//                geldige_zetten.emplace_back(Positie(1, y));
//                geldige_zetten.emplace_back(Positie(5, y));
//                break;
//        }
        geldige_zetten = filterGeldigeZettenWaarbijKoningSchaakStaat(game, geldige_zetten, this);
    }
    return geldige_zetten;;
}

vector<Positie> Koningin::geldige_zetten(const Game &game, bool controleerVoorSchaak) const {
    vector<Positie> schuineBewegingenV = schuineBewegingen(game);
    vector<Positie> rechteBewegingenV = rechteBewegingen(game);
    vector<Positie> geldige_zetten = schuineBewegingenV;
    geldige_zetten.insert(geldige_zetten.end(), rechteBewegingenV.begin(), rechteBewegingenV.end());
    // source: https://stackoverflow.com/questions/201718/concatenating-two-stdvectors

    if(controleerVoorSchaak) {
        geldige_zetten = filterGeldigeZettenWaarbijKoningSchaakStaat(game, geldige_zetten, this);
    }
    return geldige_zetten;
}

vector<Positie> SchaakStuk::rechteBewegingen(const Game &game) const {
    vector<Positie> geldige_zetten;
    Positie currenPosition = game.getPositie(this);
    Positie positie = Positie(0, 0);

    int x = currenPosition.get_x();

    try {
        while (true){
            x += 1;
            positie = Positie(x, currenPosition.get_y());
            if (game.getSchaakStuk(positie) == nullptr) {
                geldige_zetten.emplace_back(positie);
            } else if (game.getSchaakStuk(positie)->getKleur() != getKleur()){
                geldige_zetten.emplace_back(positie);
                break;
            } else {
                break;
            }
        }
    } catch (exception e){}

    x = currenPosition.get_x();

    try {
        while (true){
            x -= 1;
            if(x < 0){ break; }
            positie = Positie(x, currenPosition.get_y());
            if (game.getSchaakStuk(positie) == nullptr) {
                geldige_zetten.emplace_back(positie);
            } else if (game.getSchaakStuk(positie)->getKleur() != getKleur()){
                geldige_zetten.emplace_back(positie);
                break;
            } else {
                break;
            }
        }
    } catch (exception e){}

    int y = currenPosition.get_y();

    try {
        while (true){
            y += 1;
            if (y > 7){ break; }
            positie = Positie(currenPosition.get_x(), y);
            if (game.getSchaakStuk(positie) == nullptr) {
                geldige_zetten.emplace_back(positie);
            } else if (game.getSchaakStuk(positie)->getKleur() != getKleur()){
                geldige_zetten.emplace_back(positie);
                break;
            } else {
                break;
            }
        }
    } catch (exception e){}

    y = currenPosition.get_y();

    try {
        while (true){
            y -= 1;
            if(y < 0){ break; }
            positie = Positie(currenPosition.get_x(), y);
            if (game.getSchaakStuk(positie) == nullptr) {
                geldige_zetten.emplace_back(positie);
            } else if (game.getSchaakStuk(positie)->getKleur() != getKleur()){
                geldige_zetten.emplace_back(positie);
                break;
            } else {
                break;
            }
        }
    } catch (exception e){}

//    geldige_zetten = filterGeldigeZettenWaarbijKoningSchaakStaat(game, geldige_zetten, this);
    return geldige_zetten;
}

vector<Positie> SchaakStuk::schuineBewegingen(const Game &game) const {
    vector<Positie> geldige_zetten;
    Positie currenPosition = game.getPositie(this);
    Positie positie = Positie(0, 0);

    int x = currenPosition.get_x();
    int y = currenPosition.get_y();
    int factor = 0;

    try {
        while (true){
            factor += 1;
            positie = Positie(x+factor, y+factor);
            if (game.getSchaakStuk(positie) == nullptr) {
                geldige_zetten.emplace_back(positie);
            } else if (game.getSchaakStuk(positie)->getKleur() != getKleur()){
                geldige_zetten.emplace_back(positie);
                break;
            } else {
                break;
            }
        }
    } catch (exception e){}

    factor = 0;

    try {
        while (true){
            factor += 1;
            positie = Positie(x+factor, y-factor);
            if (game.getSchaakStuk(positie) == nullptr) {
                geldige_zetten.emplace_back(positie);
            } else if (game.getSchaakStuk(positie)->getKleur() != getKleur()){
                geldige_zetten.emplace_back(positie);
                break;
            } else {
                break;
            }
        }
    } catch (exception e){}

    factor = 0;

    try {
        while (true){
            factor += 1;
            positie = Positie(x-factor, y+factor);
            if (game.getSchaakStuk(positie) == nullptr) {
                geldige_zetten.emplace_back(positie);
            } else if (game.getSchaakStuk(positie)->getKleur() != getKleur()){
                geldige_zetten.emplace_back(positie);
                break;
            } else {
                break;
            }
        }
    } catch (exception e){}

    factor = 0;

    try {
        while (true){
            factor += 1;
            positie = Positie(x-factor, y-factor);
            if (game.getSchaakStuk(positie) == nullptr) {
                geldige_zetten.emplace_back(positie);
            } else if (game.getSchaakStuk(positie)->getKleur() != getKleur()){
                geldige_zetten.emplace_back(positie);
                break;
            } else {
                break;
            }
        }
    } catch (exception e){}

//    geldige_zetten = filterGeldigeZettenWaarbijKoningSchaakStaat(game, geldige_zetten, this);
    return geldige_zetten;
}

vector<Positie>
SchaakStuk::filterGeldigeZettenWaarbijKoningSchaakStaat(const Game &game, vector<Positie> zetten, const SchaakStuk *schaakStuk) const{
//    std::cout << "filteren..." << endl;
    Game gameCopy = game;
    vector<Positie> gefilterde_zetten = zetten;
    // TODO: mss een nieuwe game maken, of toch een kopie van schaakstukken waarop alles getest kan worden...
    // Probleem nu: filterGeldigeZetten blijft telkens opnieuw opgeroepen worden (in een cycle) en dan kom ik magisch
    // op een punt waarbij ik iets negatief wil toevoegen
    for(Positie zet: zetten){
//        std::cout << "next piece" << endl;
        gameCopy.makeMovement(schaakStuk, zet);
        if(gameCopy.schaak(getKleur())){
            gefilterde_zetten.erase(find(gefilterde_zetten.begin(), gefilterde_zetten.end(), zet));
        }
        gameCopy.undo();
        // TODO: game.removeTopFromRedoList();
    }
    return gefilterde_zetten;
}

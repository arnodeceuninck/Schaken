//  Student: Arno Deceuninck
//  Rolnummer: 20181217
//  Opmerkingen: (bvb aanpassingen van de opgave) (en-passent en rokade niet gemaakt, de rest wel)
//

#include <iostream>
#include <algorithm>
#include <QtWidgets/QMessageBox>
#include "mainwindow.h"
#include <random>

#include "game.h"

//#include "SchaakStuk.h"

Game::Game(): movePieceBool(false), pieceToMove(Positie(0,0)), turn(wit), game_over(false), promotie(false){
    int random = rand() % 2;
    if(random == 0){
        AIKleur = zwart;
    } else {
        AIKleur = wit;
    }
}

Game::~Game() {}

// Zet het bord klaar; voeg de stukken op de jusite plaats toe
void Game::setStartBord() {

    schaakstukken = {};

    // Plaats de speciale witte stukken
    plaatsSchaakStuk(new Toren(wit), Positie(0, 0));
    plaatsSchaakStuk(new Paard(wit), Positie(1, 0));
    plaatsSchaakStuk(new Loper(wit), Positie(2, 0));
    plaatsSchaakStuk(new Koning(wit), Positie(3, 0));
    plaatsSchaakStuk(new Koningin(wit), Positie(4, 0));
    plaatsSchaakStuk(new Loper(wit), Positie(5, 0));
    plaatsSchaakStuk(new Paard(wit), Positie(6, 0));
    plaatsSchaakStuk(new Toren(wit), Positie(7, 0));

    // Plaats de speciale zwarte stukken
    plaatsSchaakStuk(new Toren(zwart), Positie(0, 7));
    plaatsSchaakStuk(new Paard(zwart), Positie(1, 7));
    plaatsSchaakStuk(new Loper(zwart), Positie(2, 7));
    plaatsSchaakStuk(new Koningin(zwart), Positie(4, 7));
    plaatsSchaakStuk(new Koning(zwart), Positie(3, 7));
    plaatsSchaakStuk(new Loper(zwart), Positie(5, 7));
    plaatsSchaakStuk(new Paard(zwart), Positie(6, 7));
    plaatsSchaakStuk(new Toren(zwart), Positie(7, 7));

    // Plaats de pionnen
    for (int i = 0; i < 8; ++i) {
        plaatsSchaakStuk(new Pion(wit), Positie(i, 1));
        plaatsSchaakStuk(new Pion(zwart), Positie(i, 6));
    }
}

// Verplaats stuk s naar positie p
// Als deze move niet mogelijk is, wordt false teruggegeven
// en verandert er niets aan het schaakbord.
// Anders wordt de move uitgevoerd en wordt true teruggegeven
bool Game::move(SchaakStuk* s, Positie p) {
    movePieceBool = false;
    vector<Positie> geldige_zetten = s->geldige_zetten(*this, true);
    if (std::find(geldige_zetten.begin(), geldige_zetten.end(), p) == geldige_zetten.end()){
        return false; // De positie zit niet in geldige zetten
    }
//    redo_stack = {}; // Een verplaatsing gemaakt, dus er kan geen redo meer gedaan worden
    makeMovement(s, p);
    if(turn == AIKleur){
        moveAI();
    }
}

bool Game::makeMovement(const SchaakStuk* s, Positie p) {
//    if(s->piece().type() == Piece::King and
//        rokadePossible(s->getKleur()) and
//            (p.get_x() == 1 or p.get_x() == 5)){
//        rokadeMove(s->getKleur(), p.get_x());
//        return true;
//    } else if(s->piece().type() == Piece::Rook and
//              rokadePossible(s->getKleur()) and
//              (p.get_x() == 2 or p.get_x() == 4)){
//        rokadeMove(s->getKleur(), p.get_x());
//        return true;
//    }

    for(GeplaatstSchaakStuk* schaakStuk: schaakstukken){
        SchaakStuk* vorig_schaakstuk = getSchaakStuk(p);
        GeplaatstSchaakStuk* vorig_stuk = nullptr; // vorige schaakstuk op die plaats
        if (schaakStuk->schaakStuk == s){
            if (vorig_schaakstuk != nullptr and vorig_schaakstuk->getKleur() != s->getKleur()){
                vorig_stuk = new GeplaatstSchaakStuk(p, vorig_schaakstuk);
                std::cout << "Nice capture." << endl;
                if(vorig_schaakstuk->piece().type() == Piece::King){
                    game_over = true;
                    std::cout << "Winner: ";
                    if(turn == zwart){
                        std::cout << "black";
                    } else {
                        std::cout << "white";
                    }
                    std::cout << std::endl;
                }
                destroyPiece(p);
            }

            GeplaatstSchaakStuk* oud_stuk = new GeplaatstSchaakStuk(schaakStuk->positie, schaakStuk->schaakStuk); // orginele positie TODO: neemt dit een copy van de positie
            GeplaatstSchaakStuk* nieuw_stuk = new GeplaatstSchaakStuk(p, schaakStuk->schaakStuk); // nieuwe positie
            vector<GeplaatstSchaakStuk*> undoStukken = {oud_stuk, nieuw_stuk, vorig_stuk};

            undo_stack.emplace_back(undoStukken);
            schaakStuk->positie = p;
            firstTurn_stack.push_back(schaakStuk->schaakStuk->isFirstTime());
            schaakStuk->schaakStuk->setFirstTime(false);

            if(schaakStuk->schaakStuk->piece().type() == Piece::Pawn){
                // Controleren op promotie
                if((schaakStuk->schaakStuk->getKleur() == zwart and schaakStuk->positie.get_y() == 0) or
                        (schaakStuk->schaakStuk->getKleur() == wit and schaakStuk->positie.get_y() == 7)){
                    promotie = true;
                    promotiestuk = schaakStuk;
                }
            }

            if(pat(zwart) or pat(wit)){
                game_over = true;
                QMessageBox pat;
                pat.setText(QString("Pat!"));
                pat.exec();
            }
            switchTurn();

            return true;
        }
    }
    std::cout << "Not found..." << endl;
    return false;
}

// Geeft true als kleur schaak staat
bool Game::schaak(zw kleur) const{
    Positie koning_positie = Positie(0, 0);

    // vind de koning
    for(GeplaatstSchaakStuk* piece: schaakstukken){
        if(piece->schaakStuk->getKleur() == kleur and piece->schaakStuk->piece().type() == Piece::King){
            koning_positie = piece->positie;
        }
    }

    // kijk of hij schaak staat
    for(GeplaatstSchaakStuk* piece: schaakstukken){
        if(piece->schaakStuk->getKleur() != kleur){
            for(Positie pos: piece->schaakStuk->geldige_zetten(*this, false)){
                if(pos == koning_positie){
                    std::cout << "Schaak." << endl;
                    return true;
                }
            }
        }
    }

    return false;
}

// Geeft true als kleur schaakmat staat
bool Game::schaakmat(zw kleur) const{
    bool mogelijke_ontsnapping = false;
    // Alle mogelijke verplaatsingen van deze kleur testen en kijken of de er ontsnapt kan worden aan de schaak
    for(GeplaatstSchaakStuk* schaakStuk: schaakstukken){
        if(schaakStuk->schaakStuk->getKleur() == kleur){
            if(schaakStuk->schaakStuk->geldige_zetten(*this, true).size() != 0){
                mogelijke_ontsnapping = true;
                return false;
            }
        }
    }
    return !mogelijke_ontsnapping;
}

// Geeft true als kleur pat staat
// (pat = geen geldige zet mogelijk, maar kleur staat niet schaak;
// dit resulteert in een gelijkspel)
bool Game::pat(zw kleur) const{
    if(schaak(kleur)){
        return false;
    }
    for(GeplaatstSchaakStuk* schaakStuk: schaakstukken){
        if(schaakStuk->schaakStuk->getKleur() == kleur){
            if(!schaakStuk->schaakStuk->geldige_zetten(*this, false).empty()){
                return false;
            }
        }
    }
    return true;
}


bool Game::plaatsSchaakStuk(SchaakStuk *stuk, Positie pos) {
    GeplaatstSchaakStuk* newStuk = new GeplaatstSchaakStuk(pos, stuk);
    schaakstukken.push_back(newStuk);
    return true;
}

SchaakStuk* Game::getSchaakStuk(Positie positie) const{
    for(GeplaatstSchaakStuk* schaakStuk: schaakstukken){
        if (schaakStuk->positie == positie){
            return schaakStuk->schaakStuk;
        }
    }

    return nullptr;
}

const vector<GeplaatstSchaakStuk*> &Game::getSchaakstukken() const {
    return schaakstukken;
}

Positie Game::getPositie(const SchaakStuk *schaakStuk) const {
    for (GeplaatstSchaakStuk* geplaatstSchaakStuk: schaakstukken) {
        if (geplaatstSchaakStuk->schaakStuk == schaakStuk) {
            return geplaatstSchaakStuk->positie;
        }
    }
}

GeplaatstSchaakStuk* Game::getGeplaatstSchaakStuk(SchaakStuk *schaakStuk) {
    for (GeplaatstSchaakStuk* geplaatstSchaakStuk: schaakstukken) {
        if (geplaatstSchaakStuk->schaakStuk == schaakStuk) {
            return geplaatstSchaakStuk;
        }
    }
    std::cout << "Not found..." << endl;
}

const Positie &Game::getPieceToMove() const {
    return pieceToMove;
}

void Game::setPieceToMove(const Positie &pieceToMove) {
    movePieceBool = true;
    Game::pieceToMove = pieceToMove;
}

void Game::setSchaakstukken(const vector<GeplaatstSchaakStuk*> &schaakstukken) {
    Game::schaakstukken = schaakstukken;
}

bool Game::movePiece() {
    return movePieceBool;
}

void Game::destroyPiece(const Positie positie) {
    for (int i = 0; i < schaakstukken.size(); ++i){
        GeplaatstSchaakStuk* schaakStuk = schaakstukken[i];
        if (schaakStuk->positie == positie){
            schaakstukken.erase(schaakstukken.begin() + i);
            return;
        }
    }
}

zw Game::getTurn() const {
    return turn;
}

bool Game::isGame_over() const {
    return game_over;
}

void Game::setTurn(zw turn) {
    Game::turn = turn;
}

void Game::undo() {
    if (undo_stack.size() == 0){
        return;
    }
    int last_index = undo_stack.size()-1;
    vector<GeplaatstSchaakStuk*> changedPieces;
    changedPieces.push_back(undo_stack[last_index][0]);
    changedPieces.push_back(undo_stack[last_index][1]);
    changedPieces.push_back(undo_stack[last_index][2]);

    redo_stack.push_back(changedPieces[1]);

    if(changedPieces[2] != nullptr){
        schaakstukken.push_back(changedPieces[2]);
    }
    for (int i = 0; i < schaakstukken.size(); ++i) {
        if(schaakstukken[i]->schaakStuk == changedPieces[1]->schaakStuk){
            Positie orginele_positie = changedPieces[0]->positie;
            schaakstukken[i]->positie = orginele_positie;
            schaakstukken[i]->schaakStuk->setFirstTime(firstTurn_stack[last_index]);
            break;
        }
    }

    game_over = false;
    firstTurn_stack.erase(firstTurn_stack.begin()+last_index);
    undo_stack.erase(undo_stack.begin()+last_index);
    switchTurn();
}

void Game::redo() {
    int last_index = redo_stack.size()-1;
    if (last_index < 0){
        cout << "Nothing to redo." << endl;
        return;
    }
    move(redo_stack[last_index]->schaakStuk, redo_stack[last_index]->positie);
    redo_stack.erase(redo_stack.begin()+last_index);
}


void Game::switchTurn() {
    if(turn == wit){
        turn = zwart;
    } else {
        turn = wit;
    }
}

bool Game::isPromotie() const {
    return promotie;
}

GeplaatstSchaakStuk *Game::getPromotiestuk() const {
    return promotiestuk;
}

void Game::setPromotiestuk(SchaakStuk *promotiestuk) {
    SchaakStuk* temp = Game::promotiestuk->schaakStuk;
    Game::promotiestuk->schaakStuk = promotiestuk;
    delete temp;
}

void Game::setPromotie(bool promotie) {
    Game::promotie = promotie;
}

void Game::moveAI() {
    if(game_over == true){
        return;
    }
    zw enemy;
    if(AIKleur == zwart){
        enemy = wit;
    } else {
        enemy = zwart;
    }

    Game gameCopy = *this;
    vector<GeplaatstSchaakStuk> geldige_zetten;
    vector<GeplaatstSchaakStuk> schaak_zetten;
    vector<GeplaatstSchaakStuk> stuk_slaan_zetten;
    vector<GeplaatstSchaakStuk> schaakmat_zetten;
    int schaakStukkenSize = gameCopy.getSchaakstukken().size();
    for(GeplaatstSchaakStuk* schaakStuk: schaakstukken){
        if(schaakStuk->schaakStuk->getKleur() != AIKleur){
            continue;
        }
        for(Positie pos: schaakStuk->schaakStuk->geldige_zetten(*this, true)){
            geldige_zetten.emplace_back(GeplaatstSchaakStuk(pos, schaakStuk->schaakStuk));
            gameCopy.makeMovement(schaakStuk->schaakStuk, pos);
            if(gameCopy.schaakmat(enemy)){
                schaakmat_zetten.emplace_back(GeplaatstSchaakStuk(pos, schaakStuk->schaakStuk));
            }
            if(gameCopy.schaak(enemy)){
                schaak_zetten.emplace_back(GeplaatstSchaakStuk(pos, schaakStuk->schaakStuk));
            }
            if(gameCopy.getSchaakstukken().size() < schaakStukkenSize){
                stuk_slaan_zetten.emplace_back(GeplaatstSchaakStuk(pos, schaakStuk->schaakStuk));
            }
            gameCopy.undo();
        }
    }

    vector<GeplaatstSchaakStuk> lijstOmUitTeKiezen;
    if(!schaakmat_zetten.empty()) {
        lijstOmUitTeKiezen = schaakmat_zetten;
    } else if (!stuk_slaan_zetten.empty()){
        lijstOmUitTeKiezen = stuk_slaan_zetten;
    } else if(!schaak_zetten.empty()){
        lijstOmUitTeKiezen = schaak_zetten;
    } else {
        lijstOmUitTeKiezen = geldige_zetten;
    }
    if(lijstOmUitTeKiezen.size() == 0){
        game_over = true;
        return;
    }
    int zet = rand() % lijstOmUitTeKiezen.size();
    GeplaatstSchaakStuk stuk = lijstOmUitTeKiezen[zet];
    move(stuk.schaakStuk, stuk.positie);
    if(promotie and promotiestuk->schaakStuk->getKleur() == AIKleur){
        setPromotiestuk(new Koningin(AIKleur));
    }
}

zw Game::getAIKleur() const {
    return AIKleur;
}

void Game::setGame_over(bool game_over) {
    Game::game_over = game_over;
}

//int Game::rokadePossible(zw kleur) const{
//    // Returnt 0 als het niet mogelijk is, 1 als het naar links mogelijk is, 2 naar rechts, 3 links en rechts
//    GeplaatstSchaakStuk* koning;
//    GeplaatstSchaakStuk* torenL = nullptr;
//    GeplaatstSchaakStuk* torenR = nullptr;
//    for(GeplaatstSchaakStuk* schaakStuk: schaakstukken){
//        if(schaakStuk->schaakStuk->getKleur() == kleur and schaakStuk->schaakStuk->piece().type() == Piece::King){
//            koning = schaakStuk;
//        }
//        if(schaakStuk->schaakStuk->getKleur() == kleur and schaakStuk->schaakStuk->piece().type() == Piece::Rook){
//            if(torenL == nullptr){
//                torenL = schaakStuk;
//            } else {
//                if(torenL->positie.get_x() > koning->positie.get_x()){
//                    torenR = torenL;
//                    torenL = schaakStuk;
//                } else {
//                    torenR = schaakStuk;
//                }
//
//            }
//        }
//    }
//    if(koning->schaakStuk->isFirstTime() == false or
//    schaak(kleur)){
//        cout << "yolo" << endl;
//        return 0;
//    }
//
//    int return_value = 0;
//    if(torenL != nullptr and torenL->schaakStuk->isFirstTime() == true){
//        // toren eerste keer, koning eerste keer
//        if(getSchaakStuk(Positie(1, torenL->positie.get_y())) == nullptr
//        and getSchaakStuk(Positie(2, torenL->positie.get_y())) == nullptr){
//            return_value += 1;
//        }
//    }
//
//    if(torenR != nullptr and torenR->schaakStuk->isFirstTime() == true){
//        // toren eerste keer, koning eerste keer
//        if(getSchaakStuk(Positie(6, torenL->positie.get_y())) == nullptr
//           and getSchaakStuk(Positie(5, torenL->positie.get_y())) == nullptr
//           and getSchaakStuk(Positie(4, torenL->positie.get_y())) == nullptr){
//            return_value += 2;
//        }
//    }
//    cout << to_string(return_value) << endl;
//    return return_value;
//}
//
//void Game::rokadeMove(zw kleur, int x) {
//    previousMoveSpecialMove = true;
//    int xKing;
//    int xRook;
//    int yRookoriginal;
//    int xRookoriginal;
//
//    if(kleur == zwart){
//        yRookoriginal = 7;
//    } else {
//        yRookoriginal = 0;
//    }
//
//    if(x < 4){
//        // rokade naar links
//        xKing = 1;
//        xRook = 2;
//        xRookoriginal = 0;
//    } else {
//        xKing = 6;
//        xRook = 5;
//        xRookoriginal = 7;
//    }
//
//    for(GeplaatstSchaakStuk* schaakStuk: schaakstukken){
//        if(schaakStuk->schaakStuk->getKleur() == kleur and schaakStuk->schaakStuk->piece().type() == Piece::King){
//            Positie pos = schaakStuk->positie;
//            schaakStuk->positie = Positie(xKing, pos.get_y());
//        }
//
//        if(schaakStuk->schaakStuk->getKleur() == kleur and
//            schaakStuk->schaakStuk->piece().type() == Piece::Rook and
//            schaakStuk->positie.get_x() == xRookoriginal and
//            schaakStuk->positie.get_y() == yRookoriginal){
//
//            Positie pos = schaakStuk->positie;
//            schaakStuk->positie = Positie(xRook, yRookoriginal);
//        }
//    }
//    cout << "Done" << endl;
//}


GeplaatstSchaakStuk::GeplaatstSchaakStuk(const Positie &positie, SchaakStuk *schaakStuk) : positie(positie),
                                                                                           schaakStuk(schaakStuk) {}

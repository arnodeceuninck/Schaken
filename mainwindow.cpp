#include "mainwindow.h"
#include "SchaakStuk.h"
#include <QMessageBox>
#include <QtWidgets>
#include <iostream>
#include "chessboard.h" // nodig om enum type te gebruiken


enum Type { King, Queen, Bishop, Knight, Rook, Pawn, None };

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    scene = new ChessBoard;
    QGraphicsView *view = new QGraphicsView(scene);
    setCentralWidget(view);

    connect(scene,SIGNAL(clicked(int,int)),this,SLOT(clicked(int,int)));

    createActions();
    createMenus();

    newGame();
}


// Deze functie wordt opgeroepen telkens er op het schaakbord
// geklikt wordt. x,y geeft de positie aan waar er geklikt
// werd; r is de 0-based rij, k de 0-based kolom
void MainWindow::clicked(int r, int k) {
    if (g.isGame_over()){
        newGame();
        return;
    }
    if(g.isPromotie() and g.getPromotiestuk()->schaakStuk->getKleur() != g.getAIKleur()){
        promotieClickHandler(r, k);
        return;
    }
    scene->setPieceFocus(r,k,!scene->hasPieceFocus(r,k));
    if (!g.movePiece()) {
        SchaakStuk* piece = g.getSchaakStuk(Positie(k, r));
        if(piece == nullptr){
            std::cout << "You can only move pieces." << endl;
            scene->removeAllFocus();
        } else if(g.getTurn() == piece->getKleur()) {
            Positie pieceToMove = Positie(k, r);
            cout << "Piece set" << endl;
            g.setPieceToMove(pieceToMove);
            vector<Positie> mogelijke_zetten = g.getSchaakStuk(pieceToMove)->geldige_zetten(g, true);
            for(Positie pos: mogelijke_zetten){
                scene->setTileFocus(pos.get_y(), pos.get_x(), true);
            }
        } else {
            std::cout << "It's not your turn." << endl;
            scene->removeAllFocus();
        }
    } else {
//        std::cout << "Moving..." << endl;
        Positie moveFrom = g.getPieceToMove();
        SchaakStuk* pieceToMove = g.getSchaakStuk(moveFrom);
        g.move(pieceToMove, Positie(k, r));
        scene->removeAllFocus();
        update();
        if(g.schaak(zwart) or g.schaak(wit)){
            QMessageBox schaak;
            schaak.setText(QString("Schaak!"));
            schaak.exec();
        }
        if(g.schaakmat(zwart) or g.schaakmat(wit)){
            QMessageBox schaakmat;
            schaakmat.setText(QString("Schaakmat! R.I.P."));
            schaakmat.exec();
            g.setGame_over(true);
        }
        if(g.isPromotie() and g.getPromotiestuk()->schaakStuk->getKleur() != g.getAIKleur()){
            QMessageBox promotie;
            promotie.setText(QString("Selecteer op het volgende scherm het schaakstuk tot wat je je pion wil promoveren"));
            promotie.exec();
            promotieView();
        }
    }

//    SchaakStuk* s=new Toren(wit);
//    scene->setItem(r,k,s->piece());
//    delete s;

//    QMessageBox joehoe;
//    joehoe.setText(QString("Je hebt een pion gezet. Wat knap!"));
//    joehoe.exec();

//    scene->removeAllFocus();

//    for (int i=0;i<5;i++) {
//        scene->setTileFocus(0, i, !scene->hasTileFocus(r, k));
//        scene->setTileFocus(i, 2, !scene->hasTileFocus(r, k));
//    }

//    joehoe.setText(QString("Kijk, de letter T!"));
//    joehoe.exec();

//    scene->removeAllFocus();

}

void MainWindow::newGame(){
    g = Game();
    g.setStartBord();
    update();
    if(g.getAIKleur() == wit){
        g.moveAI();
        update();
    }
}

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save game"), "",
                                                    tr("Chess File (*.chs);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QDataStream out(&file);
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                SchaakStuk* schaakStuk = g.getSchaakStuk(Positie(x, y));
                if (schaakStuk == nullptr){
                    out << QString(".");
                    continue;
                }

                QString pieceString = "";
                Type type = static_cast<Type>(schaakStuk->piece().type());
                switch (type){
                    case King: pieceString += "K"; break;
                    case Queen: pieceString += "Q"; break;
                    case Rook: pieceString += "R"; break;
                    case Bishop: pieceString += "B"; break;
                    case Knight: pieceString += "H"; break;
                    case Pawn: pieceString += "P"; break;
                    case None:
                        out << QString(".");
                        continue;
                }
                zw kleur = schaakStuk->getKleur();
                if(kleur==zwart){
                    pieceString += "b";
                } else {
                    pieceString += "w";
                }
                out << QString(pieceString);
            }

        }

        QString turn;
        if(g.getTurn() == zwart){
            turn = "blackTurn";
        } else {
            turn = "whiteTurn";
        }
        out << turn;
//        out << QString("Rb") << QString("Hb") << QString("Bb") << QString("Qb") << QString("Kb") << QString("Bb") << QString("Hb") << QString("Rb");
//        for  (int i=0;i<8;i++) {
//            out << QString("Pb");
//        }
//        for  (int r=3;r<7;r++) {
//            for (int k=0;k<8;k++) {
//                out << QString(".");
//            }
//        }
//        for  (int i=0;i<8;i++) {
//            out << QString("Pw");
//        }
//        out << QString("Rw") << QString("Hw") << QString("Bw") << QString("Qw") << QString("Kw") << QString("Bw") << QString("Hw") << QString("Rw");

    }
}

void MainWindow::open() {

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load game"), "",
                                                    tr("Chess File (*.chs);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        try {
            g = Game();
            QDataStream in(&file);
            QString debugstring;
            for (int r=0;r<8;r++) {
                for (int k=0;k<8;k++) {
                    QString piece;
                    in >> piece;
                    debugstring += "\t" + piece;
                    if (in.status()!=QDataStream::Ok) {
                        throw QString("Error reading file "+fileName);
                    }

                    if(piece == "."){
                        continue;
                    }

                    QCharRef type = piece[0];
                    QCharRef colorC = piece[1];
                    zw color;

                    if(colorC == "b"){
                        color = zwart;
                    } else {
                        color = wit;
                    }

                    if(type == "B"){
                        g.plaatsSchaakStuk(new Loper(color), Positie(k, r));
                    } else if(type == "H"){
                        g.plaatsSchaakStuk(new Paard(color), Positie(k, r));
                    } else if(type == "R"){
                        g.plaatsSchaakStuk(new Toren(color), Positie(k, r));
                    } else if(type == "K"){
                        g.plaatsSchaakStuk(new Koning(color), Positie(k, r));
                    } else if(type == "Q"){
                        g.plaatsSchaakStuk(new Koningin(color), Positie(k, r));
                    } else if(type == "P"){
                        g.plaatsSchaakStuk(new Pion(color), Positie(k, r));
                    }

                }
                debugstring += "\n";
            }

            QString turn;
            in >> turn;
            if(turn == "blackTurn"){
                g.setTurn(zwart);
            } else {
                g.setTurn(wit);
            }
//            QMessageBox::information(this, tr("Debug"),
//                                     debugstring);
        } catch (QString& Q) {
            QMessageBox::information(this, tr("Error reading file"),
                                     Q);
        }
    }
    update();
}


void MainWindow::undo() {
    g.undo();
    update();
}
void MainWindow::redo() {
    g.redo();
    update();
}

// Update de inhoud van de grafische weergave van het schaakbord (scene)
// en maak het consistent met de game state in variabele g.
// Opdracht 1.3
void MainWindow::update() {
    scene->clearBoard();
    for(GeplaatstSchaakStuk* schaakStuk: g.getSchaakstukken()) {
        int x = schaakStuk->positie.get_x();
        int y = schaakStuk->positie.get_y();
        scene->setItem(y, x, schaakStuk->schaakStuk->piece());

    }
}


void MainWindow::promotieView() {
    scene->clearBoard();
    scene->setItem(0, 0, Piece(Piece::Queen,g.getPromotiestuk()->schaakStuk->getKleur()==wit?Piece::White:Piece::Black));
    scene->setItem(0, 1, Piece(Piece::Rook,g.getPromotiestuk()->schaakStuk->getKleur()==wit?Piece::White:Piece::Black));
    scene->setItem(0, 2, Piece(Piece::Bishop,g.getPromotiestuk()->schaakStuk->getKleur()==wit?Piece::White:Piece::Black));
    scene->setItem(0, 3, Piece(Piece::Knight,g.getPromotiestuk()->schaakStuk->getKleur()==wit?Piece::White:Piece::Black));
}


void MainWindow::promotieClickHandler(int r, int k) {
    if(r==0 and k==0){
        g.setPromotiestuk(new Koningin(g.getPromotiestuk()->schaakStuk->getKleur()));
        g.setPromotie(false);
        update();
    } else if(r==0 and k==1){
        g.setPromotiestuk(new Toren(g.getPromotiestuk()->schaakStuk->getKleur()));
        g.setPromotie(false);
        update();
    } else if(r==0 and k==2){
        g.setPromotiestuk(new Loper(g.getPromotiestuk()->schaakStuk->getKleur()));
        g.setPromotie(false);
        update();
    } else if(r==0 and k==3){
        g.setPromotiestuk(new Paard(g.getPromotiestuk()->schaakStuk->getKleur()));
        g.setPromotie(false);
        update();
    } else {
        QMessageBox promotie;
        promotie.setText(QString("Gelieve een schaakstuk te selecteren."));
        promotie.exec();
    }
}



////////////////






void MainWindow::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Start a new game"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newGame);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Read game from disk"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save game to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Abandon game"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::on_actionExit_triggered);

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo last move"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    redoAct = new QAction(tr("&redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo last undone move"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exitAct);

    gameMenu = menuBar()->addMenu(tr("&Game"));
    gameMenu->addAction(undoAct);
    gameMenu->addAction(redoAct);
}

void MainWindow::on_actionExit_triggered() {
    if (QMessageBox::Yes == QMessageBox::question(this,
                                                  tr("Spel verlaten"),
                                                  tr("Bent u zeker dat u het spel wil verlaten?\nNiet opgeslagen wijzigingen gaan verloren.")))
    {
        QApplication::quit();
    }
}


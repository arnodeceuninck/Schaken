//
//  !!! Dit bestand moet je in principe NIET wijzigen !!!
//

#include "chessboard.h"
#include "resources.h"

#include <QtWidgets>
#include <QPixmap>


ChessBoard::ChessBoard(QObject *parent) :
    QGraphicsScene(parent)
{
    nPieceWidth = 45;
    nBorderWidth = 0;

    removeAllFocus();

    cLightPieceColor = Qt::black;
    cDarkPieceColor = Qt::black;
    cLightSquareColor = Qt::white;
    cDarkSquareColor = Qt::gray;
    cDarkSquareColorFocus = Qt::darkRed;
    cLightSquareColorFocus = QColor(255,0,0);
    cDarkPieceColorFocus = QColor(0,255,0);
    cLightPieceColorFocus = QColor(0,255,0);

    redrawEntireBoard();
}

void ChessBoard::removeAllFocus() {
    for (int i=0;i<8;i++) {
        for (int j=0;j<8;j++) {
            focusCell[i][j]=false;
            focusPiece[i][j]=false;
        }
    }
    redrawEntireBoard();
}

void ChessBoard::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    QPointF scenePos = e->scenePos();
    if( scenePos.x() < 0 || scenePos.y() < 0 || scenePos.x() > 8*nPieceWidth || scenePos.y() > 8*nPieceWidth )
    {
        focusRow = -1;
        focusCol = -1;
        return;
    }

    focusRow = rowFromPoint( scenePos.y() );
    focusCol = colFromPoint( scenePos.x() );

    if (focusRow>=0 && focusRow<8 && focusCol>=0 && focusCol<8) {
        emit(clicked((int) focusRow, (int) focusCol));
    }

    QGraphicsScene::mousePressEvent(e);
}

void ChessBoard::setTileFocus(int x, int y,bool f=true) {
    focusCell[x][y]=f;
    refreshTile(x,y);
    refreshImage(x,y);
}

void ChessBoard::setPieceFocus(int x, int y, bool f) {
    focusPiece[x][y]=f;
    refreshImage(x,y);
}

void ChessBoard::drawTile(int i, int j)
{
    QGraphicsRectItem *rect = new QGraphicsRectItem( j * nPieceWidth , i * nPieceWidth , nPieceWidth , nPieceWidth );
    if( i % 2 == j % 2 )
        if (focusCell[i][j]) {
            rect->setBrush(QBrush(cLightSquareColorFocus, Qt::SolidPattern));
        } else {
            rect->setBrush(QBrush(cLightSquareColor, Qt::SolidPattern));
        }
    else
    if (focusCell[i][j]) {
        rect->setBrush(QBrush(cDarkSquareColorFocus, Qt::SolidPattern));
    } else {
        rect->setBrush(QBrush(cDarkSquareColor, Qt::SolidPattern));
    }
    rect->setCacheMode(QGraphicsItem::NoCache);
    addItem(rect);
}

void ChessBoard::refreshTile(int i, int j) {
    QGraphicsItem *currentItem = itemAt( j * nPieceWidth , i * nPieceWidth , QTransform() );
    if( currentItem != 0 && currentItem->data(0) == 777 )
        delete currentItem;

    drawTile(i,j);
}


void ChessBoard::drawBoard()
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            drawTile(i,j);
        }
    }
}

void ChessBoard::setItem(int i, int j, Piece p)
{
    board[i][j] = p;
    refreshImage(i,j);
}

void ChessBoard::refreshBoard()
{
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            refreshImage(i,j);
}

void ChessBoard::redrawEntireBoard()
{
    qDeleteAll( items() );
    drawBoard();
    refreshBoard();
}

void ChessBoard::clearBoard()
{
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            setItem(i,j,Piece());
}

void ChessBoard::refreshImage(int i, int j)
{
    QGraphicsItem *currentItem = itemAt( xFromCol(j) , yFromRow(i) , QTransform() );
    if( currentItem != 0 && currentItem->data(0) == 777 )
        delete currentItem;

    QString filename = getPieceFilename( board[i][j] );
    if(filename.isEmpty())
        return;

    quint32 y = nPieceWidth * i;
    quint32 x = nPieceWidth * j;
    QGraphicsPixmapItem* item=new QGraphicsPixmapItem(QPixmap(filename));

    QGraphicsColorizeEffect *colorize = new QGraphicsColorizeEffect;
    if( board[i][j].color() == Piece::White )
        if (focusPiece[i][j]) colorize->setColor(cLightPieceColorFocus);
        else colorize->setColor(cLightPieceColor);
    else
        if (focusPiece[i][j]) colorize->setColor(cDarkPieceColorFocus);
        else colorize->setColor(cDarkPieceColor);
    item->setGraphicsEffect( colorize );


    item->setCacheMode(QGraphicsItem::NoCache); // needed for proper rendering
    item->setData(0, 777 );

    addItem(item);
    item->setPos(x,y);
}

QString ChessBoard::getPieceFilename(Piece p)
{
    if( p.type() == Piece::None )
        return "";

    QString filename = QString(path);
    switch(p.color())
    {
    case Piece::White:
        filename += "white";
        break;
    case Piece::Black:
    default:
        filename += "black";
        break;
    }

    switch(p.type())
    {
    case Piece::King:
        filename += "-king";
        break;
    case Piece::Queen:
        filename += "-queen";
        break;
    case Piece::Bishop:
        filename += "-bishop";
        break;
    case Piece::Knight:
        filename += "-knight";
        break;
    case Piece::Rook:
        filename += "-rook";
        break;
    case Piece::Pawn:
        filename += "-pawn";
        break;
    default:
        break;
    }

    return filename + ".svg";
}


void ChessBoard::setInitialPositions()
{
    clearBoard();
    setItem(0, 0, Piece(Piece::Rook,Piece::Black));
    setItem(0, 1, Piece(Piece::Knight,Piece::Black));
    setItem(0, 2, Piece(Piece::Bishop,Piece::Black));
    setItem(0, 3, Piece(Piece::Queen,Piece::Black));
    setItem(0, 4, Piece(Piece::King,Piece::Black));
    setItem(0, 5, Piece(Piece::Bishop,Piece::Black));
    setItem(0, 6, Piece(Piece::Knight,Piece::Black));
    setItem(0, 7, Piece(Piece::Rook,Piece::Black));
    setItem(1, 0, Piece(Piece::Pawn,Piece::Black));
    setItem(1, 1, Piece(Piece::Pawn,Piece::Black));
    setItem(1, 2, Piece(Piece::Pawn,Piece::Black));
    setItem(1, 3, Piece(Piece::Pawn,Piece::Black));
    setItem(1, 4, Piece(Piece::Pawn,Piece::Black));
    setItem(1, 5, Piece(Piece::Pawn,Piece::Black));
    setItem(1, 6, Piece(Piece::Pawn,Piece::Black));
    setItem(1, 7, Piece(Piece::Pawn,Piece::Black));

    setItem(7, 0, Piece(Piece::Rook,Piece::White));
    setItem(7, 1, Piece(Piece::Knight,Piece::White));
    setItem(7, 2, Piece(Piece::Bishop,Piece::White));
    setItem(7, 3, Piece(Piece::Queen,Piece::White));
    setItem(7, 4, Piece(Piece::King,Piece::White));
    setItem(7, 5, Piece(Piece::Bishop,Piece::White));
    setItem(7, 6, Piece(Piece::Knight,Piece::White));
    setItem(7, 7, Piece(Piece::Rook,Piece::White));
    setItem(6, 0, Piece(Piece::Pawn,Piece::White));
    setItem(6, 1, Piece(Piece::Pawn,Piece::White));
    setItem(6, 2, Piece(Piece::Pawn,Piece::White));
    setItem(6, 3, Piece(Piece::Pawn,Piece::White));
    setItem(6, 4, Piece(Piece::Pawn,Piece::White));
    setItem(6, 5, Piece(Piece::Pawn,Piece::White));
    setItem(6, 6, Piece(Piece::Pawn,Piece::White));
    setItem(6, 7, Piece(Piece::Pawn,Piece::White));
}

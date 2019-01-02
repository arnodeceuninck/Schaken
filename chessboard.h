//
//  !!! Dit bestand moet je in principe NIET wijzigen !!!
//

#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsScene>

class QAction;
class QActionGroup;

class Piece
{
public:
    enum Type { King, Queen, Bishop, Knight, Rook, Pawn, None };
    enum Color { White, Black };

    Piece() { eType = None; eColor = White; }
    Piece(Type t, Color c) { eType = t; eColor = c; }

    Type type() const { return eType; }
    Color color() const { return eColor; }
    void setType(Type t) { eType = t; }
    void setColor(Color c) { eColor = c; }

private:
    Type eType;
    Color eColor;
};

class ChessBoard : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ChessBoard(QObject *parent = 0);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void clicked(int x,int y);

public:
    void setItem(int i, int j, Piece p);
    void clearBoard();

    void setInitialPositions();
    void setTileFocus(int x, int y, bool f);
    void setPieceFocus(int x, int y, bool f);
    void removeAllFocus();
    bool hasTileFocus(int x,int y) const {return focusCell[x][y];}
    bool hasPieceFocus(int x,int y) const {return focusPiece[x][y];}

private:
    void drawTile(int i, int j);
    void refreshTile(int i, int j);
    void refreshImage(int i, int j);
    void refreshBoard();
    void redrawEntireBoard();

    void drawBoard();
    QString getPieceFilename(Piece p);

    Piece board[8][8];
    bool focusCell[8][8];
    bool focusPiece[8][8];

    quint8 rowFromPoint(int y) const { return y / nPieceWidth; }
    quint8 colFromPoint(int x) const { return x / nPieceWidth; }

    quint32 xFromCol(int c) const { return c*nPieceWidth + 0.5*nPieceWidth; }
    quint32 yFromRow(int r) const { return r*nPieceWidth + 0.5*nPieceWidth; }

    QColor cLightSquareColor;
    QColor cDarkSquareColor;
    QColor cLightPieceColor;
    QColor cDarkPieceColor;
    QColor cDarkSquareColorFocus;
    QColor cLightSquareColorFocus;
    QColor cDarkPieceColorFocus;
    QColor cLightPieceColorFocus;

    quint32 nPieceWidth;
    quint32 nBorderWidth;

    qint8 focusRow, focusCol;
};

#endif // CHESSBOARD_H

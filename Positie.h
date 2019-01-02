//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_POSITIE_H
#define SCHAKEN_POSITIE_H

#include <sstream>

using namespace std;

class IllegalPosition: public exception
{
public:
    IllegalPosition(string pos):pos(pos) {}
    virtual const char* what() const throw()
    {
        return (string("Illegal board position ")+pos+" was used.").c_str();
    }
private:
    string pos;
};


class Positie{
public:
    Positie(string posstring) {
        if (!(posstring.size()==2 && posstring[0]>='a' && posstring[0]<='h' && posstring[1]>='1' && posstring[1]<='8')) {
            x=-1;y=-1;
            throw(IllegalPosition(posstring));
        } else {
            x=int(posstring[0])-int('a');
            y=int(posstring[1])-int('1');
        }
    }

    Positie(int xin, int yin) {
        if (!(xin>=0 && xin<8 && yin>=0 && yin<8)) {
            x=-1;y=-1;
            stringstream s;
            s<<"("<<xin<<","<<yin<<")";
            throw(IllegalPosition(s.str()));
        } else {
            x=xin;
            y=yin;
        }
    }

    string getpositie() const {
        if (x==-1 || y==-1) throw(IllegalPosition("UNDEFINED"));
        return string(1,char(x+int('a')))+char(y+int('1'));
    }

    int get_x() const {
        if (x==-1 || y==-1) throw(IllegalPosition("UNDEFINED"));
        return x;
    }
    int get_y() const {
        if (x==-1 || y==-1) throw(IllegalPosition("UNDEFINED"));
        return y;
    }

    bool operator==(const Positie& p) const {
        return p.x==x && p.y==y;
    }
private:
    int x,y;
};

ostream& operator<<(ostream& s,Positie p);




#endif //SCHAKEN_POSITIE_H

//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "Positie.h"

ostream& operator<<(ostream& s,Positie p) {
    s<<p.getpositie();
    return s;
}
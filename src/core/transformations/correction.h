#ifndef CORRECTION_H
#define CORRECTION_H

#include "transformation.h"

class Correction : public Transformation
{
public:
    Correction(PNM*);
    Correction(PNM*, ImageViewer*);

    virtual PNM* transform();

protected:
    inline const int    add(double, double);
    inline const double mult(int, double);
    inline const int    power(int, double);
};

#endif // CORRECTION_H

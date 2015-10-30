#ifndef EDGECANNY_H
#define EDGECANNY_H

#include "convolution.h"

class EdgeCanny : public Convolution
{
public:
    EdgeCanny(PNM*);
    EdgeCanny(PNM*, ImageViewer*);

    virtual PNM* transform();

    void checkHysterysis(int, int, int, int, int, int, PNM*);

private:
    void initMembers();

    short bucket[361];
    math::matrix<double> m_orientation,
                         m_magnitude;
};

#endif // EDGECANNY_H

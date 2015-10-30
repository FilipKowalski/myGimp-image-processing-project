#ifndef EDGE_LAPLACE_GAUSS_H
#define EDGE_LAPLACE_GAUSS_H

#include "convolution.h"

class EdgeLaplaceOfGauss : public Convolution
{
public:
    EdgeLaplaceOfGauss(PNM*);
    EdgeLaplaceOfGauss(PNM*, ImageViewer*);

    virtual math::matrix<double> getMask(int, Mode);

    static double getLoG(int, int, double);
    int getSize();

private:
    int size;
    double sigma;
};

#endif // EDGE_LAPLACE_GAUSS_H

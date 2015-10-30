#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "transformation.h"
#include "matrix.h"

class Convolution : public Transformation
{
public:
    Convolution(PNM*);
    Convolution(PNM*, ImageViewer*);

    virtual math::matrix<double> getMask(int, Mode);
    virtual PNM* convolute(math::matrix<double>, Mode);
    virtual PNM* transform();

protected:
    const math::matrix<double> join(math::matrix<double>, math::matrix<double>);
    const double sum(math::matrix<double>);
};

#endif // CONVOLUTION_H

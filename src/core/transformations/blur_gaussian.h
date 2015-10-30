#ifndef BLUR_GAUSSIAN_H
#define BLUR_GAUSSIAN_H

#include "convolution.h"

class BlurGaussian : public Convolution
{
public:
    BlurGaussian(PNM*);
    BlurGaussian(PNM*, ImageViewer*);

    virtual PNM* transform();
    virtual math::matrix<double> getMask(int, Mode);
    static double getGauss(int, int, double);

private:
    int     radius;
    double  sigma;
    int     size;
};

#endif // BLUR_GAUSSIAN_H

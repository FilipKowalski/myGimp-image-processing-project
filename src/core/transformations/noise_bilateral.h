#ifndef NOISE_BILATERAL_H
#define NOISE_BILATERAL_H

#include "convolution.h"

class NoiseBilateral : public Convolution
{
public:
    NoiseBilateral(PNM*);
    NoiseBilateral(PNM*, ImageViewer*);

    virtual PNM* transform();

private:
    int getNeighbours(int, int, Channel);

    static double g(int, int, int);
    static double r(int, int, int);
};

#endif // NOISE_MEDIAN_H

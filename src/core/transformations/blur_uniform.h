#ifndef BLUR_UNIFORM_H
#define BLUR_UNIFORM_H

#include "convolution.h"

class BlurUniform : public Convolution
{
public:
    BlurUniform(PNM*);
    BlurUniform(PNM*, ImageViewer*);

    virtual math::matrix<double> getMask(int, Mode);
};

#endif // BLUR_UNIFORM_H

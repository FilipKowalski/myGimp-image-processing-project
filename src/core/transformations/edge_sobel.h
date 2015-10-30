#ifndef EDGE_SOBEL_H
#define EDGE_SOBEL_H

#include "edge_gradient.h"

class EdgeSobel : public EdgeGradient
{
public:
    EdgeSobel(PNM*);
    EdgeSobel(PNM*, ImageViewer*);

    math::matrix<double>* rawVerticalDetection();
    math::matrix<double>* rawHorizontalDetection();

private:
    void prepareMatrices();

    math::matrix<double> raw_g_x,
                         raw_g_y;
};

#endif // EDGE_SOBEL_H

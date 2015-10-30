#include "edge_roberts.h"

EdgeRoberts::EdgeRoberts(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgeRoberts::EdgeRoberts(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgeRoberts::prepareMatrices()
{
    math::matrix<double> tempX(2, 2);
    math::matrix<double> tempY(2, 2);

    tempX(0, 0) = 1;
    tempX(0, 1) = 0;
    tempX(1, 0) = 0;
    tempX(1, 1) = -1;

    tempY(0, 0) = 0;
    tempY(0, 1) = 1;
    tempY(1, 0) = -1;
    tempY(1, 1) = 0;

    g_x = tempX;
    g_y = tempY;
}

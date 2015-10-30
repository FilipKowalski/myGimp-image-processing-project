#include "edge_sobel.h"

EdgeSobel::EdgeSobel(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

EdgeSobel::EdgeSobel(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

void EdgeSobel::prepareMatrices()
{
    math::matrix<double> tempX(3, 3);
    math::matrix<double> tempY(3, 3);

    tempX(0, 0) = -1;
    tempX(0, 2) = 1;
    tempX(1, 0) = -2;
    tempX(1, 2) = 2;
    tempX(2, 0) = -1;
    tempX(2, 2) = 1;

    for (int i = 0; i < 3; i++)
        tempX(i, 1) = 0;

    tempY(0, 0) = -1;
    tempY(0, 1) = -2;
    tempY(0, 2) = -1;
    tempY(2, 0) = 1;
    tempY(2, 1) = 2;
    tempY(2, 2) = 1;

    for (int i = 0; i < 3; i++)
        tempY(1, i) = 0;

    g_x = tempX;
    g_y = tempY;
}

math::matrix<double>* EdgeSobel::rawHorizontalDetection()
{
    math::matrix<double>* x_gradient = new math::matrix<double>(this->image->width(), this->image->height());

    int width = image->width();
    int height = image->height();

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            math::matrix<double> window = getWindow(i, j, 3, LChannel, RepeatEdge);
            (*x_gradient)(i, j) = sum(join(g_x, window));
        }

    return x_gradient;
}

math::matrix<double>* EdgeSobel::rawVerticalDetection()
{
    math::matrix<double>* y_gradient = new  math::matrix<double>(this->image->width(), this->image->height());

    int width = image->width();
    int height = image->height();

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            math::matrix<double> window = getWindow(i, j, 3, LChannel, RepeatEdge);
            (*y_gradient)(i, j) = sum(join(g_y, window));
        }


    return y_gradient;
}

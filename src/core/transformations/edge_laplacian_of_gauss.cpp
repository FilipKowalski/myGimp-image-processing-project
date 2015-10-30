#include "edge_laplacian_of_gauss.h"

#include "blur_gaussian.h"

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img) :
    Convolution(img)
{
}

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<double> EdgeLaplaceOfGauss::getMask(int, Mode)
{
    size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();

    math::matrix<double> mask(size, size);

    int half = size / 2;

    for (int i = -half; i <= half; i++) {
        for (int j = -half; j <= half; j++) {
            mask(i + half, j + half) = getLoG(i, j, sigma);
        }
    }

    return mask;
}

double EdgeLaplaceOfGauss::getLoG(int x, int y, double s)
{
    return ((pow(x,2)+pow(y,2))/pow(s,2) - 2/pow(s,2)) * BlurGaussian::getGauss(x, y, s);
}

int EdgeLaplaceOfGauss::getSize()
{
    return size;
}

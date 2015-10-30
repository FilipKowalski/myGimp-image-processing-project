#include "edge_laplacian.h"

EdgeLaplacian::EdgeLaplacian(PNM* img) :
    Convolution(img)
{
}

EdgeLaplacian::EdgeLaplacian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<double> EdgeLaplacian::getMask(int, Mode)
{
    int size = getParameter("size").toInt();
    math::matrix<double> mask(size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mask(i, j) = -1;
        }
    }

    int middle = floor(size/2);
    mask(middle, middle) = size * size - 1;

    return mask;
}


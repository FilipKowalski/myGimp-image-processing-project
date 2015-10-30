#include "edge_zero.h"

#include "edge_laplacian_of_gauss.h"

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img) :
    Convolution(img)
{
}

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeZeroCrossing::transform()
{
    int width = image->width(),
        height = image->height();

    int    size  = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    int    t     = getParameter("threshold").toInt();
    int    v_0   = 128;

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    EdgeLaplaceOfGauss* Edge = new EdgeLaplaceOfGauss(image);
    Edge->setParameter("size", size);
    Edge->setParameter("sigma", sigma);
    image = Edge->transform();

    math::matrix<double> window(size, size);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            window = getWindow(x, y, size, LChannel, RepeatEdge);

            double max   = PIXEL_VAL_MIN;
            double min   = PIXEL_VAL_MAX;

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {

                    if (window(i, j) > max)
                        max = window(i, j);
                    if (window(i, j) < min)
                        min = window(i, j);
                }
            }

            if (min < (v_0 - t) && max > (v_0 + t))
                newImage->setPixel(x, y, image->pixel(x, y));
            else
                newImage->setPixel(x, y, 0);
        }
    }

    delete Edge;

    return newImage;
}


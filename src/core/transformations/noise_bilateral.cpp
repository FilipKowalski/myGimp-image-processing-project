#include "noise_bilateral.h"

#define M_PI 3.14159265358979323846
#define M_E 2.71828182845904523536

NoiseBilateral::NoiseBilateral(PNM* img) :
    Convolution(img)
{
}

NoiseBilateral::NoiseBilateral(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseBilateral::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    if (image->format() == QImage::Format_Indexed8)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                newImage->setPixel(x, y, getNeighbours(x, y, LChannel));
            }
    }
    else //if (image->format() == QImage::Format_RGB32)
    {
        for(int x = 0; x < width; x++)
            for(int y = 0; y < height; y++)
            {
                newImage->setPixel(x, y, qRgb(getNeighbours(x, y, RChannel), getNeighbours(x, y, GChannel) , getNeighbours(x, y, BChannel)));
            }
    }

    return newImage;
}

int NoiseBilateral::getNeighbours(int x, int y, Channel channel)
{
    int sigma_d = getParameter("sigma_d").toInt();
    int sigma_r = getParameter("sigma_r").toInt();
    int radius = sigma_d;
    int windowSize = radius * 2 + 1;

    math::matrix<double> window = getWindow(x, y, windowSize, channel, RepeatEdge);

    int val = 0, k = 0;

    for(int x = 0; x < windowSize; x++)
        for(int y = 0; y < windowSize; y++)
        {
            int kernels = g(x / windowSize, y / windowSize, sigma_d) * r(x / windowSize, y / windowSize, sigma_r);
            val += window(x, y) * kernels;
            k += kernels;
        }

    return val/k;
}

double NoiseBilateral::g(int x, int y, int sigma_d)
{
    return pow(M_E, -((x*x) + (y*y)) / (2*sigma_d*sigma_d));
}

double NoiseBilateral::r(int v, int v_ij, int sigma_r)
{
    return pow(M_E, (- (v - v_ij)*(v - v_ij)) / (2*sigma_r*sigma_r));
}

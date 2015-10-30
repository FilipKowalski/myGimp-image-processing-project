#include "noise_median.h"

NoiseMedian::NoiseMedian(PNM* img) :
    Convolution(img)
{
}

NoiseMedian::NoiseMedian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseMedian::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    if (image->format() == QImage::Format_Indexed8)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                newImage->setPixel(x, y, getMedian(x, y, LChannel));
            }
    }
    else //if (image->format() == QImage::Format_RGB32)
    {
        for(int x = 0; x < width; x++)
            for(int y = 0; y < height; y++)
            {
                newImage->setPixel(x, y, qRgb(getMedian(x, y, RChannel),
                                              getMedian(x, y, GChannel),
                                              getMedian(x, y, BChannel)));
            }
    }
    return newImage;
}

int NoiseMedian::compare(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int NoiseMedian::getMedian(int x, int y, Channel channel)
{
    int radius = getParameter("radius").toInt();
    int size = radius * 2  + 1;
    double* tmp_tab = new double[size * size];
    math::matrix<double> window(size, size);

    if(channel == RChannel)
        window = getWindow(x, y, size, RChannel, RepeatEdge);
    else if(channel == GChannel)
        window = getWindow(x, y, size, GChannel, RepeatEdge);
    else if(channel == BChannel)
        window = getWindow(x, y, size, BChannel, RepeatEdge);
    else if(channel == LChannel)
        window = getWindow(x, y, size, LChannel, RepeatEdge);

    int a = 0;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            tmp_tab[a] = window(i, j);
            a++;
        }
    }
    std::sort(tmp_tab, tmp_tab + size * size);

    return (int) tmp_tab[(int)(floor(size*size)) / 2];
}

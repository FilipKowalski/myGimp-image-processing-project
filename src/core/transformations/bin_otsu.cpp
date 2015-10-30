#include "bin_otsu.h"

#include "histogram_equalization.h"
#include "../histogram.h"

BinarizationOtsu::BinarizationOtsu(PNM* img) :
    Transformation(img)
{
}

BinarizationOtsu::BinarizationOtsu(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationOtsu::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    HistogramEqualization* imageHistogram = new HistogramEqualization(image);

    image = imageHistogram->transform();
    QHash<int, int>* histogram = image->getHistogram()->get(Histogram::LChannel);

    double* bcv = new double[256];

    for(int T = 0; T < 256; T++)
    {
        int sum1 = 0;
        int sum2 = 0;

        for(int i = 0; i < T; i++)
            if (histogram->contains(i))
            {
                sum1 += histogram->value(i);
                sum2 += histogram->value(i) * i;
            }

        double W_b = sum1 / pow(255, 2);
        double u_b;

		if (sum1 != 0)
			u_b = sum2 / sum1;
		else
			u_b = 0;

        for(int i = T; i < 256; i++)
            if (histogram->contains(i))
            {
                sum1 += histogram->value(i);
                sum2 += histogram->value(i) * i;
            }

        double W_f = sum1 / pow(255, 2);
        double u_f = sum2 / sum1;

        bcv[T] = W_b * W_f * pow(u_b - u_f, 2);
    }

    int T = 0;

    for (int i = 1; i < 256; i++)
        if (bcv[T] < bcv[i])
            T = i;

    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
        {
            QRgb pixel = image->pixel(x,y);

            int val = qGray(pixel);

            if (val < T)
                newImage->setPixel(x, y, 0);
            else
                newImage->setPixel(x, y, 1);
        }

    return newImage;
}
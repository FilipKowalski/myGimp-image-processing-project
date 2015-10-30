#include "correction.h"

Correction::Correction(PNM* img) :
    Transformation(img)
{
}

Correction::Correction(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* Correction::transform()
{
    double shift  = getParameter("shift").toDouble();
    double factor = getParameter("factor").toDouble();
    double gamma  = getParameter("gamma").toDouble();

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    if (image->format() == QImage::Format_Indexed8)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y);

                int val = qGray(pixel);
                val = add(val, shift);
                val = mult(val, factor);
                val = power(val, gamma);

                newImage->setPixel(x,y, val);
            }
    }
    else //if (image->format() == QImage::Format_RGB32)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y);

                int r = qRed(pixel);
                int g = qGreen(pixel);
                int b = qBlue(pixel);

                r = add(r, shift);
                g = add(g, shift);
                b = add(b, shift);

                r = mult(r, factor);
                g = mult(g, factor);
                b = mult(b, factor);

                r = power(r, gamma);
                g = power(g, gamma);
                b = power(b, gamma);

                newImage->setPixel(x,y, qRgb(r,g,b));
            }
    }

    return newImage;
}

const int Correction::add(double value, double shift)
{
    int result = value + shift;

    if(result < PIXEL_VAL_MIN)
    {
        result = PIXEL_VAL_MIN;
    }
    else if(result > PIXEL_VAL_MAX)
    {
        result = PIXEL_VAL_MAX;
    }

    return result;
}

const  double Correction::mult(int value, double factor)
{
    double result = value * factor;

    if(result < PIXEL_VAL_MIN)
    {
        result = PIXEL_VAL_MIN;
    }
    else if(result > PIXEL_VAL_MAX)
    {
        result = PIXEL_VAL_MAX;
    }

    return result;
}

const int Correction::power(int value, double power)
{
    int result = std::pow(value, power);

    if(result < PIXEL_VAL_MIN)
    {
        result = PIXEL_VAL_MIN;
    }
    else if(result > PIXEL_VAL_MAX)
    {
        result = PIXEL_VAL_MAX;
    }

    return result;
}

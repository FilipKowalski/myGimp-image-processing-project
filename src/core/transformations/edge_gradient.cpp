#include "edge_gradient.h"

EdgeGradient::EdgeGradient(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

EdgeGradient::EdgeGradient(PNM* img) :
    Convolution(img)
{
}

PNM* EdgeGradient::verticalDetection()
{
    return convolute(g_y, RepeatEdge);
}

PNM* EdgeGradient::horizontalDetection()
{
    return convolute(g_x, RepeatEdge);
}

PNM* EdgeGradient::transform()
{
    PNM* newImage = new PNM(image->width(), image->height(), image->format());

    int width = image->width();
    int height = image->height();

    PNM* xImage = horizontalDetection();
    PNM* yImage = verticalDetection();

    if (image->format() == QImage::Format_Indexed8)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb x_pixel = xImage->pixel(x, y);
                QRgb y_pixel = yImage->pixel(x, y);

                int valX = qGray(x_pixel);
                int valY = qGray(y_pixel);

                newImage->setPixel(x, y, sqrt(valX*valX + valY*valY));
            }
    }
    else //if (image->format() == QImage::Format_RGB32)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb x_pixel = xImage->pixel(x, y);
                QRgb y_pixel = yImage->pixel(x, y);

                int redX = qRed(x_pixel);
                int redY = qRed(y_pixel);
                int greenX = qGreen(x_pixel);
                int greenY = qGreen(y_pixel);
                int blueX = qBlue(x_pixel);
                int blueY = qBlue(y_pixel);

                int newRed = sqrt(redX*redX + redY*redY);
                int newGreen = sqrt(greenX*greenX + greenY*greenY);
                int newBlue = sqrt(blueX*blueX + blueY*blueY);

                newImage->setPixel(x, y, qRgb(newRed, newGreen, newBlue));
            }
    }

    return newImage;
}


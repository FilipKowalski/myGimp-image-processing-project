#include "bin_niblack.h"

BinarizationNiblack::BinarizationNiblack(PNM* img) :
    Transformation(img)
{
}

BinarizationNiblack::BinarizationNiblack(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationNiblack::transform()
{
    int width = image->width();
    int height = image->height();

    int    r = getParameter("r").toInt();
    double a = getParameter("a").toDouble();
    double u = 0, sigma = 0, T = 0;

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++)
        {
            math::matrix<double> window = getWindow(x, y, r, LChannel, RepeatEdge);

            for (int i = 0; i < r; i++){
                for (int j = 0; j < r; j++){
                    u += (window(i, j));
                }
            }
            u = u / pow(r, 2);

            for (int i = 0; i < r; i++){
                for (int j = 0; j < r; j++){
                    sigma += (pow(window(i,j) - u, 2)/pow(r, 2));
                }
            }
            sigma = sqrt(sigma);

            T = u + a * sigma;

            QRgb pixel = image->pixel(x,y);

            int val = qGray(pixel);

            if (val < T)
                newImage->setPixel(x, y, 0);
            else
                newImage->setPixel(x, y, 1);

        }
    return newImage;
}

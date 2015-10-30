#include "map_horizon.h"

#include "map_height.h"

MapHorizon::MapHorizon(PNM* img) :
    Transformation(img)
{
}

MapHorizon::MapHorizon(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* MapHorizon::transform()
{
    int width  = image->width(),
        height = image->height();

    double scale     = getParameter("scale").toDouble();
    int    sun_alpha = getParameter("alpha").toInt();
    int dx, dy;

    switch (getParameter("direction").toInt())
    {
    case NORTH: dx = 0; dy = -1; break;
    case SOUTH: dx = 0; dy = 1; break;
    case EAST:  dx = 1; dy = 0; break;
    case WEST:  dx = -1; dy = 0; break;
    default:
        qWarning() << "Unknown direction!";
        dx =  0;
        dy = -1;
    }

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    MapHeight* mapHeight = new MapHeight(image);

    image = mapHeight->transform();

    delete mapHeight;

    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            double alpha = 0.0;
            int curr_h = qGray(image->pixel(i, j));

            int k = i + dx;
            int l = j + dy;

            while(k < width && l < height)
            {
                double ray_h = qGray(image->pixel(k,l));

                if (curr_h < ray_h)
                {
                    double dist = sqrt(pow(k - i, 2) + pow(l - j, 2)) * scale;
                    double ray_alpha = atan((ray_h-curr_h)/dist)*180./3.14;
                    if (ray_alpha > alpha)
                        alpha = ray_alpha;
                }

                ++k;
                ++l;
            }

            double delta = alpha - sun_alpha;

            if (delta <= 0)
                newImage->setPixel(i,j, PIXEL_VAL_MAX);
            else
                newImage->setPixel(i,j, cos(delta*3.14/180.)*255);
        }

    return newImage;
}

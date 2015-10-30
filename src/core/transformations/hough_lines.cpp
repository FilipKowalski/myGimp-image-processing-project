#include "hough_lines.h"

#include "bin_gradient.h"
#include "edge_laplacian.h"
#include "hough.h"

#include <QPainter>

HoughLines::HoughLines(PNM* img) :
    Transformation(img)
{
}

HoughLines::HoughLines(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HoughLines::transform()
{
    // Cut of value from the image;
    int  threshold      = getParameter("threshold").toInt();
    bool drawWholeLines = getParameter("draw_whole_lines").toBool();

    PNM* newImage = new PNM(image->copy());

    EdgeLaplacian* edgeLaplacian = new EdgeLaplacian(image);
    edgeLaplacian->setParameter("size", 3);
    image = edgeLaplacian->transform();
    delete edgeLaplacian;

    BinarizationGradient* binGradient = new BinarizationGradient(image);
    image = binGradient->transform();
    delete binGradient;

    Hough* hough = new Hough(image);
    hough->setParameter("theta_density" , 3);
    hough->setParameter("skip_edge_detection", true);
    image = hough->transform();
    delete hough;

    int width  = image->width(),
        height = image->height();

    for (int theta = 0; theta < width; theta++) {
        for (int rho = 0; rho < height; rho++) {
            QRgb pixel = image->pixel(theta, rho);
            if(qGray(pixel) > threshold)
                newImage->setPixel(theta, rho, 1);
       }
    }

    return newImage;
}

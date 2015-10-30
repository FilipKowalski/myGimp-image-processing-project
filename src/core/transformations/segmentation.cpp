#include "Segmentation.h"

Segmentation::Segmentation(PNM* img) :
    Transformation(img)
{
}

Segmentation::Segmentation(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* Segmentation::transform()
{
    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return 0;
}

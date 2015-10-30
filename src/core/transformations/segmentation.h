#ifndef SEGMENTATION_RECTANGLES_H
#define SEGMENTATION_RECTANGLES_H

#include "transformation.h"

class Segmentation : public Transformation
{
public:
    explicit Segmentation(PNM*);
    explicit Segmentation(PNM*, ImageViewer*);

    virtual PNM* transform();
};

#endif // SEGMENTATION_RECTANGLES_H

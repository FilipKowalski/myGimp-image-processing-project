#include "morph_erode.h"

MorphErode::MorphErode(PNM* img) :
    MorphologicalOperator(img)
{
}

MorphErode::MorphErode(PNM* img, ImageViewer* iv) :
    MorphologicalOperator(img, iv)
{
}

const int MorphErode::morph(math::matrix<double> window, math::matrix<bool> se)
{
    double max=0.0;

    for (int i = 0; i < window.RowNo(); i++) {
        for (int j = 0; j < window.RowNo(); j++) {
            if (se(i, j) == true && window(i, j) > max)
                max = window(i, j);
        }
    }

    return max;
}

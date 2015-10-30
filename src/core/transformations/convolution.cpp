#include "convolution.h"

/** Overloaded constructor */
Convolution::Convolution(PNM* img) :
    Transformation(img)
{
}

Convolution::Convolution(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

/** Returns a convoluted form of the image */
PNM* Convolution::transform()
{
    return convolute(getMask(3, Normalize), RepeatEdge);
}

/** Returns a sizeXsize matrix with the center point equal 1.0 */
math::matrix<double> Convolution::getMask(int size, Mode mode = Normalize)
{
    math::matrix<double> mask(size, size);

    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
        {
            mask(i, j) = 0;
        }

    int center = floor(size / 2);
    mask(center, center) = 1;

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<double> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    double weight = sum(mask);

    if (image->format() == QImage::Format_Indexed8)
    {
        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                math::matrix<double> window = getWindow(x, y, mask.ColNo(), LChannel, mode);
                math::matrix<double> akumulator = join(mask, window);

                double sumaAkumulatora = sum(akumulator);
                if(weight > 0)
                {
                    sumaAkumulatora /= weight;
                }

				if(sumaAkumulatora > PIXEL_VAL_MAX)
                {
					sumaAkumulatora = PIXEL_VAL_MAX;
                }
				else if(sumaAkumulatora < PIXEL_VAL_MIN)
                {
					sumaAkumulatora = PIXEL_VAL_MIN;
                }

                newImage->setPixel(x, y, sumaAkumulatora);
            }
    }
    else //if (image->format() == QImage::Format_RGB32)
    {
        for(int x = 0; x < width; x++)
            for(int y = 0; y < height; y++)
            {
                math::matrix<double> windowR = getWindow(x, y, mask.ColNo(), RChannel, mode);
                math::matrix<double> akumulatorR = join(mask, windowR);
                math::matrix<double> windowG = getWindow(x, y, mask.ColNo(), GChannel, mode);
                math::matrix<double> akumulatorG = join(mask, windowG);
                math::matrix<double> windowB = getWindow(x, y, mask.ColNo(), BChannel, mode);
                math::matrix<double> akumulatorB = join(mask, windowB);

                double sumaAkumulatoraR = sum(akumulatorR);
				double sumaAkumulatoraG = sum(akumulatorG);
                double sumaAkumulatoraB = sum(akumulatorB);

                if(weight > 0)
                {
                    sumaAkumulatoraR /= weight;
                    sumaAkumulatoraG /= weight;
                    sumaAkumulatoraB /= weight;
                }

				if(sumaAkumulatoraR > PIXEL_VAL_MAX)
                {
					sumaAkumulatoraR = PIXEL_VAL_MAX;
                }
				else if(sumaAkumulatoraR < PIXEL_VAL_MIN)
                {
					sumaAkumulatoraR = PIXEL_VAL_MIN;
                }
				if(sumaAkumulatoraG > PIXEL_VAL_MAX)
                {
					sumaAkumulatoraG = PIXEL_VAL_MAX;
                }
				else if(sumaAkumulatoraG < PIXEL_VAL_MIN)
                {
					sumaAkumulatoraG = PIXEL_VAL_MIN;
                }
				if(sumaAkumulatoraB > PIXEL_VAL_MAX)
                {
					sumaAkumulatoraB = PIXEL_VAL_MAX;
                }
				else if(sumaAkumulatoraB < PIXEL_VAL_MIN)
                {
					sumaAkumulatoraB = PIXEL_VAL_MIN;
                }

                newImage->setPixel(x, y, qRgb(sumaAkumulatoraR, sumaAkumulatoraG, sumaAkumulatoraB));
            }
    }

    return newImage;
}

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<double> Convolution::join(math::matrix<double> A, math::matrix<double> B)
{
    int size = A.RowNo();
    math::matrix<double> C(size, size);

    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
        {
            C(i, j) = A(i, j) * B(i, j);
        }

    return C;
}

/** Sums all of the matrixes elements */
const double Convolution::sum(const math::matrix<double> A)
{
    double sum = 0.0;

    int size = A.RowNo();

    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
        {
            sum += A(i, j);
        }

    return sum;

}

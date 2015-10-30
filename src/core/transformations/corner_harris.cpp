#include "corner_harris.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

CornerHarris::CornerHarris(PNM* img) :
    Convolution(img)
{
}

CornerHarris::CornerHarris(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* CornerHarris::transform()
{
    int    threshold    = getParameter("threshold").toInt();
    double sigma        = getParameter("sigma").toDouble(),
           sigma_weight = getParameter("sigma_weight").toDouble(),
           k_param      = getParameter("k").toDouble();

    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    math::matrix<double> I_x2 (width, height),
            I_y2(width, height),
            I_xy(width, height),
            corners_candidates(width, height),
            corners_nonmax_suppress(width, height);

    ConversionGrayscale* grayscale = new ConversionGrayscale(image);
    image = grayscale->transform();
    delete grayscale;

    BlurGaussian* gauss = new BlurGaussian(image);
    gauss->setParameter("size", 3);
    gauss->setParameter("sigma", 3.6);
    image = gauss->transform();
    delete gauss;

    EdgeSobel* sobel = new EdgeSobel(image);
    image = sobel->transform();
    math::matrix<double>* x_gradient = sobel->rawHorizontalDetection();
    math::matrix<double>* y_gradient = sobel->rawVerticalDetection();

    double gx_pix, gy_pix;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            gx_pix = (*x_gradient)(i, j);
            gy_pix = (*y_gradient)(i, j);

            I_x2(i, j) = gx_pix * gx_pix;
            I_y2(i, j) = gy_pix * gy_pix;
            I_xy(i, j) = gx_pix * gy_pix;
        }
    }

    double S_x2, S_y2, S_xy;
    double g_val;
    double r;
    math::matrix<double> H(2, 2);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if(i == 0 || j == 0 || i == width - 1 || j == height - 1) {
                corners_candidates(i, j) = 0;
                corners_nonmax_suppress(i, j) = 0;
            } else {
                S_x2 = 0;
                S_y2 = 0;
                S_xy = 0;
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        g_val = BlurGaussian::getGauss(k, l, sigma);
                        S_x2 += I_x2(i + k, j + l) * g_val;
                        S_y2 += I_y2(i + k, j + l) * g_val;
                        S_xy += I_xy(i + k, j + l) * g_val;
                    }
                }
                S_x2 = S_x2 / sigma_weight;
                S_y2 = S_y2 / sigma_weight;
                S_xy = S_xy / sigma_weight;
                H(0, 0) = S_x2;
                H(1, 1) = S_y2;
                H(0, 1) = S_xy;
                H(1, 0) = S_xy;
                r = H(0, 0) * H(1, 1) - H(0, 1) * H(1, 0) - k_param*pow(H(0, 0) + H(1, 1), 2);
                if (r > threshold)
                    corners_candidates(i, j) = r;
                else
                    corners_candidates(i, j) = 0;
            }
        }
    }

    bool make_nonmax_suppression = true;
    double val;
    while(make_nonmax_suppression) {
        make_nonmax_suppression = false;
        for (int i = 1; i < width - 1; i++) {
            for (int j = 1; j < height - 1; j++) {
                val = corners_candidates(i, j);
                if (val > corners_candidates(i - 1, j - 1) &&
                        val > corners_candidates(i - 1, j) &&
                        val > corners_candidates(i, j - 1) &&
                        val > corners_candidates(i - 1, j + 1) &&
                        val > corners_candidates(i + 1, j - 1) &&
                        val > corners_candidates(i + 1, j + 1) &&
                        val > corners_candidates(i + 1, j) &&
                        val > corners_candidates(i, j + 1)) {
                    corners_nonmax_suppress(i, j) = val;
                } else {
                    if (val > 0)
                        make_nonmax_suppression = true;

                    corners_nonmax_suppress(i, j) = 0;
                }
            }
        }
        corners_candidates = corners_nonmax_suppress;
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (corners_candidates(i, j) == 0)
                newImage->setPixel(i, j, 0);
            else
                newImage->setPixel(i, j, 1);
        }
    }

    return newImage;
}

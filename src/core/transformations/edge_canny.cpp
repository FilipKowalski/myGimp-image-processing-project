#include "edge_canny.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
    initMembers();
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
    initMembers();
}

void EdgeCanny::initMembers()
{
    m_orientation = math::matrix<double>(image->width(), image->height());
    m_magnitude   = math::matrix<double>(image->width(), image->height());

    for(int i = 0; i < 360; i++) {
        if ((0 <= i && i <= 22) || (158 <= i && i <= 202) || (338 <= i && i <= 360))
            bucket[i] = 0;
        else if ((23 <= i && i <= 67) || (203 <= i && i <= 247))
            bucket[i] = 1;
        else if ((68 <= i && i <= 112) || (248 <= i && i <=292))
            bucket[i] = 2;
        else if ((113 <= i && i <= 157) || (293 <= i && i <= 337))
            bucket[i] = 3;
    }
}



PNM* EdgeCanny::transform()
{
    int width  = image->width(),
        height = image->height();

    int upper_thresh = getParameter("upper_threshold").toInt(),
        lower_thresh = getParameter("lower_threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    ConversionGrayscale* grayscale = new ConversionGrayscale(image);
    image = grayscale->transform();
    delete grayscale;

    BlurGaussian* gaussianBlur = new BlurGaussian(image);
    gaussianBlur->setParameter("size", 3);
    gaussianBlur->setParameter("sigma", 1.6);
    image = gaussianBlur->transform();
    delete gaussianBlur;

    EdgeSobel* sobel = new EdgeSobel(image);
    math::matrix<double>* G_x = sobel->rawHorizontalDetection();
    math::matrix<double>* G_y = sobel->rawVerticalDetection();
    delete sobel;

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
        {
            m_magnitude(x,y) = sqrt((*G_x)(x,y)*(*G_x)(x,y) + (*G_y)(x,y)*(*G_y)(x,y));
            if ((*G_x)(x,y) != 0)
               m_orientation(x,y) = atan(1) * 180 / 3.14;
            else
               m_orientation(x,y) = 0;
        }

    list<QPoint> initialPoints;

    int theta, x, y, w, z;
    double m, neigh_1, neigh_2;
    for (int i = 1; i < width - 1; i++)
        for (int j = 1; j < height - 1; j++)
        {
            theta = m_orientation(i,j);
            m = m_magnitude(i,j);

            switch(bucket[theta]) {
            case 0: x=i-1; y=j; w=i+1; z=j; break;
            case 1: x=i-1; y=j-1; w=i+1; z=j+1; break;
            case 2: x=i; y=j-1; w=i; x=j+1; break;
            case 3: x=i-1; y=j+1; w=i+1; z=j-1; break;
            }

            neigh_1 = m_magnitude(x,y);
            neigh_2 = m_magnitude(w,z);

            if (m > neigh_1 && m > neigh_2 && m > upper_thresh)
            {
                newImage->setPixel(i,j, PIXEL_VAL_MAX);
                initialPoints.push_back(QPoint(i,j));
            }
            else
            {
                newImage->setPixel(i,j, PIXEL_VAL_MIN);
            }
        }

//    QPoint pointah;
//    while(!initialPoints.empty())
//    {
//        pointah = initialPoints.front();

//        int i = pointah.x();
//        int j = pointah.y();

//        initialPoints.pop_front();

//        theta = m_orientation(i,j);

//        int k = i;
//        int l = j;

//        while (newImage->pixel(k,l) != PIXEL_VAL_MAX && m_magnitude(k,l) >= lower_thresh
//               && m_orientation(k, l) == theta)
//        {
//            newImage->setPixel(k,l,PIXEL_VAL_MAX);

//            switch(bucket[theta]) {
//            case 0:
//                ++k; ++l;
//                break;
//            case 1:
//                ++k;
//                break;
//            case 2:
//                --k; --l;
//                break;
//            case 3:
//                ++l;
//                break;
//            }
//        }
//    }

    return newImage;
}

void EdgeCanny::checkHysterysis(int i, int j, int di, int dj, int direction, int lower_thresh, PNM* newImage)
{
    qDebug() << Q_FUNC_INFO << "Not implemented yet!";
}

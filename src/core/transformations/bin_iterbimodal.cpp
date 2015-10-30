#include "bin_iterbimodal.h"

BinarizationIterBimodal::BinarizationIterBimodal(PNM* img) :
    Transformation(img)
{
}

BinarizationIterBimodal::BinarizationIterBimodal(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationIterBimodal::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    int T = 256/2;
    int T_new = 0;

    while (true){
        int Tw = 0;
        int iw = 0;
        int Tb = 0;
        int ib = 0;

        for (int x=0; x<width; x++)
            for (int y=0; y<height; y++)
            {
                QRgb pixel = image->pixel(x,y);

                int val = qGray(pixel);

                if (val > T)
                {
                    Tw += val;
                    ++iw;
                }
                else
                {
                    Tb += val;
                    ++ib;
                }
            }
		
		if (iw == 0 && ib == 0) 
			T_new = 0;
		else if (iw == 0)
			T_new = Tb/ib;
		else if (ib == 0)
			T_new = Tw/iw;
		else
			T_new = ((Tw/iw) + (Tb/ib)) / 2;

        if(T != T_new)
            T = T_new;
        else
            break;
    }

    for (int x=0; x<width; x++)
        for (int y=0; y<height; y++)
        {
            QRgb pixel = image->pixel(x,y);

            int val = qGray(pixel);

            if (val < T)
                newImage->setPixel(x, y, 0);
            else
                newImage->setPixel(x, y, 1);
        }

    return newImage;
}




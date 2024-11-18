#ifndef IMG_H
#define IMG_H

class image {
    public:
        // image(const double ar, const int iw);
        image(const int iw, const int ih);
        double const aspect_ratio;
        int const image_width;
        int const image_height;

        int calculate_img_height() const;

};

#endif
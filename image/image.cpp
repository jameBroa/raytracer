#include "image.h"

image::image(double ar, int iw): aspect_ratio(ar), image_width(iw), image_height(calculate_img_height()) {};
image::image(int iw, int ih): image_width(iw), image_height(ih), aspect_ratio(iw/ih){};
int image::calculate_img_height() const {
    int temp = int(image_width / aspect_ratio);
    return (temp < 1) ? 1: temp;
}

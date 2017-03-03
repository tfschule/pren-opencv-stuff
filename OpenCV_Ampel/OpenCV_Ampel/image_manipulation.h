//
//  image_manipulation.hpp
//  OpenCV3_Example2
//
//  Created by Valentin Schuler on 28.10.16.
//  Copyright © 2016 Valentin Schuler. All rights reserved.
//

#ifndef image_manipulation_h
#define image_manipulation_h

#include <stdio.h>

cv::Mat create_red_hue_image(cv::Mat image);
cv::Mat create_green_hue_image(cv::Mat rgb_image);

cv::Mat create_contour_image(cv::Mat image);
cv::Mat create_line_image(cv::Mat image);
float get_image_angle(cv::Mat contour_image);

#endif /* image_manipulation_h */

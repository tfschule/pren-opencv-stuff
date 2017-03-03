//
//  image_manipulation.cpp
//  OpenCV3_Example2
//
//  Created by Valentin Schuler on 28.10.16.
//  Copyright © 2016 Valentin Schuler. All rights reserved.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

#include "image_manipulation.h"

using namespace std;

//
// Erstellt aus einem RGB Bild ein HSV rotgefiltertes Bild
//
cv::Mat create_red_hue_image(cv::Mat rgb_image) {
    
    cv::Mat blur_image = rgb_image.clone();
    cv::medianBlur(rgb_image, blur_image, 3);
    
    // Convert input image to HSV
    cv::Mat hsv_image, red_hue_image;
    cv::cvtColor(blur_image, hsv_image, cv::COLOR_BGR2HSV);
    
    // Threshold the HSV image, keep only the red pixels
    cv::Mat lower_red_hue_range, upper_red_hue_range;
    cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
    cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);
    
    // Combine the above two images
    cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
    cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);
    
    return red_hue_image;
}

//
// Erstellt aus einem RGB Bild ein HSV grüngefiltertes Bild
//
cv::Mat create_green_hue_image(cv::Mat rgb_image) {
    
    cv::Mat blur_image = rgb_image.clone();
    cv::medianBlur(rgb_image, blur_image, 3);
    
    // Convert input image to HSV
    cv::Mat hsv_image, green_hue_image;
    cv::cvtColor(blur_image, hsv_image, cv::COLOR_BGR2HSV);
    
    // Threshold the HSV image, keep only the green pixels
    cv::Mat green_hue_range;
    cv::inRange(hsv_image,  cv::Scalar( 90*(180.0/360),  70*(255.0/100),  30*(255.0/100)),
                            cv::Scalar(120*(180.0/360), 100*(255.0/100), 100*(255.0/100)), green_hue_image);
    
    cv::GaussianBlur(green_hue_image, green_hue_image, cv::Size(3, 3), 2, 2);
    
    return green_hue_image;
}



//
// Erstellt aus einem schwarz/weiss Bild ein Konturbild.
//
cv::Mat create_contour_image(cv::Mat image) {

    cv::RNG rng(12345);
    
    cv::Mat detected_lines_image, contour_image;
    
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
    
    /// find contours
    cv::findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    
    /// draw contours
    contour_image = cv::Mat::zeros(image.size(), CV_8UC3 );
    
    for( int i = 0; i< contours.size(); i++ ) {
        cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        cv::drawContours(contour_image, contours, i, color, 1, 8, hierarchy, 0, cv::Point() );
        cout << cv::contourArea(contours[0]) << endl;
        
        char str[200];
        sprintf(str,"%i", (int)cv::contourArea(contours[0]));
        putText(contour_image, str, cv::Point2f(100,100), cv::FONT_HERSHEY_PLAIN, 2,  cv::Scalar(0,0,255,255));
    }

    return contour_image;
}

//
// Errechnet aus einem Contour Bild den Drehwinkel des Bildes
//
float get_image_angle(cv::Mat contour_image) {
    
    int thresh = 100;
    
    cv::Mat canny_image;
    vector<cv::Vec2f> lines;
    
    // detect lines
    cv::HoughLines(contour_image, lines, 1, CV_PI/180, 150, 0, 0 );
    
    float angles[10];
    int j = 0;
    
    // calculate angle
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        
        if( theta>(2*CV_PI/360)*150 || theta<CV_PI/180*10)
        {
            angles[j++] = theta;
        }
    }
    
    int number_of_angles = j;
    float average_angle = 0;
    
    while(j>=0) {
        average_angle += angles[j--];
    }
    
    if (number_of_angles > 0) {
        average_angle = average_angle / number_of_angles;
    }
    
    return average_angle;
}

//
// Errechnet aus einem Contour Bild den Drehwinkel des Bildes
//
cv::Mat create_line_image(cv::Mat image) {
    
    int thresh = 100;
    cv::RNG rng(12345);
    
    cv::Mat temp_image;
    cv::Mat line_image(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));
    vector<cv::Vec2f> lines;
    
    // detect lines
    cv::Canny(image, temp_image, thresh, thresh*2, 3 );
    cv::HoughLines(temp_image, lines, 1, CV_PI/180, 150, 0, 0 );
    
    
    // calculate angle
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        
        if( theta>(2*CV_PI/360)*150 || theta<CV_PI/180*10)
        {
            cv::Scalar color = cv::Scalar(255,0,0);
            cv::line(line_image, pt1, pt2, color, 3, CV_8UC3);
        }
    }
    return line_image;
}



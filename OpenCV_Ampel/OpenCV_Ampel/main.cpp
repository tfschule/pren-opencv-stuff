#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "image_manipulation.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    cv::Mat imgOriginal, image;
    
    // Load an image
    imgOriginal = cv::imread("/Users/valentin/desktop/pren/ampel_gruen.jpg");
        
    cv::resize(imgOriginal, image , Size(600, 800));
        
    image = image( Rect(0,280,600,200) );
    
    Mat imgHSV, imgContour;

    imgHSV = create_green_hue_image(image);
    imgContour = create_contour_image(imgHSV);

    imshow("Contour Image", imgContour); //show the thresholded image
    
    cv::imwrite("/Users/valentin/desktop/pren/ampel_roi_g.jpg", image);
    cv::imwrite("/Users/valentin/desktop/pren/ampel_contour_g.jpg", imgContour);
        
    waitKey(0);
    return 0;
    
}

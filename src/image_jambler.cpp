#include <image_jambler.hpp>
#include <random>

void addNoise(cv::Mat &image, double factor) {
    std::random_device rd;

    std::mt19937 generator(rd());

    std::uniform_real_distribution<double> distribution(0.0, factor);

    std::cout << "Generating noise from 0.0 to " << factor << std::endl;

    for (int x = 0; x < image.rows; ++x) {
        for (int y = 0; y < image.cols; ++y) {
            cv::Vec3b px = image.at<cv::Vec3b>(x, y);
            px[0] = static_cast<uchar>(px[0]*(distribution(generator) + 1)) % 255; // Blue
            px[1] = static_cast<uchar>(px[1]*(distribution(generator) + 1)) % 255; // Green
            px[2] = static_cast<uchar>(px[2]*(distribution(generator) + 1)) % 255; // Red

            image.at<cv::Vec3b>(x, y) = px;
        }
    }
}

void blur(cv::Mat &image) {
    int gaussianKernelSize = 7; // positive and odd
    cv::GaussianBlur(image, image, cv::Size(gaussianKernelSize, gaussianKernelSize), 0);

    int medianKernelSize = 15; // Must be odd
    cv::medianBlur(image, image, medianKernelSize);
}

void colorShift(cv::Mat &image, double factor) {
    cv::Mat colorMatrix = (cv::Mat_<float>(3, 3) << 
                            1+factor, factor, factor,   // Red channel adjustment
                            factor, 1+factor, factor,   // Green channel adjustment
                            factor, factor, 1+factor);  // Blue channel adjustment
    
    cv::transform(image, image, colorMatrix);
}

void jambleImage(cv::Mat &image, double factor) {
    // Clone image to remove metadata
    cv::Mat clone = image.clone();

    colorShift(clone, factor);
    blur(clone);
    addNoise(clone, factor);

    image = clone;
}
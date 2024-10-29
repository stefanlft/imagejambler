#include <image_jambler.hpp>
#include <random>
#include <randomizer.hpp>

void addNoise(cv::Mat &image, double factor, Randomizer &rnd) {
    std::cout << "Generating noise from 0.0 to " << factor << std::endl;

    for (int x = 0; x < image.rows; ++x) {
        for (int y = 0; y < image.cols; ++y) {
            cv::Vec3b px = image.at<cv::Vec3b>(x, y);
            px[0] = static_cast<uchar>(px[0]*(rnd.getNextReal(factor) + 1)) % 255; // Blue
            px[1] = static_cast<uchar>(px[1]*(rnd.getNextReal(factor) + 1)) % 255; // Green
            px[2] = static_cast<uchar>(px[2]*(rnd.getNextReal(factor) + 1)) % 255; // Red

            image.at<cv::Vec3b>(x, y) = px;
        }
    }
}

void blur(cv::Mat &image, double factor, Randomizer &rnd) {
    int gaussianKernelSize = rnd.getNextInt(25*factor)/2*2+1; // positive and odd
    cv::GaussianBlur(image, image, cv::Size(gaussianKernelSize, gaussianKernelSize), 0);

    int medianKernelSize = rnd.getNextInt(25*factor)/2*2+1; // positive be odd
    cv::medianBlur(image, image, medianKernelSize);
}

void colorShift(cv::Mat &image, double factor, Randomizer &rnd) {

    double rf = rnd.getNextReal(factor/2.0);
    double gf = rnd.getNextReal(factor/2.0);
    double bf = rnd.getNextReal(factor/2.0);

    cv::Mat colorMatrix = (cv::Mat_<float>(3, 3) << 
                            1+rf, rf, rf,   // Red channel adjustment
                            gf, 1+gf, gf,   // Green channel adjustment
                            bf, bf, 1+bf);  // Blue channel adjustment
    
    cv::transform(image, image, colorMatrix);
}

void wbShift(cv::Mat &image, double factor, Randomizer &rnd) {
    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    // Adjust temperature
    // temp > 1.0 makes the image warmer (more red), temp < 1.0 makes it cooler (more blue)
    double temp = rnd.getNextReal(1.0-factor, 1.0+factor);
    channels[0] *= (1.0 / temp); // Blue channel
    channels[2] *= temp;         // Red channel

    // Adjust tint
    // tint > 0 adds red to green, tint < 0 adds blue to green
    double tint = rnd.getNextReal(-factor, factor);
    channels[1] += tint * channels[2]; // Green channel influenced by red
    channels[1] -= tint * channels[0]; // Green channel influenced by blue

    // Clip the values to ensure they remain valid
    for (auto& channel : channels) {
        cv::normalize(channel, channel, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    }

    // Merge the channels back
    cv::merge(channels, image);
}

void colorManipulation(cv::Mat &image, double factor, Randomizer &rnd) {
    colorShift(image, factor, rnd);
    wbShift(image, factor, rnd);
}

void jambleImage(cv::Mat &image, double factor) {
    Randomizer rnd;

    // Clone image to remove metadata
    cv::Mat clone = image.clone();

    colorManipulation(clone, factor, rnd);
    blur(clone, factor, rnd);
    addNoise(clone, factor, rnd);

    image = clone;
}
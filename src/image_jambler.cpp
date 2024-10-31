#include <image_jambler.hpp>
#include <random>
#include <randomizer.hpp>

void addNoise(cv::Mat &image, double factor, Randomizer &rnd) {
    std::cout << "Generating noise from 0.0 to " << factor << std::endl;

    for (int x = 0; x < image.rows; ++x) {
        for (int y = 0; y < image.cols; ++y) {
            cv::Vec3b px = image.at<cv::Vec3b>(x, y);
            px[0] = static_cast<uchar>(px[0]*(rnd.getNextReal(factor/3.0) + 0.75)) % 255; // Blue
            px[1] = static_cast<uchar>(px[1]*(rnd.getNextReal(factor/3.0) + 0.75)) % 255; // Green
            px[2] = static_cast<uchar>(px[2]*(rnd.getNextReal(factor/3.0) + 0.75)) % 255; // Red

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
    double temp = rnd.getNextReal(1.0-factor/2.0, 1.0+factor/2.0);
    channels[0] *= (1.0 / temp); // Blue channel
    channels[2] *= temp;         // Red channel

    // Adjust tint
    // tint > 0 adds red to green, tint < 0 adds blue to green
    double tint = rnd.getNextReal(-factor/2.0, factor/2.0);
    channels[1] += tint * channels[2]; // Green channel influenced by red
    channels[1] -= tint * channels[0]; // Green channel influenced by blue

    // Clip the values to ensure they remain valid
    for (auto& channel : channels) {
        cv::normalize(channel, channel, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    }

    // Merge the channels back
    cv::merge(channels, image);
}

void brightnessContrastShift(cv::Mat &image, double factor, Randomizer &rnd) {
    double alpha = rnd.getNextReal(1.0+factor);
    int beta = rnd.getNextInt(factor*50);

    image.convertTo(image, -1, alpha, beta);

    // gamma

    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    double gamma_ = 1.0-factor/2.0;
    for( int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
    cv::Mat res = image.clone();
    cv::LUT(image, lookUpTable, res);
    image = res;

}

void colorManipulation(cv::Mat &image, double factor, Randomizer &rnd) {
    colorShift(image, factor, rnd);
    wbShift(image, factor, rnd);
    brightnessContrastShift(image, factor, rnd);
}

void textureScrambling(cv::Mat &image, double factor, Randomizer &rnd) {
    int neighbourhoodSize = (factor * 10)/2*2 + 1;
    std::vector<std::vector<std::pair<int, int>>> offsets;
    rnd.random3D(offsets, image.rows, image.cols, -neighbourhoodSize/2, neighbourhoodSize/2);

    cv::Mat scrambled = image.clone();

    for (int x = 0; x < image.rows; ++x) {
        for (int y = 0; y < image.cols; ++y) {
            int xOffset = offsets[x][y].first;
            int yOffset = offsets[x][y].second;

            if (xOffset != 0 or yOffset != 0) {
                int newX = x + xOffset >= 0 && x + xOffset < image.rows ? x + xOffset : x;
                int newY = y + yOffset >= 0 && y + yOffset < image.cols ? y + yOffset : y;

                scrambled.at<cv::Vec3b>(newX, newY) = image.at<cv::Vec3b>(x, y);
            }
        }
    }

    image = scrambled;
}

void histogramEqualization(cv::Mat &image, double factor, Randomizer &rnd) {

    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    for (auto& channel : channels) {
        // cv::normalize(channel, channel, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        cv::equalizeHist(channel, channel);
    }

    // Merge the channels back
    cv::merge(channels, image);

}

void cropAndRotate(cv::Mat &image, double factor, Randomizer &rnd) {
    // rotate
    
    int sign = rnd.getNextInt(-1, 1);
    if (sign == 0) sign = -1;

    int angle = std::round(15.0 * rnd.getNextReal(factor*2.0)) * sign;
    cv::Point2f center(image.cols/2, image.rows/2); // center of rotation
    cv::Mat rotation_matrix = getRotationMatrix2D(center, angle, 1.0);
    warpAffine(image, image, rotation_matrix, image.size());

    // crop
    int cropFactor = 200;
    image = image(cv::Range(rnd.getNextInt(factor*cropFactor), image.rows-rnd.getNextInt(factor*cropFactor)),
                  cv::Range(rnd.getNextInt(factor*cropFactor), image.cols-rnd.getNextInt(factor*cropFactor)));
}

void jambleImage(cv::Mat &image, double factor) {
    Randomizer rnd;

    // Clone image to remove metadata
    cv::Mat clone = image.clone();

    textureScrambling(clone, factor, rnd);
    histogramEqualization(clone, factor, rnd);
    colorManipulation(clone, factor, rnd);
    blur(clone, factor, rnd);
    addNoise(clone, factor, rnd);

    cropAndRotate(clone, factor, rnd);

    image = clone;
}
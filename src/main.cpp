#include <iostream>
#include <opencv2/opencv.hpp>
#include <image_jambler.hpp>

cv::Mat resizeForScreen(cv::Mat &image, int maxWidth, int maxHeight) {
    // Calculate the scaling factor to fit the image within the screen
    double scaleX = static_cast<double>(maxWidth) / image.cols;
    double scaleY = static_cast<double>(maxHeight) / image.rows;
    double scale = std::min(scaleX, scaleY); // Choose the smaller scale factor to fit within both dimensions

    // Resize the image if it exceeds the screen limits
    cv::Mat resizedImage;
    if (scale < 1) {
        cv::resize(image, resizedImage, cv::Size(), scale, scale);
    } else {
        resizedImage = image; // No resizing if image is smaller than max dimensions
    }

    return resizedImage;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path> <factor: default 30>" << std::endl;
        return 1;
    }

    // Get noise grade
    double factor = 0.3;
    if (argc >= 3) {
        factor = (std::stoi(std::string(argv[2]))) / 100.0;
    }

    // Read image data
    std::string imagePath = argv[1];
    cv::Mat image = cv::imread(imagePath);
    cv::Mat original = image.clone();

    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image!" << std::endl;
        return 1;
    }

    jambleImage(image, factor);

    // Display the resized (or original) image
    cv::imshow("Jumbled  Image (Fitted to Screen)", resizeForScreen(image, 800, 600));
    cv::imshow("Original Image (Fitted to Screen)", resizeForScreen(original, 800, 600));
    while (cv::waitKey(0) != 'q');
    return 0;
}

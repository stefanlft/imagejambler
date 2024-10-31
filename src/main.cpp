#include <iostream>
#include <opencv2/opencv.hpp>
#include <randomizer.hpp>
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

std::string changeExtension(const std::string& filePath, const std::string& newExtension) {
    // Find the last dot in the filename
    size_t dotIndex = filePath.find_last_of(".");
    if (dotIndex != std::string::npos) {
        // Return the file path with the new extension
        return filePath.substr(0, dotIndex) + newExtension;
    }
    // If no dot is found, append the new extension
    return filePath + newExtension;
}


int main(int argc, char** argv) {
    const cv::String keys =
        "{help h usage ? |      | print this message   }"
        "{@image         |      | image to be jambled  (required) }"
        "{factor         |  30  | factor of protection (optional) }"
        ;
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("ImageJambler v1.0.0");
    if (parser.has("help")) {
        parser.printMessage();
        return 0;
    }

    if (!parser.has("@image")) {
        std::cerr << "Error: Required argument '@image' is missing." << std::endl;
        std::cerr << "Run with --help." << std::endl;
        return 1;
    }

    if (!parser.check()) {
        parser.printErrors();
        return 0;
    }

    // Get protec   tion grade
    double factor = parser.get<int>("factor")/100.0;

    // Read image data
    std::string imagePath = parser.get<std::string>("@image");
    cv::Mat image = cv::imread(imagePath);
    cv::Mat original = image.clone();

    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image!" << std::endl;
        return 1;
    }


    Randomizer rnd;

    jambleImage(image, factor);

    // Display the resized (or original) image
    cv::imshow("Jumbled  Image (Fitted to Screen)", resizeForScreen(image, 800, 600));
    cv::imshow("Original Image (Fitted to Screen)", resizeForScreen(original, 800, 600));
    while (int key = cv::waitKey(0)) {
        if (key == 'q') {
            break;
        } else if (key == 's') {
            cv::imwrite(changeExtension("jambled_"+imagePath,".jpg"), image, {
                cv::IMWRITE_JPEG_QUALITY, rnd.getNextInt(100),
                cv::IMWRITE_JPEG_OPTIMIZE, rnd.getNextInt(1),
                cv::IMWRITE_JPEG_PROGRESSIVE, rnd.getNextInt(1)
            });
        }
    }

    cv::destroyAllWindows();
    return 0;
}

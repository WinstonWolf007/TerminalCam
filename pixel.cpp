#include <opencv2/opencv.hpp>
#include <string>
#include <algorithm>
#include <cmath>


std::string color(int red, int green, int blue, std::string text) {
    std::string RED = std::to_string(red);
    std::string GREEN = std::to_string(green);
    std::string BLUE = std::to_string(blue);

    std::string start = "\033[48;2;" + RED + ";" + GREEN + ";" + BLUE + "m";
    std::string end = "\033[0m";
    
    return start + text + end;
}


int main() {
    cv::VideoCapture capture(0); // Ouvrir la caméra par défaut (index 0)

    if (!capture.isOpened()) {
        std::cerr << "Erreur : Impossible d'ouvrir la caméra." << std::endl;
        return 1;
    }

    cv::Mat frameInit;

    int pixelSize = 4;

    bool running = true;

    while (running) {
        capture.read(frameInit);

        if (frameInit.empty()) {
            std::cerr << "Erreur : Image vide reçue depuis la caméra." << std::endl;
            break;
        }

        cv::Mat frame;
        cv::flip(frameInit, frame, 1);

        // Pixelisation de l'image en remplaçant chaque bloc par sa couleur moyenne
        std::string col_string = "";

        for (int y = 0; y < frame.rows; y += pixelSize) {

            std::string row_string = "";

            for (int x = 0; x < frame.cols; x += pixelSize) {
                cv::Rect roi(x, y, pixelSize, pixelSize);
                cv::Scalar blockColor = cv::mean(frame(roi));
                frame(roi) = blockColor;

                int RED = blockColor[0];
                int GREEN = blockColor[1];
                int BLUE = blockColor[2];
            
                std::string colorPX = color(BLUE, GREEN, RED, " ");

                // update row
                row_string += colorPX;
                row_string += colorPX;
                // row_string += " ";
            }

            col_string += row_string + "\n";
        }

        std::cout << "\n\n\n\n" << col_string << "\n\n\n\n" << std::endl;

        // cv::imshow("Pixelized Image", frame);

        if (cv::waitKey(100) == 27) { // Sortir de la boucle si la touche Esc est pressée
            break;
        }
    }

    capture.release();
    cv::destroyAllWindows();

    return 0;
}

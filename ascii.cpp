#include <opencv2/opencv.hpp>
#include <string>
#include <algorithm>
#include <cmath>


int main() {
    cv::VideoCapture capture(0); // Ouvrir la caméra par défaut (index 0)

    if (!capture.isOpened()) {
        std::cerr << "Erreur : Impossible d'ouvrir la caméra." << std::endl;
        return 1;
    }

    std::string gray_chars = "@%$#=-+*&|/}{][)('!?:;,. ";

    cv::Mat frameInit;
    // cv::namedWindow("Pixelized Image", cv::WINDOW_NORMAL);

    int pixelSize = 2;

    while (true) {
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

                // get gray RGB
                float gray_value_float = 0.2989 * RED + 0.5870 * GREEN + 0.1140 * BLUE;
                int gray_value_int = gray_value_float;

                // get percent
                float gray_percent_float = gray_value_int * 100 / 255;
                int gray_percent_int = gray_percent_float;

                // get index of gray
                float gray_char_index_float = gray_percent_float * (gray_chars.size() - 1) / 100;
                int gray_char_index_int = static_cast<int>(std::round(gray_char_index_float));
                char gray_char = gray_chars[gray_char_index_int];

                // update row
                row_string += gray_char;
                row_string += "  ";
            }

            col_string += row_string + "\n";
        }

        std::cout << col_string << std::endl;

        // cv::imshow("Pixelized Image", frame);

        if (cv::waitKey(10) == 27) { // Sortir de la boucle si la touche Esc est pressée
            break;
        }
    }

    capture.release();
    cv::destroyAllWindows();

    return 0;
}

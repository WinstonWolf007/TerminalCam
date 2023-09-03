#include <iostream>
#include <string>


std::string color(int red, int green, int blue, std::string text) {
    std::string RED = std::to_string(red);
    std::string GREEN = std::to_string(green);
    std::string BLUE = std::to_string(blue);

    std::string start = "\033[48;2;" + RED + ";" + GREEN + ";" + BLUE + "m";
    std::string end = "\033[0m";
    
    return start + text + end;
}

int main() {
    std::cout << color(255, 255, 0, "Saut tout le monde") << std::endl;
    return 0;
}

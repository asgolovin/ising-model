#include "userInput.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "userInput.h"

void userInput(std::string filename, int &size, double &J, double &B, double &T) {
  std::vector<std::string> msg;
  std::ifstream file(filename);
  std::string input;
  // check for file availability and process it line by line
  if (file) {
    // loop over all lines in the file
    std::string lineStr;
    while (getline(file, lineStr, '@')) {
      msg.emplace_back(lineStr);
    }
    // intro
    std::cout << msg[0];
    // ask about size
    std::cout << msg[1];
    getline(std::cin, input);
    std::stringstream(input) >> size;
    while (size > 200 || size < 10){
        std::cout << "Wrong input! Enter a number between 10 and 200:\n";
        getline(std::cin, input);
        std::stringstream(input) >> size;
    }
    // ask about J
    std::cout << msg[2];
    getline(std::cin, input);
    std::stringstream(input) >> J;
    while (J > 1 || J < -1 || input.find_first_of("0123456789") == std::string::npos){
        std::cout << "Wrong input! Enter a number between -1 and 1:\n";
        getline(std::cin, input);
        std::stringstream(input) >> J;
    }
    // ask about B
    std::cout << msg[3];
    getline(std::cin, input);
    std::stringstream(input) >> B;
    while (B > 10 || B < -10 || input.find_first_of("0123456789") == std::string::npos){
        std::cout << "Wrong input! Enter a number between -10 and 10:\n";
        getline(std::cin, input);
        std::stringstream(input) >> B;
    }
    // ask about T
    std::cout << msg[4];
    getline(std::cin, input);
    std::stringstream(input) >> T;
    while (T <= 0){
        std::cout << "Wrong input! Enter a number above 0:\n";
        getline(std::cin, input);
        std::stringstream(input) >> T;
    }
    // final
    std::cout << msg[5];
  }
  else {
      std::cout << "Couldn't open input file\n";
  }
}
      
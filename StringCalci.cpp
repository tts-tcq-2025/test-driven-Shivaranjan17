#include "StringCalculator.h"
#include <stdexcept>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>
#include <numeric>

int StringCalculator::Add(const std::string& input) {
    if (input.empty()) return 0;

    std::string delimiter = detectDelimiter(input);
    std::string body = stripHeader(input);
    std::string cleanNumbers = unifyDelimiters(body, delimiter);

    std::vector<int> values = toNumbers(cleanNumbers);

    ensureNoNegatives(values);

    return sumUp(values);
}

std::string StringCalculator::detectDelimiter(const std::string& text) {
    if (text.rfind("//", 0) != 0) return ",";  // default delimiter

    auto pos = text.find('\n');
    if (pos == std::string::npos) return ",";

    std::string header = text.substr(2, pos - 2);
    if (header.size() >= 2 && header.front() == '[' && header.back() == ']') {
        return header.substr(1, header.size() - 2);
    }
    return header;
}

std::string StringCalculator::stripHeader(const std::string& text) {
    if (text.rfind("//", 0) != 0) return text;

    auto pos = text.find('\n');
    if (pos == std::string::npos || pos + 1 >= text.size()) return text;

    return text.substr(pos + 1);
}

std::string StringCalculator::unifyDelimiters(const std::string& text,
                                              const std::string& delimiter) {
    std::string normalized = text;

    if (delimiter != ",") {
        std::string esc = std::regex_replace(delimiter,
                                             std::regex(R"([-[\]{}()*+?.,\^$|#\s])"),
                                             R"(\$&)");
        normalized = std::regex_replace(normalized, std::regex(esc), ",");
    }

    std::replace(normalized.begin(), normalized.end(), '\n', ',');
    return normalized;
}

std::vector<int> StringCalculator::toNumbers(const std::string& text) {
    std::vector<int> nums;
    std::stringstream ss(text);
    std::string token;

    while (std::getline(ss, token, ',')) {
        if (!token.empty()) nums.push_back(std::stoi(token));
    }
    return nums;
}

int StringCalculator::sumUp(const std::vector<int>& nums) {
    return std::accumulate(nums.begin(), nums.end(), 0,
                           [](int acc, int val) {
                               return val <= 1000 ? acc + val : acc;
                           });
}

void StringCalculator::ensureNoNegatives(const std::vector<int>& nums) {
    std::vector<int> negatives;
    for (int n : nums) {
        if (n < 0) negatives.push_back(n);
    }

    if (!negatives.empty()) {
        std::ostringstream msg;
        msg << "negatives not allowed: ";
        for (size_t i = 0; i < negatives.size(); ++i) {
            if (i > 0) msg << ", ";
            msg << negatives[i];
        }
        throw std::invalid_argument(msg.str());
    }
}

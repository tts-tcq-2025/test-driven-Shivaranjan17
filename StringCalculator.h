#pragma once
#include <string>
#include <vector>

class StringCalculator {
public:
    // Main function to add numbers from a string
    static int Add(const std::string& input);

private:
    // Helper functions
    static std::string detectDelimiter(const std::string& text);
    static std::string stripHeader(const std::string& text);
    static std::string unifyDelimiters(const std::string& text, const std::string& delimiter);

    static std::vector<int> toNumbers(const std::string& text);
    static int sumUp(const std::vector<int>& nums);
    static void ensureNoNegatives(const std::vector<int>& nums);
};

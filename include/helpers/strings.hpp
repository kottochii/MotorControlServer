#pragma once
#include <string_view>
#include <vector>
#include <algorithm>


namespace secsys_server
{
    namespace string_helpers
    {
        template<typename T>
        std::vector<std::basic_string_view<T>> tokenize(const std::basic_string_view<T>& original, const std::basic_string_view<T>& delimiters) {
            std::vector<std::basic_string_view<T>> tokens;
            // Start iterating at the beginning of the original string
            auto start = original.begin();
            while (start != original.end()) {
                // Find the first occurrence of any delimiter character
                auto delim_pos = std::find_first_of(start, original.end(), delimiters.begin(), delimiters.end());
                // If there are no delimiters remaining, then the rest of the string is a token
                if (delim_pos == original.end()) {
                    tokens.push_back(std::basic_string_view<T>(start, original.end() - start));
                    break;
                }
                // Add the token before the delimiter to the list
                tokens.push_back(std::basic_string_view<T>(start, delim_pos - start));
                // Update start to be the character after the delimiter
                start = delim_pos + 1;
            }
        return tokens;
        }

        std::string generate_random_string(int n);
    }
}
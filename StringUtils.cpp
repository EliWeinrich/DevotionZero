#include "StringUtils.h"

#include <algorithm>
#include <cctype>

std::string toLower(std::string text)
{
    std::transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(std::tolower(c));
        }
    );

    return text;
}
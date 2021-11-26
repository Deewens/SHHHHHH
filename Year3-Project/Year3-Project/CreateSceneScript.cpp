#include <iostream>
#include <iomanip>
#include <fstream>

#include "json.hpp"

using json = nlohmann::json;

int main()
{
    json cells;

    for (int i = 0; i < 300; i++)
    {
        json cell = {
                {"spriteName", "sand"},
                {"gridIndex", i},
                {"rotation", 0.0},
                {"impassable", false},
        };
        cells.push_back(cell);
    }

    json level = {{"scene", cells}};

    std::ofstream o("scene_test.json");
    o << std::setw(4) << level << std::endl;
    return 0;
}
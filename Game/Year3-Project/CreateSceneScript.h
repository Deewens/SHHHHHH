#include <iostream>
#include <iomanip>
#include <fstream>

#include "json.hpp"

using json = nlohmann::json;

void createSceneScript()
{
    json cells;
    // 0 to 19: top side
    // 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280: left side
    // 280 to 299: bottom side
    // 19, 39, 59, 79, 99, 119, 139, 159, 179, 199, 219, 239, 259, 279, 299: right side

    std::vector<int> topCells;
    std::vector<int> leftCells = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260};
    std::vector<int> bottomCells;
    std::vector<int> rightCells = {39, 59, 79, 99, 119, 139, 159, 179, 199, 219, 239, 259, 279};

    for (int i = 1; i < 19; i++)
    {
        topCells.push_back(i);
    }

    for (int i = 281; i < 299; i++)
    {
        bottomCells.push_back(i);
    }


    for (int i = 0; i < 300; i++)
    {
        std::string spriteName = "sand";
        if (std::find(topCells.begin(), topCells.end(), i) != topCells.end())
        {
            spriteName = "beach_tm_01";
        }

        if (std::find(bottomCells.begin(), bottomCells.end(), i) != bottomCells.end())
        {
            spriteName = "beach_bm_01";
        }

        if (std::find(leftCells.begin(), leftCells.end(), i) != leftCells.end())
        {
            spriteName = "beach_lm_01";
        }

        if (std::find(rightCells.begin(), rightCells.end(), i) != rightCells.end())
        {
            spriteName = "beach_rm_01";
        }

        if (i == 0)
        {
            spriteName = "beach_tl";
        }

        if (i == 280)
        {
            spriteName = "beach_bl";
        }

        if (i == 19)
        {
            spriteName = "beach_tr";
        }

        if (i == 299)
        {
            spriteName = "beach_br";
        }
        json cell = {
                {"spriteName", spriteName},
                {"gridIndex", i},
                {"rotation", 0.0},
                {"impassable", false},
        };
        cells.push_back(cell);
    }

    json level = {{"scene", cells}};

    std::ofstream o("scene.json");
    o << std::setw(4) << level << std::endl;
}
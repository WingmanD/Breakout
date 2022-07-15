#pragma once
#include <filesystem>

#include "BoxCollisionComponent.hpp"
#include "BreakoutBall.hpp"
#include "BreakoutBrick.hpp"
#include "Paddle.hpp"
#include "Player.hpp"
#include "SoundCue.hpp"
#include "SphereCollisionComponent.hpp"
#include "Util.hpp"
#include "tinyxml/tinyxml2.h"

struct BreakoutLevelInfo {
    int rowCount = 0;
    int columnCount = 0;
    int rowSpacing = 0;
    int columnSpacing = 0;
    std::filesystem::path backgroundImagePath;

    struct BrickTypeInfo {
        std::string name;
        std::filesystem::path texturePath;
        int hitPoints = 0;
        std::filesystem::path hitSoundPath;
        std::filesystem::path breakSoundPath;
        int breakScore = 0;
    };

    std::map<std::string, BrickTypeInfo*> brickTypeMap;

    std::vector<std::vector<BrickTypeInfo*>> bricks;

    BreakoutLevelInfo(const std::filesystem::path& path) {
        if (!exists(path) || !is_regular_file(path))
            throw std::runtime_error("BreakoutLevelInfo: " + path.string() + " is not a file");

        tinyxml2::XMLDocument doc;
        doc.LoadFile(path.string().c_str());
        if (doc.Error())
            throw std::runtime_error("BreakoutLevelInfo: " + path.string() + " is not a valid XML file");


        auto levelElement = doc.FirstChildElement("Level");
        if (!levelElement) throw std::runtime_error("BreakoutLevelInfo: " + path.string() + " is not a valid XML file");

        rowCount = levelElement->IntAttribute("RowCount");
        columnCount = levelElement->IntAttribute("ColumnCount");
        rowSpacing = levelElement->IntAttribute("RowSpacing");
        columnSpacing = levelElement->IntAttribute("ColumnSpacing");
        backgroundImagePath = getAttribute(levelElement, "BackgroundTexture");

        auto brickTypesElement = levelElement->FirstChildElement("BrickTypes");
        if (!brickTypesElement)
            throw std::runtime_error(
                "BreakoutLevelInfo: " + path.string() + " is not a valid XML file");

        for (auto brickType = brickTypesElement->FirstChildElement("BrickType"); brickType; brickType = brickType->
             NextSiblingElement("BrickType")) {
            BrickTypeInfo* brickTypeInfo = new BrickTypeInfo();

            brickTypeInfo->name = getAttribute(brickType, "Id");
            brickTypeInfo->texturePath = getAttribute(brickType, "Texture");

            auto hitPointsAttr = getAttribute(brickType, "HitPoints");

            if (hitPointsAttr == "Infinite") brickTypeInfo->hitPoints = std::numeric_limits<int>::max();
            else brickTypeInfo->hitPoints = std::stoi(hitPointsAttr);

            brickTypeInfo->hitSoundPath = getAttribute(brickType, "HitSound");
            brickTypeInfo->breakSoundPath = getAttribute(brickType, "BreakSound");
            brickTypeInfo->breakScore = brickType->IntAttribute("BreakScore");

            brickTypeMap[brickTypeInfo->name] = brickTypeInfo;
        }


        bricks.resize(rowCount, std::vector<BrickTypeInfo*>(columnCount));
        auto bricksElement = levelElement->FirstChildElement("Bricks");
        std::string bricksText = bricksElement->GetText();
        std::vector<std::string> lines = Util::split(bricksText, '\n');

        int row = 0;
        for (auto line : lines) {
            line = Util::trim(line);
            if (line.empty()) continue;

            std::vector<std::string> brickNames = Util::split(line, ' ');
            if (brickNames.size() != static_cast<unsigned long long>(columnCount))
                throw std::runtime_error("BreakoutLevelInfo: " + path.string() + " is not a valid XML file");


            for (auto column = 0; column < columnCount; column++) {
                if (brickNames[column] == "_") bricks[row][column] = nullptr;
                else {
                    auto brickType = brickTypeMap.find(brickNames[column]);
                    if (brickType == brickTypeMap.end())
                        throw std::runtime_error("BreakoutLevelInfo: Brick type " + brickNames[column] + " not found");
                    bricks[row][column] = brickType->second;
                }
            }

            row++;

            if (row >= rowCount) break;
        }

    }

private:
    std::string getAttribute(tinyxml2::XMLElement* element, const char* name) {
        auto attribute = element->Attribute(name);
        if (!attribute)
            return "";

        return attribute;
    }

};

class BreakoutBoard : public Player {
    std::map<std::string, StaticMesh*> brickNameSMeshMap;

    std::vector<std::vector<BreakoutBrick*>> bricks;
    std::vector<BreakoutBall*> balls;
    Paddle* paddle = nullptr;

    StaticMesh* ballMesh = nullptr;

    BoxCollisionComponent* wallTop = nullptr;
    BoxCollisionComponent* wallLeft = nullptr;
    BoxCollisionComponent* wallRight = nullptr;
    BoxCollisionComponent* killVolume = nullptr;


    int rowCount = 0;
    int columnCount = 0;

    float cellWidth = 0;
    float cellHeight = 0;

    float boardWidth = 0;
    float totalBoardHeight = 0;


    bool bDisabled = true;

    float ballSpeed = 3.0f;


public:
    BreakoutBoard(Engine* owningEngine, const BreakoutLevelInfo& level);

    void tick(double deltaTime) override;

    void onKey(int key, int scancode, int action, int mods) override;

    void onMouseMove(double xpos, double ypos) override;

private:
    void spawnBall();
    void start();
    void applyBallMovement(float deltaTime);
    void cellIndicesFromBallLocation(BreakoutBall* ball, int& row, int& column) const;
    void checkCollision(BreakoutBall* ball);
    void checkCollision(BreakoutBall* ball, int row, int column);
    [[nodiscard]] glm::vec2 getCellCenter(int row, int column) const;


};

#pragma once
#include <filesystem>

#include "Player.hpp"
#include "SoundCue.hpp"
#include "StaticMeshComponent.hpp"
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
            if (brickNames.size() != columnCount)
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

struct Brick {
    int HP = 0;
    BreakoutLevelInfo::BrickTypeInfo* info = nullptr;

    SoundCue* hitSound = nullptr;
    SoundCue* breakSound = nullptr;

    StaticMeshComponent* meshComp = nullptr;

    Brick(BreakoutLevelInfo::BrickTypeInfo* info): info(info) { HP = info->hitPoints; }

    void hit() {
        if (hitSound) hitSound->play(0);

        if (HP != std::numeric_limits<int>::max()) {
            HP--;
            if (HP <= 0) { if (breakSound) breakSound->play(0); }
        }
    }
};

struct Ball {
    StaticMeshComponent* meshComp = nullptr;
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    float radius = 0;

    explicit Ball(StaticMeshComponent* const mesh_comp)
        : meshComp(mesh_comp) {
        auto AABB = mesh_comp->getMesh()->getBoundingBox();

        radius = (AABB.max.x - AABB.min.x) / 2.0f;
    }
};

class BreakoutBoard : public Player {
    std::map<std::string, StaticMesh*> brickNameSMeshMap;
    std::vector<std::vector<Brick*>> bricks;

    StaticMeshComponent* player = nullptr;

    StaticMesh* ballMesh = nullptr;

    int rowCount = 0;
    int columnCount = 0;

    float cellWidth = 0;
    float cellHeight = 0;

    float brickWidth = 0;
    float brickHeight = 0;

    float boardWidth = 0;
    std::vector<Ball*> balls;

    bool bDisabled = true;

    float ballSpeed = 1.0f;
    float totalBoardHeight = 0;

public:
    BreakoutBoard(Engine* owningEngine, const BreakoutLevelInfo& level);

    void tick(double deltaTime) override;

    void onKey(int key, int scancode, int action, int mods) override;

    void onMouseMove(double xpos, double ypos) override;

private:
    void spawnBall();
    void start();
    void applyBallMovement(float deltaTime);
    void cellIndicesFromBallLocation(Ball* ball, int& row, int& column);
    void checkCollision(Ball* ball);


};

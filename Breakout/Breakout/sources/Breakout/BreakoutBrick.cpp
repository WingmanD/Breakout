#include "Breakout/BreakoutBrick.hpp"

#include "Breakout/BreakoutGameMode.hpp"
#include "Engine.hpp"

BreakoutBrick::BreakoutBrick(Engine* owner, int HP, int score, StaticMesh* mesh) : Object(owner), HP(HP), score(score){
    meshComp = new StaticMeshComponent(mesh);
    meshComp->attachTo(this);

    boxCollision = new BoxCollisionComponent(mesh->getBoundingBox());
    boxCollision->attachTo(meshComp);
}

void BreakoutBrick::hit() {
    // on hit, play sound, decrease HP, if HP is 0, destroy brick, add score and play sound
    if (hitSound)
        hitSound->play(0);
    
    if (HP != std::numeric_limits<int>::max()) {
        HP--;
        if (HP <= 0) {
            bIsDestroyed = true;
            
            if (const auto gameMode = dynamic_cast<BreakoutGameMode*>(engine->getGameMode()))
                gameMode->updateScore(score);
            
            if (breakSound) breakSound->play(0);
        }
    }
}

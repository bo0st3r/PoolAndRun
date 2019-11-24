#include "Configuration/Configuration.h"
#include "EndGameState.h"
#include "State/GameState.h"

#include <iostream>

using sf::Vector2f;

namespace pr{
    GameState::GameState(GameDataRef data):_data(data){
        _view = ViewRef(new View(Vector2f(0.0f, 0.0f), Vector2f(float(Game::VIEW_WIDTH), float(Game::VIEW_HEIGHT))));

        _isEnding = false;
        _isWinning = false;
    }

    GameState::~GameState()
    {
        ECSCoordinatorSingleton::releaseInstance();
    }

    void GameState::init(){
            std::cout << "INIT" << std::endl;
        ecs = ECSCoordinatorSingleton::getInstance();
        compManager = ComponentManagerSingleton::getInstance();

        initECS();
        initEntities();
        initTileMap();

        // Loads and sets the background
        _data->assets.loadTexture(BACKGROUND_NAME, GAME_BACKGROUND_FILEPATH);
        _data->assets.setSpriteTexture(_background, BACKGROUND_NAME);
        _background.setPosition(0,0);

        // Scales the background to fit the window size
        scaleBackgroundToWindow(_background, _data);
    }

    void GameState::initTileMap(){
        const int level[] = {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,22,22,22,22,22,22,22,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,22,21,21,20,21,21,21,21,22,22,22,22,22,22,22,22,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,22,21,21,20,21,21,21,21,21,21,21,21,21,21,21,21,21,22,22,22,22,22,22,22,22,20,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,22,21,21,20,21,21,21,21,21,21,21,21,21,21,21,21,21,22,22,22,22,22,22,22,22,20,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,22,21,21,20,21,21,21,21,22,22,22,22,22,22,22,22,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,22,22,22,22,22,22,22,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,-1,-1,-1,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,6,7,7,7,7,7,7,7,7,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,6,7,7,16,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,18,-1,-1,6,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,16,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,11,13,-1,-1,-1,6,7,7,7,7,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,7,7,8,-1,-1,-1,-1,-1,11,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,16,18,-1,-1,-1,11,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,18,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,8,-1,-1,-1,-1,-1,-1,16,0,0,0,0,0,18,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,-1,-1,-1,-1,-1,11,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,18,-1,-1,-1,16,0,0,0,0,0,0,0,-1,-1,-1,-1,6,7,8,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,6,7,7,16,18,7,7,7,8,-1,-1,6,7,7,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,6,7,7,7,7,7,8,-1,-1,-1,-1,-1,-1,-1,6,7,7,7,8,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,18,7,7,7,7,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,13,-1,-1,11,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,7,7,16,0,18,7,7,7,8,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,7,16,0,18,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,16,0,0,0,0,0,0,0,18,-1,-1,11,0,0,18,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,
            13,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,13,7,7,7,7,8,-1,-1,11,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,7,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,7,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,0,0,0,18,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,16,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,7,7,8,-1,-1,6,7,11,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,7,7,7,7,7,16,0,0,0,0,0,18,7,7,7,7,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,7,7,7,16,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,6,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,7,-1,-1,-1,-1,-1,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,6,7,7,16,0,0,0,0,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,-1,-1,6,7,7,7,7,8,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,16,0,0,0,0,0,18,-1,-1,6,7,7,7,7,7,7,7,16,18,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,16,18,-1,-1,6,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,7,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,6,7,8,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,16,0,0,0,18,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,6,7,7,7,8,-1,-1,-1,16,0,18,-1,-1,-1,6,7,7,16,0,0,0,0,0,0,0,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,16,18,-1,-1,6,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,0,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,
            13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,16,18,-1,-1,6,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,11,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,
            18,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,16,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,16,18,-1,-1,6,7,7,7,7,7,7,7,7,7,16,0,18,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,-1,-1,-1,6,7,7,7,7,7,7,7,16,0,0,0,0,0,0,0,0,0,0,0,0,0,18,7,7,7,7,7,16,0,18,7,7,7,7,7,7,7,7,16,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,18,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,16,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,7,7,7,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

        _tileMap.loadMap(GAME_TILE_SET_FILEPATH, sf::Vector2u(TILE_WIDTH, TILE_HEIGHT), level, MAP_WIDTH, MAP_HEIGHT, ecs, compManager);

    }

    void GameState::initECS()
    {
        CollisionSystem* collision = new CollisionSystem(
                                                    compManager->getEntityCharacterMap(),
                                                    compManager->getEntityRendererMap(),
                                                    compManager->getEntityPositionMap(),
                                                    compManager->getEntityVelocityMap(),
                                                    compManager->getEntityGravityMap(),
                                                    compManager->getEntityColliderMap(),
                                                    compManager->getEntityTriggerMap(),
                                                    compManager->getEntityConstraintMap(),
                                                    *_view,
                                                    TileMap::getloadedTileEntitiesRef()
                                                    );

        ControllerSystem* controller = new ControllerSystem(
                                                       compManager->getEntityVelocityMap(),
                                                       compManager->getEntityConstraintMap(),
                                                       compManager->getEntityControllerMap()
                                                       );

        PhysicSystem* physic = new PhysicSystem(
                                           compManager->getEntityPositionMap(),
                                           compManager->getEntityVelocityMap(),
                                           compManager->getEntityGravityMap(),
                                           compManager->getEntityConstraintMap()
                                           );

        RenderSystem* render = new RenderSystem(
                                           compManager->getEntityPositionMap(),
                                           compManager->getEntityRendererMap(),
                                           compManager->getEntityCharacterMap(),
                                           compManager->getEntityControllerMap(),
                                           compManager->getEntityUIMap(),
                                           _data->assets
                                           );

        RespawnSystem* respawn = new RespawnSystem(
                                              compManager->getEntityRespawnMap(),
                                              compManager->getEntityCharacterMap(),
                                              compManager->getEntityPositionMap(),
                                              compManager->getEntityVelocityMap(),
                                              compManager->getEntityTriggerMap()
                                              );

        TriggerSystem* trigger = new TriggerSystem(
                                              compManager->getEntityTriggerMap(),
                                              compManager->getEntityPositionMap(),
                                              compManager->getEntityVelocityMap(),
                                              compManager->getEntityRendererMap(),
                                              compManager->getEntityCharacterMap()
                                              );

        ecs->addSystem(controller);
        ecs->addSystem(collision);
        ecs->addSystem(trigger);
        ecs->addSystem(respawn);
        ecs->addSystem(physic);
        ecs->addSystem(render);
    }

    void GameState::initEntities()
    {
        EntityCreator::createPlayer(2, 28, "ball0", *compManager, *ecs);

        EntityCreator::createEnnemyBall(11, 28, "ball9", *compManager, *ecs);
        EntityCreator::createEnnemyBall(8, 22, "ball10", *compManager, *ecs);
        EntityCreator::createEnnemyBall(67, 28, "ball11", *compManager, *ecs);
        EntityCreator::createEnnemyBall(70, 20, "ball12", *compManager, *ecs);
        EntityCreator::createEnnemyBall(145, 28, "ball13", *compManager, *ecs);
        EntityCreator::createEnnemyBall(134, 23, "ball14", *compManager, *ecs);
        EntityCreator::createEnnemyBall(192, 19, "ball15", *compManager, *ecs);
        EntityCreator::createEnnemyBall(243, 13, "ball8", *compManager, *ecs);

        EntityCreator::createEnnemyCue(55, 28, *compManager, *ecs);
        EntityCreator::createEnnemyCue(93, 22, *compManager, *ecs);
        EntityCreator::createEnnemyCue(121, 28, *compManager, *ecs);
        EntityCreator::createEnnemyCue(147, 23, *compManager, *ecs);
        EntityCreator::createEnnemyCue(209, 15, *compManager, *ecs);

        EntityCreator::createCheckPoint(2, 28, *compManager, *ecs);
        EntityCreator::createCheckPoint(48, 23, *compManager, *ecs);
        EntityCreator::createCheckPoint(89, 20, *compManager, *ecs);
        EntityCreator::createCheckPoint(183, 28, *compManager, *ecs);
        EntityCreator::createCheckPoint(182, 9, *compManager, *ecs);
        EntityCreator::createCheckPoint(235, 28, *compManager, *ecs);

        EntityCreator::createHole(24, 28, TAG_ENEMY_BALL, *compManager, *ecs);
        EntityCreator::createHole(73, 28, TAG_ENEMY_BALL, *compManager, *ecs);
        EntityCreator::createHole(156, 30, TAG_ENEMY_BALL, *compManager, *ecs);
        EntityCreator::createHole(189, 28, TAG_ENEMY_BALL, *compManager, *ecs);
        EntityCreator::createHole(266, 28, TAG_ENEMY_BALL, *compManager, *ecs);

        EntityCreator::createHole(160, 28, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(188, 19, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(222, 18, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(245, 9, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(245, 10, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(245, 12, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(245, 13, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(245, 15, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(245, 16, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(245, 21, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(245, 22, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(249, 24, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(249, 25, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(249, 27, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(249, 28, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(250, 17, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(251, 17, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(252, 17, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(250, 23, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(251, 23, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(252, 23, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(253, 12, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(253, 13, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(257, 15, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(257, 16, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(257, 21, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(257, 22, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(257, 27, TAG_PLAYER, *compManager, *ecs);
        EntityCreator::createHole(257, 28, TAG_PLAYER, *compManager, *ecs);

        EntityCreator::createWarp(1, 22, 46, 23, *compManager, *ecs);
        EntityCreator::createWarp(76, 28, 87, 20, *compManager, *ecs);
        EntityCreator::createWarp(165, 28, 181, 28, *compManager, *ecs);
        EntityCreator::createWarp(280, 28, 181, 9, *compManager, *ecs);

        EntityCreator::createEndLevel(280, 23, *compManager, *ecs);

        EntityCreator::createUIElement(10, 10, 0.05, 0.05, UITypeEnum::DashIndicator, -1, "ball8", *compManager, *ecs);
        EntityCreator::createHealthBar(100, 10, *compManager, *ecs);
    }

    void GameState::handleInput(Event event){
        switch(event.type){
            case Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Num1){
                    _isEnding = true;
                    _isWinning = true;
                } else if(event.key.code == sf::Keyboard::Num2){
                    _isEnding = true;
                    _isWinning = false;
                } else if(event.key.code == sf::Keyboard::Num5){
                    _data->window.close();
                }
                break;


            case Event::Closed:
                _data->window.close();
                break;

            default:
                break;
        }
    }

    void GameState::update(float dt){
        ecs->updateSystems(dt);

            // Put that one line below after entities updates to avoid stuttering
       // _view->setCenter(Vector2f(336.0f, 380.0f));
//        if(_view->getCenter)
/*
ICIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
ICIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
ICIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
ICIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
https://www.youtube.com/watch?v=CpVbMeYryKo
https://www.youtube.com/watch?v=CpVbMeYryKo
https://www.youtube.com/watch?v=CpVbMeYryKo
https://www.youtube.com/watch?v=CpVbMeYryKo
ICIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
ICIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
ICIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
ICIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
    */
        resizeView();
        _data->window.setView(*_view);

        if(_isEnding){
            _data->machine.addState(StateRef(new EndGameState(_data, _isWinning)));
        }    }


    void GameState::str(){
        std::cout << "game" << std::endl;
    }

    void GameState::draw(float dt){
        _data->window.draw(_background);
        _data->window.draw(_tileMap);
        ecs->updateRender(dt, _data->window, *_view);
    }

    void GameState::resizeView(){
        float aspectRatio = float(_data->window.getSize().x) /  float(_data->window.getSize().y);
        _view->setSize(Game::VIEW_WIDTH * aspectRatio, Game::VIEW_HEIGHT* aspectRatio);
    }

}

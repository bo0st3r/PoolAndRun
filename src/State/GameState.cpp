#include "Configuration/Configuration.h"
#include "State/GameState.h"

#include <iostream>

using sf::Vector2f;

namespace pr{
    GameState::GameState(GameDataRef data):_data(data){
        _view = ViewRef(new View(Vector2f(0.0f, 0.0f), Vector2f(float(Game::VIEW_WIDTH), float(Game::VIEW_HEIGHT))));

        ecs = ECSCoordinatorSingleton::getInstance();
        compManager = ComponentManagerSingleton::getInstance();
    }

    GameState::~GameState()
    {
        delete ecs;
        delete compManager;
    }

    void GameState::init(){
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
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,11,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,7,7,-1,-1,-1,6,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,7,-1,-1,-1,-1,-1,-1,-1,6,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,7,7,7,7,7,7,7,7,7,7,8,-1,-1,6,8,-1,-1,6,7,7,7,-1,7,7,7,7,8,-1,-1,7,7,7,7,7,-1,-1,7,0,0,0,0,7,7,7,7,7,7,7,11,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,
            0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,6,11,0,0,0,-1,0,0,0,0,13,8,-1,-1,0,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,6,7,
            0,0,0,0,0,0,0,0,0,0,0,13,7,7,7,7,7,11,0,0,0,0,-1,0,0,0,0,0,13,8,-1,-1,0,0,0,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,6,11,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,13,8,-1,-1,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,11,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,13,8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,6,11,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,13,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,11,0,0,0,0
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
                                                    compManager->getEntityColliderMap(),
                                                    compManager->getEntityTriggerMap(),
                                                    compManager->getEntityConstraintMap(),
                                                    *_view
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
                                              compManager->getEntityRendererMap()
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
        EntityCreator::createPlayer(0, 0, "ball0", *compManager, *ecs);
        EntityCreator::createEnnemyBall(0, 100, "ball1", *compManager, *ecs);
    }

    void GameState::handleInput(Event event){
        switch(event.type){
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

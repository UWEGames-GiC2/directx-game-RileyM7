//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <list>
#include "CommonStates.h"
#include "../DirectXTK/Inc/Effects.h" //this clashes with a SDK file so must explitily state it
#include "Keyboard.h"
#include "Mouse.h"
#include "Audio.h"
#include "CMOGO.h"
#include "TPSCamera.h"
#include "FPSCamera.h"
#include "Terrain.h"

using std::list;

// Forward declarations
struct GameData;
struct DrawData;
struct DrawData2D;
class GameObject;
class GameObject2D;
class FPSCamera;
class TPSCamera;
class Light;
class Sound;
class Player;
class Projectile;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game() noexcept;
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND _window, int _width, int _height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int _width, int _height);

    // Properties
    void GetDefaultSize( int& _width, int& _height ) const noexcept;

private:

    void Update(DX::StepTimer const& _timer);
    void RenderGame();
    void RenderMenu();
    void RenderWin();
    void RenderLose();
    void ResetGame();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    bool                                            targetHit = false;
    int                                             onGroundCount = 1;

    float                                           gravity = 500.0f;


    enum game_states
    {
                                                    menu = 0, game = 1, win = 2, lose = 3
    };

    game_states                                     currentState = menu;

    enum trigger
    {
                                                    triggerNone = 0, trigger1 = 1, trigger2 = 2
    };

    trigger                                         TriggerState = triggerNone;


    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

    //Scarle Added stuff
    GameData* m_GameData = NULL;			//Data to be shared to all Game Objects as they are ticked
    DrawData* m_DrawData = NULL;			//Data to be shared to all 3D Game Objects as they are drawn
    DrawData2D* m_DrawData2DMenu = NULL;	    //Data to be passed by game to all 2D Game Objects via Draw 
    DrawData2D* m_DrawData2DWin = NULL;
    DrawData2D* m_DrawData2DLose = NULL;
    DrawData2D* m_DrawDataMenu = NULL;      //Data to be passed by game to all Menu Game objects via Draw

    //Basic 3D renderers
    std::shared_ptr<FPSCamera> m_FPScam = NULL; //FPS camera
    std::shared_ptr<TPSCamera> m_TPScam = NULL;//TPS cam
    std::shared_ptr <Light> m_light = NULL; //base light
    std::shared_ptr <Player> pPlayer = NULL;
    

    //required for the CMO model rendering system
    DirectX::CommonStates* m_states = NULL;
    DirectX::IEffectFactory* m_fxFactory = NULL;

    //basic keyboard and mouse input system
    void ReadInput(); //Get current Mouse and Keyboard states
    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    std::unique_ptr<DirectX::Mouse> m_mouse;

    std::vector<std::shared_ptr<GameObject>> m_GameObjects; //data structure to hold pointers to the 3D Game Objects
    std::vector< std::shared_ptr<GameObject2D>> m_GameObjects2D; //data structure to hold pointers to the 2D Game Objects 
    std::vector< std::shared_ptr<GameObject2D>> m_GameObjectsMenu; //Vector to store all shared ptr game objects for the menu
    std::vector< std::shared_ptr<GameObject2D>> m_GameObjectsWin; //Vector to store all shared ptr game objects for the menu
    std::vector< std::shared_ptr<GameObject2D>> m_GameObjectsLose;

    //list<CMOGO*> m_CMOGameObjects; //data structure to hold pointers to all 3D CMO Game Objects
    //list<CMOGO*> m_PhysicsObjects

    std::vector<std::shared_ptr<CMOGO>> m_ColliderObjects;
    std::vector<std::shared_ptr<Terrain>> m_TriggerObjects;
    std::vector<std::shared_ptr<CMOGO>> m_PhysicsObjects;

    std::vector<std::shared_ptr<CMOGO>> m_PlayerProjectile;
    std::vector<std::shared_ptr<CMOGO>> m_Projectile;

    void CheckCollision();
     void CheckTrigger();
                                         
    //sound stuff
	//This uses a simple system, but a better pipeline can be used using Wave Banks
	//See here: https://github.com/Microsoft/DirectXTK/wiki/Creating-and-playing-sounds Using wave banks Section
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
    list<Sound*>m_Sounds;
};

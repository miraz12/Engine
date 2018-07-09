#pragma once 

namespace Base
{
    class Manager
    {
    public:
        /// constructor
        Manager();
        /// destructor
        virtual ~Manager();


        /// called when attached to game server
        virtual void OnActivate();
        /// called when removed from game server
        virtual void OnDeactivate();
        /// return true if currently active
        bool IsActive() const;
        /// called before frame by the game server
        virtual void OnBeginFrame();
        /// called per-frame by the game server
        virtual void OnFrame();
        /// called after frame by the game server
        virtual void OnEndFrame();
        /// called after loading game state
        virtual void OnLoad();
        /// called before saving game state
        virtual void OnSave();
        /// called by Game::Server::Start() when the world is started
        virtual void OnStart();
        /// render a debug visualization 
        virtual void OnRenderDebug();

    private:
        bool isActive;
    };
}
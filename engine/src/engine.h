// Engine Header File
#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>

using namespace sf;
using namespace std;

class Scene 
{
    public:
      Scene() = default;
      virtual ~Scene();
      virtual void Load() = 0;
      virtual void LoadAsync();
      virtual void UnLoad();
      virtual void Update(const double& dt);
      virtual void Render();
      bool isLoaded() const;
      shared_ptr<Entity> makeEntity();

      EntityManager ents;

    protected:
      void setLoaded(bool);
    private:
      mutable bool _loaded;
      mutable future<void> _loaded_future;
      mutable mutex _loaded_mtx;
};

class Engine 
{
    public:
      Engine() = delete;
      static void Start(unsigned int width, unsigned int height,
                        const string& gameName, Scene* scn);
      static void ChangeScene(Scene*);
      static RenderWindow& GetWindow();
      static Vector2u getWindowSize();
      static void setVsync(bool b);

    private:
      static Scene* _activeScene;
      static string _gameName;
      static void Update();
      static void Render(RenderWindow& window);
};

namespace timing 
{
    // Return time since Epoc
    long long now();
    // Return time since last() was last called.
    long long last();
} // namespace timing
#include <functional>
#include <unordered_map>
#include <SDL2/SDL.h>

struct InputEvent {
    enum class Type {
        KEY_PRESS,
        KEY_RELEASE,
        MOUSE_LEFT_CLICK,
    };

    Type type;
    int keyOrButtonCode;
};


bool operator==(const InputEvent& lhs, const InputEvent& rhs) {
    return lhs.type == rhs.type && lhs.keyOrButtonCode == rhs.keyOrButtonCode;
}

namespace std {
    template <>
    struct hash<InputEvent> {
        size_t operator()(const InputEvent& event) const {
            return hash<int>()(static_cast<int>(event.type)) ^ 
                   hash<int>()(event.keyOrButtonCode);
        }
    };
}

class InputContext {
  std::unordered_map<InputEvent, std::function<void()>> bindings;

  public:
  void bind(InputEvent event, std::function<void()> action) {
    bindings[event] = action;
  }

  void processInput(InputEvent event) {
    if (bindings.find(event) != bindings.end()) {
      bindings[event]();
    }
  }
};

class InputManager {
  private:
    InputContext* activeContext;

    void handleRawInput(const SDL_Event& sdlEvent) {
      InputEvent event;

      if (sdlEvent.type == SDL_KEYDOWN) {
        event.type = InputEvent::Type::KEY_PRESS;
      } else if (sdlEvent.type == SDL_KEYUP) {
        event.type = InputEvent::Type::KEY_RELEASE;
      }

      event.keyOrButtonCode = sdlEvent.key.keysym.sym;

      if (activeContext) {
        activeContext->processInput(event);
      }
    }

  protected:
    InputManager() {}

    static InputManager* instance;

  public:
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    static InputManager *GetInstance() {
      if (!instance) {
        instance = new InputManager();
      }

      return instance;
    }


    void Update(const SDL_Event& event) {
      handleRawInput(event);
    }

    void setActiveContext(InputContext* context) {
      activeContext = context;
    }
};

InputManager* InputManager::instance = nullptr;

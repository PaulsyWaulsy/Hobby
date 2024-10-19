
class GameObject {
public:
    virtual ~GameObject();
    virtual void upate(float deltaTime) = 0;
    virtual void render();
};

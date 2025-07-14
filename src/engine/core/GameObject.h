#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>

class GameObject {
public:
    GameObject(const std::string& name);
    virtual ~GameObject();

    //core methods
    virtual void Update(float deltaTime);
    virtual void Render();

    //transformation
    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void SetScale(float x, float y, float z);

    void Translate(float x, float y, float z);
    void Rotate(float x, float y, float z);
    void Scale(float x, float y, float z);

    //Hierachy
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);

    //Getters
    const std::string& GetName() const { return m_name;}
    bool IsActive() const { return m_active; }

    //Setters
    void SetActive(bool active) { m_active = active; }

protected:
    std::string m_name;
    bool m_active;

    //Transformation properties
    float m_position[3];
    float m_rotation[3];
    float m_scale[3];

    //Hierarchy
    GameObject* m_parent;
    std::vector<GameObject*> m_children;
};

#endif // GAME_OBJECT_H

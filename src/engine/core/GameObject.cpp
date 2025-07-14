#include "GameObject.h"
#include <algorithm>

GameObject::GameObject(const std::string& name) : m_name(name), m_active(true) {
    m_position[0] = 0.0f;
    m_position[1] = 0.0f;
    m_position[2] = 0.0f;

    m_rotation[0] = 0.0f;
    m_rotation[1] = 0.0f;
    m_rotation[2] = 0.0f;

    m_scale[0] = 1.0f;
    m_scale[1] = 1.0f;
    m_scale[2] = 1.0f;
}

GameObject::~GameObject() {}

void GameObject::Update(float deltaTime) {}

void GameObject::Render() {}

void GameObject::SetPosition(float x, float y, float z) {
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
}

void GameObject::SetRotation(float x, float y, float z) {
    m_rotation[0] = x;
    m_rotation[1] = y;
    m_rotation[2] = z;
}

void GameObject::SetScale(float x, float y, float z) {
    m_scale[0] = x;
    m_scale[1] = y;
    m_scale[2] = z;
}

void GameObject::Translate(float x, float y, float z) {
    m_position[0] += x;
    m_position[1] += y;
    m_position[2] += z;
}

void GameObject::Rotate(float x, float y, float z) {
    m_rotation[0] += x;
    m_rotation[1] += y;
    m_rotation[2] += z;
}

void GameObject::Scale(float x, float y, float z) {
    m_scale[0] *= x;
    m_scale[1] *= y;
    m_scale[2] *= z;
}

void GameObject::AddChild(GameObject* child) {
    m_children.push_back(child);
}

void GameObject::RemoveChild(GameObject* child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        m_children.erase(it);
    }
}

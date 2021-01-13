#pragma once

#include <vector>

#include "Math/Math.h"

#ifdef NoEngine_Editor
namespace Editor
{
    class ComponentEditors;
    class SceneEditors;
}
#endif

namespace Core
{
    class GameObject;
    class Scene;
    // Adapted from Python Implementation : https://github.com/Eikins/3D-Sea-Project/blob/master/sea3d/math/matrix4.py
    // From Noe Masse
    class Transform
    {
#ifdef NoEngine_Editor
        friend class Editor::ComponentEditors;
        friend class Editor::SceneEditors;
#endif
        friend class GameObject;
        friend class Scene;
    private:
        Transform* _parent = nullptr;
        std::vector<Transform*> _children;
    
        GameObject* _gameObject = nullptr;

        Math::Vector3 _position = Math::Vector3::Zero;
        Math::Quaternion _rotation = Math::Quaternion::Identity;
        Math::Vector3 _scale = Math::Vector3::One;

        Math::Vector3 _eulerAngles = Math::Vector3::Zero;
    
        bool _hasChanged = false;

        Math::Matrix4x4 _transformMatrix = Math::Matrix4x4::Identity;
        // Cache the matrix to avoid to recompute each frame
        Math::Matrix4x4 _localToWorldMatrix = Math::Matrix4x4::Identity;

        bool HasParentChanged();
    
        Transform(GameObject* gameObject);
        Transform() {}
    public:
        ~Transform();

        inline GameObject* GetGameObject() { return _gameObject; }

        void ForceUpdate();
        
        // Hierarchy
        // TODO: Adapt scene to support parental modifications
        void SetParent(Transform* parent);
        void AddChild(Transform* child);
        void RemoveChild(Transform* child);

        // Transformations
        void SetPosition(const Math::Vector3& position);
        void SetRotation(const Math::Quaternion& rotation);
        void SetEulerAngles(const Math::Vector3& eulerAngles);
        void SetScale(const Math::Vector3& scale);

        inline Math::Vector3 GetPosition() const { return _position; }
        inline Math::Quaternion GetRotation() const { return _rotation; }
        inline Math::Vector3 GetScale() const { return _scale; }
        inline Math::Vector3 GetEulerAngles() const { return _eulerAngles; }

        inline Math::Vector3 GetWorldPosition() { return GetLocalToWorldMatrix().GetTranslation(); }
        inline Math::Vector3 GetLossyScale() { return GetLocalToWorldMatrix().GetScale(); }

        void Translate(const Math::Vector3& translation);
        void Rotate(const Math::Quaternion& rotation);

        // Helpers
        Math::Vector3 RightVector();
        Math::Vector3 UpVector();
        Math::Vector3 ForwardVector();

        // Matrix
        Math::Matrix4x4 GetLocalToWorldMatrix();
    };
}
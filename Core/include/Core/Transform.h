#pragma once

#include <vector>

#include "Math/Math.h"

namespace Editor
{
    class CameraEditor;
}

namespace Core
{
    // Adapted from Python Implementation : https://github.com/Eikins/3D-Sea-Project/blob/master/sea3d/math/matrix4.py
    // From Noe Masse
    class Transform
    {
        friend class Editor::CameraEditor;
    private:
        Transform* _parent;
        std::vector<Transform*> _children;
        
        Math::Vector3 _position;
        Math::Quaternion _rotation;
        Math::Vector3 _scale;
    
        bool _hasChanged = false;

        Math::Matrix4x4 _transformMatrix;
        // Cache the matrix to avoid to recompute each frame
        Math::Matrix4x4 _localToWorldMatrix;

        bool HasParentChanged();
    public:
        Transform(Transform* parent);
        Transform() : Transform(nullptr) {}
        ~Transform();

        void ForceUpdate();
        
        // Hierarchy
        void SetParent(Transform* parent);
        void AddChild(Transform* child);
        void RemoveChild(Transform* child);

        // Transformations
        void SetPosition(const Math::Vector3& position);
        void SetRotation(const Math::Quaternion& rotation);
        void SetScale(const Math::Vector3& scale);

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
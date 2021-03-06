#include "Core/Transform.h"

using namespace Math;

namespace Core
{
#pragma region Constructor / Destructor

    Transform::Transform(GameObject* gameObject)
    {
        _gameObject = gameObject;
    }

    Transform::~Transform()
    {
        // Default behaviour : children become root transforms
        for (int i = 0; i < _children.size(); i++)
        {
            _children[i]->_parent = nullptr;
        }
    }
#pragma endregion

#pragma region Methods
    void Transform::ForceUpdate()
    {
        _hasChanged = true;
    }

    bool Transform::HasParentChanged()
    {
        if (_parent == nullptr)
        {
            return false;
        }
        else
        {
            return _parent->_hasChanged || _parent->HasParentChanged();
        }
    }

    void Transform::SetParent(Transform* parent)
    {
        if (parent == _parent)
        {
            return;
        }

        if (_parent != nullptr)
        {
            _parent->RemoveChild(this);
        }

        if (parent != nullptr)
        {
            parent->AddChild(this);
        }

        _parent = parent;
        _hasChanged = true;
    }

    void Transform::AddChild(Transform* child)
    {
        if (child->_parent != nullptr)
        {
            child->_parent->RemoveChild(this);
        }
        child->_parent = this;
        _children.push_back(child);
        _hasChanged = true;
    }

    void Transform::RemoveChild(Transform* child)
    {
        int childIndex = -1;
        for (int i = 0; i < _children.size(); i++)
        {
            if (child == _children[i])
            {
                childIndex = i;
                break;
            }
        }
        if (childIndex >= 0)
        {
            child->_parent = nullptr;
            child->_hasChanged = true;
            _children.erase(_children.begin() + childIndex);
        }
    }

    void Transform::SetPosition(const Math::Vector3& position)
    {
        if (_position != position)
        {
            _position = position;
            _hasChanged = true;
        }
    }

    void Transform::SetRotation(const Math::Quaternion& rotation)
    {
        if (_rotation != rotation)
        {
            _rotation = rotation;
            _eulerAngles = rotation.GetEulerAngles();
            _hasChanged = true;
        }
    }

    void Transform::SetEulerAngles(const Math::Vector3& eulerAngles)
    {
        if (_eulerAngles != eulerAngles)
        {
            _eulerAngles = eulerAngles;
            _rotation = Quaternion::Euler(_eulerAngles);
            _hasChanged = true;
        }
    }

    void Transform::SetScale(const Math::Vector3& scale)
    {
        if (_scale != scale)
        {
            _scale = scale;
            _hasChanged = true;
        }
    }

    void Transform::Translate(const Math::Vector3& translation)
    {
        if (translation != Vector3::Zero)
        {
            _position += translation;
            _hasChanged = true;
        }
    }

    void Transform::Rotate(const Math::Quaternion& rotation)
    {
        if (_rotation != Quaternion::Identity)
        {
            _rotation *= rotation;
            _hasChanged = true;
        }
    }

    Vector3 Transform::RightVector()
    {
        if (_hasChanged) GetLocalToWorldMatrix();
        return Vector3::Normalize(Vector3(_localToWorldMatrix.m_0_0, _localToWorldMatrix.m_1_0, _localToWorldMatrix.m_2_0));
    }

    Vector3 Transform::UpVector()
    {
        if (_hasChanged) GetLocalToWorldMatrix();
        return Vector3::Normalize(Vector3(_localToWorldMatrix.m_0_1, _localToWorldMatrix.m_1_1, _localToWorldMatrix.m_2_1));
    }

    Vector3 Transform::ForwardVector()
    {
        if (_hasChanged) GetLocalToWorldMatrix();
        return Vector3::Normalize(Vector3(_localToWorldMatrix.m_0_2, _localToWorldMatrix.m_1_2, _localToWorldMatrix.m_2_2));
    }

    Matrix4x4 Transform::GetLocalToWorldMatrix()
    {
        if (_hasChanged)
        {
            _transformMatrix = Matrix4x4::TRS(_position, _rotation, _scale);
            _localToWorldMatrix = _parent == nullptr ? _transformMatrix : _parent->GetLocalToWorldMatrix() * _transformMatrix;
            _hasChanged = false;
            for (auto& transform : _children)
            {
                transform->_hasChanged = true;
            }
        } else if (HasParentChanged())
        {
            _localToWorldMatrix = _parent->GetLocalToWorldMatrix() * _transformMatrix;
        }

        return _localToWorldMatrix;
    }
#pragma endregion

}
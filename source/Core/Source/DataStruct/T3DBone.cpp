

#include "T3DBone.h"


namespace Tiny3D
{
    BonePtr Bone::create(const String &name, const Matrix4 &m)
    {
        BonePtr bone = new Bone(name, m);
        if (bone != nullptr)
        {
            bone->release();
        }
        else
        {
            T3D_SAFE_RELEASE(bone);
        }
        return bone;
    }

    Bone::Bone()
        : Node()
        , mTranslation(Vector3::ZERO)
        , mScaling(Vector3(1.0, 1.0, 1.0))
        , mOrientation(Quaternion::IDENTITY)
        , mIsDirty(true)
    {

    }

    Bone::Bone(const String &name, const Matrix4 &m)
        : Node()
        , mTranslation(Vector3::ZERO)
        , mScaling(Vector3(1.0, 1.0, 1.0))
        , mOrientation(Quaternion::IDENTITY)
        , mIsDirty(true)
    {
        setName(name);

        m.decomposition(mTranslation, mScaling, mOrientation);

//         Vector3 translation;
//         Vector3 scaling;
//         Quaternion orientation;
//         m.decomposition(translation, scaling, orientation);
// 
//         setPosition(translation);
//         setScale(scaling);
//         setOrientation(orientation);
    }

    Bone::~Bone()
    {

    }

    Node::Type Bone::getNodeType() const
    {
        return E_NT_BONE;
    }

    NodePtr Bone::clone() const
    {
        BonePtr bone = new Bone();
        if (bone != nullptr)
        {
            bone->release();
            cloneProperties(bone);
        }
        return bone;
    }

    void Bone::cloneProperties(const NodePtr &node) const
    {
        BonePtr bone = smart_pointer_cast<Bone>(node);

        bone->mTranslation = mTranslation;
        bone->mScaling = mScaling;
        bone->mOrientation = mOrientation;
        bone->mCombineTransform = mCombineTransform;
    }

    void Bone::onAttachParent(const NodePtr &parent)
    {
        Node::onAttachParent(parent);
        setDirty(true, true);
    }

    void Bone::setDirty(bool isDirty, bool recursive /* = false */)
    {
        mIsDirty = isDirty;

        if (recursive)
        {
            auto itr = mChildren.begin();

            while (itr != mChildren.end())
            {
                const BonePtr &bone = smart_pointer_cast<Bone>(*itr);
                bone->setDirty(isDirty, recursive);
                ++itr;
            }
        }
    }

    const Transform &Bone::getCombineTransform()
    {
        if (isDirty())
        {
            NodePtr parent = getParent();
            while (parent != nullptr && parent->getNodeType() != E_NT_BONE)
                parent = parent->getParent();

            if (parent != nullptr)
            {
                BonePtr bone = smart_pointer_cast<Bone>(parent);
                const Transform &transform = bone->getCombineTransform();
                mCombineTransform.applyTransform(transform, mTranslation, mOrientation, Vector3::UNIT_SCALE);
//                 Quaternion R = transform.getOrientation() * mOrientation;
// //                 Vector3 S = transform.getScale() * mScaling;
//                 Vector3 T = transform.getOrientation() * mTranslation;
// //                 T = T * transform.getScale();
//                 T = transform.getTranslate() + T;
//                 mCombineTransform.setTranslate(T);
//                 mCombineTransform.setOrientation(R);
// //                 mCombineTransform.setScale(S);
// 
//                 const Matrix4 &m0 = transform.getAffineMatrix();
//                 Matrix4 Mt(mTranslation);
//                 Matrix4 Ms(mScaling);
//                 Matrix4 Mr(mOrientation);
//                 Matrix4 m1 = Mr * Ms;
//                 m1.setTranslate(mTranslation);
//                 Matrix4 m2 = m1 * m0;
//                 mCombineTransform.update();
            }
            else
            {
                mCombineTransform.setTranslate(mTranslation);
                mCombineTransform.setOrientation(mOrientation);
                mCombineTransform.setScale(Vector3::UNIT_SCALE);
                mCombineTransform.update();
            }

            setDirty(false);
        }

        return mCombineTransform;
    }

    void Bone::updateBone()
    {
        getCombineTransform();

//         auto itr = mChildren.begin();
// 
//         while (itr != mChildren.end())
//         {
//             const BonePtr &bone = smart_pointer_cast<Bone>(*itr);
//             bone->updateBone();
//             ++itr;
//         }
    }
}


#include "matrix4.hpp"
#include "../Quaternion/quaternion.hpp"
#include "Core/Maths/Utils/numerics.hpp"

namespace Solid
{
    template<typename T>
    Mat4<T>::Mat4()
    {
        elements = Mat4::Zero.elements;
    }

    template<typename T>
    Mat4<T>::Mat4(T e0, T e1, T e2, T e3,
                  T e4, T e5, T e6, T e7,
                  T e8, T e9, T e10, T e11,
                  T e12, T e13, T e14, T e15)
    {
        //TODO MAKE CONTRUCTOR ROW CONVENTION
        elements[0] = e0;
        elements[1] = e1;
        elements[2] = e2;
        elements[3] = e3;

        elements[4] = e4;
        elements[5] = e5;
        elements[6] = e6;
        elements[7] = e7;

        elements[8] = e8;
        elements[9] = e9;
        elements[10]= e10;
        elements[11]= e11;

        elements[12] = e12;
        elements[13] = e13;
        elements[14] = e14;
        elements[15] = e15;

    }

    template<typename T>
    Mat4<T>::Mat4(std::array<T, 16> _elements)
    {
        elements = _elements;
    }

#pragma region Static Methods

    template<typename T>
    Mat4<T> Mat4<T>::Zero = {0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0};

    template<typename T>
    Mat4<T> Mat4<T>::Identity = {1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1};

    template<typename T>
    Mat4<T> Mat4<T>::Translate(const Vec3 &_pos)
    {
        Mat4<T> tranlate = Mat4<T>::Identity;

        tranlate.elements[12] = _pos.x;
        tranlate.elements[13] = _pos.y;
        tranlate.elements[14] = _pos.z;

        return tranlate;
    }

    template<typename T>
    Mat4<T> Mat4<T>::Rotation(const Quat &_rot)
    {
        Mat4<T> matrix;


        matrix.elements[0] = 1.0f - (2.0f * _rot.y * _rot.y) - (2.0f * _rot.z * _rot.z);
        matrix.elements[1] = 2.0f * _rot.x * _rot.y - 2.0f * _rot.w * _rot.z;
        matrix.elements[2] = 2.0f * _rot.x * _rot.z + 2.0f * _rot.w * _rot.y;
        matrix.elements[3] = 0;

        matrix.elements[4] = 2.0f * _rot.x * _rot.y + 2.0f * _rot.w * _rot.z;
        matrix.elements[5] = 1.0f - (2.0f * _rot.x * _rot.x) - (2.0f * _rot.z * _rot.z);
        matrix.elements[6] = 2.0f * _rot.y * _rot.z - 2.0f * _rot.w * _rot.x;
        matrix.elements[7] = 0;

        matrix.elements[8] = 2.0f * _rot.x * _rot.z - 2.0f * _rot.w * _rot.y;
        matrix.elements[9] = 2.0f * _rot.y * _rot.z + 2.0f * _rot.w * _rot.x;
        matrix.elements[10] = 1.0f - (2.0f * _rot.x * _rot.x) - (2.0f * _rot.y * _rot.y);
        matrix.elements[11] = 0;

        matrix.elements[12] = 0;
        matrix.elements[13] = 0;
        matrix.elements[14] = 0;
        matrix.elements[15] = 1;

        return matrix;
    }

    template<typename T>
    Mat4<T> Mat4<T>::Scale(const Vec3 &_scale)
    {
        Mat4 result;

        result.elements[0] = _scale.x;
        result.elements[5] = _scale.y;
        result.elements[10] = _scale.z;
        result.elements[15] = 1;

        return result;
    }

    template<typename T>
    Mat4<T> Mat4<T>::Transform(const Vec3 &_pos, const Quat &_rot, const Vec3 &_scale)
    {
        Mat4<T> TRS = Scale(_scale)*Rotation(_rot)*Translate(_pos);
        return TRS ;
    }

    template<typename T>
    Mat4<T> Mat4<T>::Perspective(float _fov, float _aspect, float _near, float _far)
    {
        /*T ymax = tanf(_fov * 3.14f / 180.f / 2.f);
        T xmax = ymax * _aspect;



        Mat4 m;

        float temp = _far - _near;

        m.elements[0] = 1 / xmax;
        m.elements[1] = 0.0;
        m.elements[2] = 0.0;
        m.elements[3] = 0.0;
        m.elements[4] = 0.0;
        m.elements[5] = 1 / ymax;
        m.elements[6] = 0.0;
        m.elements[7] = 0.0;
        m.elements[8] = 0.f;
        m.elements[9] = 0.f;
        m.elements[10] = -(_far + _near) / temp;
        m.elements[11] = -1;
        m.elements[12] = 0.0;
        m.elements[13] = 0.0;
        m.elements[14] = -2 * (_far * _near) / temp;
        m.elements[15] = 0.0;*/

        Mat4 m;

        T range = _near - _far;
        T invRange = _far - _near;
        T halfFov = tanf(_fov * 3.14f / 180.f / 2.f);

        m.elements[0] = 1/(halfFov*_aspect);
        m.elements[5] = 1/halfFov;
        m.elements[10] = -(_far+_near) / invRange;
        m.elements[11] = -1;
        m.elements[14] = -2*_far * _near / invRange;

        return m;
    }

    template<typename T>
    bool Mat4<T>::DecomposeTransform(const Mat4<T>& transform, Vec3& translation, Quat& rotation, Vec3& scale)
    {
        Mat4<float> LocalMatrix(transform);

        // Normalize the matrix.
        if (Maths::Equals0(LocalMatrix[15]))
            return false;

        // First, isolate perspective.  This is the messiest.
        if (
                (!Maths::Equals0(LocalMatrix[3])) ||
                (!Maths::Equals0(LocalMatrix[7])) ||
                (!Maths::Equals0(LocalMatrix[11])))
        {
            // Clear the perspective partition
            LocalMatrix[3] = LocalMatrix[7] = LocalMatrix[11] = 0.0f;
            LocalMatrix[15] = 1;
        }

        // Next take care of translation (easy).
        translation = Vec3(LocalMatrix[12], LocalMatrix[13], LocalMatrix[14]);
        LocalMatrix[12] =  LocalMatrix[13] = LocalMatrix[14] = 0.0f;

        Vec3 Row[3], Pdum3;

        // Now get scale and shear.
        for (size_t i = 0; i < 3; ++i)
        {

            Row[i].x = LocalMatrix[(4*i)];
            Row[i].y = LocalMatrix[(4*i)+1];
            Row[i].z = LocalMatrix[(4*i)+2];
        }
        // Compute X scale factor and normalize first row.
        scale.x = Row[0].Length();
        Row[0] = Row[0].GetNormalized();//detail::scale(Row[0], static_cast<T>(1));
        scale.y = Row[1].Length();
        Row[1] = Row[1].GetNormalized();
        scale.z = Row[2].Length();;
        Row[2] = Row[2].GetNormalized();

        Pdum3 = Vec3::Cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
        if (Vec3::Dot(Row[0], Pdum3) < 0)
        {
            scale *= static_cast<T>(-1);
            for (size_t i = 0; i < 3; i++)
            {
                Row[i] *= static_cast<T>(-1);
            }
        }

        float tr = Row[0].x + Row[1].y + Row[2].z;
        if(tr > 0)
        {
            float s = Maths::Sqrt(tr + 1) * 2;
            rotation.w = 0.25f * s;
            rotation.x = (Row[2].y - Row[1].z) / s;
            rotation.y = (Row[0].z - Row[2].x) / s;
            rotation.z = (Row[1].x - Row[0].y) / s;

        }
        else if((Row[0].x > Row[1].y) && (Row[0].x > Row[2].z))
        {
            float s = Maths::Sqrt(1 + Row[0].x - Row[1].y - Row[2].z) * 2;
            rotation.w = (Row[2].y - Row[1].z) / s;
            rotation.x = 0.25f * s;
            rotation.y = (Row[0].y + Row[1].x) / s;
            rotation.z = (Row[0].z + Row[2].x) / s;
        }
        else if((Row[1].y > Row[2].z))
        {
            float s = Maths::Sqrt(1 + Row[1].y - Row[0].x - Row[2].z) * 2;
            rotation.w = (Row[0].z - Row[2].x) / s;
            rotation.x = (Row[0].y + Row[1].x) / s;
            rotation.y = 0.25f * s;
            rotation.z = (Row[1].z + Row[2].y) / s;
        }
        else
        {
            float s = Maths::Sqrt(1 + Row[2].z - Row[1].y - Row[0].x) * 2;
            rotation.w = (Row[1].x - Row[0].y) / s;
            rotation.x = (Row[0].z + Row[2].x) / s;
            rotation.y = (Row[1].z + Row[2].y) / s;
            rotation.z = 0.25f * s;
        }

        return true;
    }

    /*template <typename T>
    Mat4<T> Mat4<T>::Orthogonal()
    {

    }*/
#pragma endregion
#pragma region Methods

    template<typename T>
    constexpr bool Mat4<T>::IsZero() const noexcept
    {
        // Function can't be constexpr with a loop.

        for (unsigned int i = 0; i < 16; i++)
        {
            if (!Maths::Equals0(elements[i]))
            {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    constexpr bool Mat4<T>::IsIdentity() const noexcept
    {
        for (unsigned int i = 0; i < 16; i++)
        {
            if (i == 0 || i == 5 || i == 10 || i == 15)
            {
                if (!Maths::Equals(elements[i], 1))
                {
                    return false;
                }
            } else
            {
                if (!Maths::Equals0(elements[i]))
                {
                    return false;
                }
            }
        }
        return true;
    }

    template<typename T>
    constexpr bool Mat4<T>::IsEquals(const Mat4<T> &_other, T _epsilon) const noexcept
    {
        for (unsigned int i = 0; i < 16; i++)
        {
            if (!Maths::Equals(elements[i], _other.elements[i], _epsilon));
            return false;
        }
        return true;
    }

    template<typename T>
    T &Mat4<T>::At(unsigned int _index)
    {
        return elements[_index];
    }

    template<typename T>
    constexpr T &Mat4<T>::At(unsigned int _index) const
    {
        return elements[_index];

    }

    template<typename T>
    T &Mat4<T>::At(unsigned int _n, unsigned int _m)
    {
        return elements[_n * 4 + _m];

    }

    template<typename T>
    constexpr T &Mat4<T>::At(unsigned int _n, unsigned int _m) const
    {
        return elements[_n * 4 + _m];

    }

    template<typename T>
    Mat4<T> &Mat4<T>::Multiply(T _scale) noexcept
    {
        for (unsigned int i = 0; i < 16; i++)
            elements[i] *= _scale;

        return *this;
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::GetMultiplied(T _scale) const noexcept
    {
        return Mat4(elements[0] * _scale, elements[1] * _scale, elements[2] * _scale, elements[3] * _scale,
                    elements[4] * _scale, elements[5] * _scale, elements[6] * _scale, elements[7] * _scale,
                    elements[8] * _scale, elements[9] * _scale, elements[10] * _scale, elements[11] * _scale,
                    elements[12] * _scale, elements[13] * _scale, elements[14] * _scale, elements[15] * _scale);
    }

    template<typename T>
    Mat4<T> &Mat4<T>::Divide(T _scale) noexcept
    {
        for (unsigned int i = 0; i < 16; i++)
            elements[i] /= _scale;

        return *this;
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::GetDivided(T _scale) const noexcept
    {
        return Mat4(elements[0] / _scale, elements[1] / _scale, elements[2] / _scale, elements[3] / _scale,
                    elements[4] / _scale, elements[5] / _scale, elements[6] / _scale, elements[7] / _scale,
                    elements[8] / _scale, elements[9] / _scale, elements[10] / _scale, elements[11] / _scale,
                    elements[12] / _scale, elements[13] / _scale, elements[14] / _scale, elements[15] / _scale);
    }

    template<typename T>
    Mat4<T> &Mat4<T>::Transpose() noexcept
    {
        *this->elements = GetTransposed();

        return *this;
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::GetTransposed() const noexcept
    {
        Mat4 Transpose;
        Transpose.elements[0] = elements[0];
        Transpose.elements[1] = elements[4];
        Transpose.elements[2] = elements[8];
        Transpose.elements[3] = elements[12];

        Transpose.elements[4] = elements[1];
        Transpose.elements[5] = elements[5];
        Transpose.elements[6] = elements[9];
        Transpose.elements[7] = elements[13];

        Transpose.elements[8] = elements[2];
        Transpose.elements[9] = elements[6];
        Transpose.elements[10] = elements[10];
        Transpose.elements[11] = elements[14];

        Transpose.elements[12] = elements[3];
        Transpose.elements[13] = elements[7];
        Transpose.elements[14] = elements[11];
        Transpose.elements[15] = elements[15];

        return Transpose;
    }

    template<typename T>
    Mat4<T> Mat4<T>::CoMatrix() const
    {
        Mat4 coM;
        float temp1 = det_2(10, 15, 14, 11);
        float temp2 = det_2(14, 7, 6, 15);
        float temp3 = det_2(6, 11, 10, 7);

        // det_2 used multipled times could be stored in tempX for performance gains

        coM.elements[0] = (elements[5] * temp1 + elements[9] * temp2 + elements[13] * temp3);
        coM.elements[1] = -(elements[4] * temp1 + elements[8] * temp2 + elements[12] * temp3);
        coM.elements[2] = (elements[4] * det_2(9, 15, 13, 11) + elements[8] * det_2(13, 7, 5, 15) +
                           elements[12] * det_2(5, 11, 9, 7));
        coM.elements[3] = -(elements[4] * det_2(9, 14, 13, 10) + elements[8] * det_2(13, 6, 5, 14) +
                            elements[12] * det_2(5, 10, 9, 6));

        float temp4 = det_2(14, 3, 2, 15);
        float temp5 = det_2(2, 11, 10, 3);

        coM.elements[4] = -(elements[1] * temp1 + elements[9] * temp4 + elements[13] * temp5);
        coM.elements[5] = (elements[0] * temp1 + elements[8] * temp4 + elements[12] * temp5);
        coM.elements[6] = -(elements[0] * det_2(9, 15, 13, 11) + elements[8] * det_2(13, 3, 1, 15) +
                            elements[12] * det_2(1, 11, 9, 3));
        coM.elements[7] = (elements[0] * det_2(9, 14, 13, 10) + elements[8] * det_2(13, 2, 1, 14) +
                           elements[12] * det_2(1, 10, 9, 2));

        coM.elements[8] = (elements[1] * det_2(6, 15, 14, 7) + elements[5] * det_2(14, 3, 2, 15) +
                           elements[13] * det_2(2, 7, 6, 3));
        coM.elements[9] = -(elements[0] * det_2(6, 15, 14, 7) + elements[4] * det_2(14, 3, 2, 15) +
                            elements[12] * det_2(2, 7, 6, 3));
        coM.elements[10] = (elements[0] * det_2(5, 15, 13, 7) + elements[4] * det_2(13, 3, 1, 15) +
                            elements[12] * det_2(1, 7, 5, 3));
        coM.elements[11] = -(elements[0] * det_2(5, 14, 13, 6) + elements[4] * det_2(13, 2, 1, 14) +
                             elements[12] * det_2(1, 6, 5, 2));

        coM.elements[12] = -(elements[1] * det_2(6, 11, 10, 7) + elements[5] * det_2(10, 3, 2, 11) +
                             elements[9] * det_2(2, 7, 6, 3));
        coM.elements[13] = (elements[0] * det_2(6, 11, 10, 7) + elements[4] * det_2(10, 3, 2, 11) +
                            elements[8] * det_2(2, 7, 6, 3));
        coM.elements[14] = -(elements[0] * det_2(5, 11, 9, 7) + elements[4] * det_2(9, 3, 1, 11) +
                             elements[8] * det_2(1, 7, 5, 3));
        coM.elements[15] = (elements[0] * det_2(5, 10, 9, 6) + elements[4] * det_2(9, 2, 1, 10) +
                            elements[8] * det_2(1, 6, 5, 2));


        return coM;
    }

    template<typename T>
    Mat4<T> &Mat4<T>::Inverse() noexcept
    {
        elements = GetInversed();
        return *this;
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::GetInversed() const
    {
        Mat4 inverse;
        const Mat4 AdjM = CoMatrix().GetTransposed();

        inverse.elements[0] = AdjM.elements[0] / Determinant();
        inverse.elements[1] = AdjM.elements[1] / Determinant();
        inverse.elements[2] = AdjM.elements[2] / Determinant();
        inverse.elements[3] = AdjM.elements[3] / Determinant();

        inverse.elements[4] = AdjM.elements[4] / Determinant();
        inverse.elements[5] = AdjM.elements[5] / Determinant();
        inverse.elements[6] = AdjM.elements[6] / Determinant();
        inverse.elements[7] = AdjM.elements[7] / Determinant();

        inverse.elements[8] = AdjM.elements[8] / Determinant();
        inverse.elements[9] = AdjM.elements[9] / Determinant();
        inverse.elements[10] = AdjM.elements[10] / Determinant();
        inverse.elements[11] = AdjM.elements[11] / Determinant();

        inverse.elements[12] = AdjM.elements[12] / Determinant();
        inverse.elements[13] = AdjM.elements[13] / Determinant();
        inverse.elements[14] = AdjM.elements[14] / Determinant();
        inverse.elements[15] = AdjM.elements[15] / Determinant();

        return inverse;
    }

    template<typename T>
    constexpr float Mat4<T>::det_2(unsigned x, unsigned y, unsigned z, unsigned w) const noexcept
    {
        return (elements[x] * elements[y] - elements[z] * elements[w]);
    }

    template<typename T>
    constexpr float Mat4<T>::Determinant() const noexcept
    {
        return (elements[0] * (elements[5] * det_2(10, 15, 14, 11) + elements[9] * det_2(14, 7, 6, 15) +
                               elements[13] * det_2(6, 11, 10, 7)) -
                elements[1] * (elements[4] * det_2(10, 15, 14, 11) + elements[8] * det_2(14, 7, 6, 15) +
                               elements[12] * det_2(6, 11, 10, 7)) +
                elements[2] * (elements[4] * det_2(9, 15, 13, 11) + elements[8] * det_2(13, 7, 5, 15) +
                               elements[12] * det_2(5, 11, 9, 7)) -
                elements[3] * (elements[4] * det_2(9, 14, 13, 10) + elements[8] * det_2(13, 6, 5, 14) +
                               elements[12] * det_2(5, 10, 9, 6)));
    }


    template<typename T>
    const std::string Mat4<T>::ToString() const noexcept
    {
        std::string str = "\n";

        for (unsigned int i = 0; i < 4; i++)
        {
            for (unsigned int j = 0; j < 4; j++)
            {
                str += std::to_string(elements[i*4+j]) + "  ";
            }
            str += "\n";
        }

        return str;
    }
#pragma endregion
#pragma region Operator

    template<typename T>
    constexpr Mat4<T> Mat4<T>::operator*(T _scale) const noexcept
    {
        return GetMultiply(_scale);
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::operator/(T _scale) const noexcept
    {
        return GetDivide(_scale);
    }

    template<typename T>
    Mat4<T> &Mat4<T>::operator*=(T _scale) noexcept
    {
        Multiply(_scale);
        return *this;
    }

    template<typename T>
    Mat4<T> &Mat4<T>::operator/=(T _scale) noexcept
    {
        Divide(_scale);
        return *this;
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::operator+(const Mat4<T> &_mat) const noexcept
    {
        return Mat4<T>(
                elements[0] + _mat.elements[0],
                elements[1] + _mat.elements[1],
                elements[2] + _mat.elements[2],
                elements[3] + _mat.elements[3],

                elements[4] + _mat.elements[4],
                elements[5] + _mat.elements[5],
                elements[6] + _mat.elements[6],
                elements[7] + _mat.elements[7],

                elements[8] + _mat.elements[8],
                elements[9] + _mat.elements[9],
                elements[10] + _mat.elements[10],
                elements[11] + _mat.elements[11],

                elements[12] + _mat.elements[12],
                elements[13] + _mat.elements[13],
                elements[14] + _mat.elements[14],
                elements[15] + _mat.elements[15]
        );
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::operator-(const Mat4<T> &_mat) const noexcept
    {
        return Mat4<T>(
                elements[0] - _mat.elements[0],
                elements[1] - _mat.elements[1],
                elements[2] - _mat.elements[2],
                elements[3] - _mat.elements[3],

                elements[4] - _mat.elements[4],
                elements[5] - _mat.elements[5],
                elements[6] - _mat.elements[6],
                elements[7] - _mat.elements[7],

                elements[8] - _mat.elements[8],
                elements[9] - _mat.elements[9],
                elements[10] - _mat.elements[10],
                elements[11] - _mat.elements[11],

                elements[12] - _mat.elements[12],
                elements[13] - _mat.elements[13],
                elements[14] - _mat.elements[14],
                elements[15] - _mat.elements[15]
        );
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::operator*(const Mat4<T> &_mat) const noexcept
    {
        //TODO mAKE MULTIPLICATION LINE CONVENTION
        return Mat4<T>(
                elements[0] * _mat.elements[0] + elements[1] * _mat.elements[4] + elements[2] * _mat.elements[8] +
                elements[3] * _mat.elements[12],
                elements[0] * _mat.elements[1] + elements[1] * _mat.elements[5] + elements[2] * _mat.elements[9] +
                elements[3] * _mat.elements[13],
                elements[0] * _mat.elements[2] + elements[1] * _mat.elements[6] + elements[2] * _mat.elements[10] +
                elements[3] * _mat.elements[14],
                elements[0] * _mat.elements[3] + elements[1] * _mat.elements[7] + elements[2] * _mat.elements[11] +
                elements[3] * _mat.elements[15],

                elements[4] * _mat.elements[0] + elements[5] * _mat.elements[4] + elements[6] * _mat.elements[8] +
                elements[7] * _mat.elements[12],
                elements[4] * _mat.elements[1] + elements[5] * _mat.elements[5] + elements[6] * _mat.elements[9] +
                elements[7] * _mat.elements[13],
                elements[4] * _mat.elements[2] + elements[5] * _mat.elements[6] + elements[6] * _mat.elements[10] +
                elements[7] * _mat.elements[14],
                elements[4] * _mat.elements[3] + elements[5] * _mat.elements[7] + elements[6] * _mat.elements[11] +
                elements[7] * _mat.elements[15],

                elements[8] * _mat.elements[0] + elements[9] * _mat.elements[4] + elements[10] * _mat.elements[8] +
                elements[11] * _mat.elements[12],
                elements[8] * _mat.elements[1] + elements[9] * _mat.elements[5] + elements[10] * _mat.elements[9] +
                elements[11] * _mat.elements[13],
                elements[8] * _mat.elements[2] + elements[9] * _mat.elements[6] + elements[10] * _mat.elements[10] +
                elements[11] * _mat.elements[14],
                elements[8] * _mat.elements[3] + elements[9] * _mat.elements[7] + elements[10] * _mat.elements[11] +
                elements[11] * _mat.elements[15],

                elements[12] * _mat.elements[0] + elements[13] * _mat.elements[4] + elements[14] * _mat.elements[8] +
                elements[15] * _mat.elements[12],
                elements[12] * _mat.elements[1] + elements[13] * _mat.elements[5] + elements[14] * _mat.elements[9] +
                elements[15] * _mat.elements[13],
                elements[12] * _mat.elements[2] + elements[13] * _mat.elements[6] + elements[14] * _mat.elements[10] +
                elements[15] * _mat.elements[14],
                elements[12] * _mat.elements[3] + elements[13] * _mat.elements[7] + elements[14] * _mat.elements[11] +
                elements[15] * _mat.elements[15]
        );
    }

    template<typename T>
    constexpr Mat4<T> Mat4<T>::operator/(const Mat4<T> &_mat) const noexcept
    {
        return GetInversed();
    }

    template<typename T>
    Mat4<T> &Mat4<T>::operator+=(Mat4<T> &_mat) noexcept
    {
        return *this = *this + _mat;
    }

    template<typename T>
    Mat4<T> &Mat4<T>::operator-=(Mat4<T> &_mat) noexcept
    {
        return *this = *this - _mat;
    }

    template<typename T>
    Mat4<T> &Mat4<T>::operator*=(Mat4<T> &_mat) noexcept
    {
        return *this = *this * _mat;
    }

    template<typename T>
    Mat4<T> &Mat4<T>::operator/=(Mat4<T> &_mat) noexcept
    {
        return *this = *this / _mat;
    }

    template<typename T>
    T &Mat4<T>::operator[](unsigned int _index)
    {
        return At(_index);
    }

    template<typename T>
    constexpr Mat4<T> operator*(T _value, const Mat4<T> &_mat) noexcept
    {
        return _mat * _value;
    }


    template<typename T>
    constexpr Mat4<T> operator/(T _value, const Mat4<T> &_mat)
    {
        for(unsigned int i = 0; i < 16; i++)
        {
            if(Maths::Equals0(_mat[i]))
            {
                Log::Send("operator / Mat4: scale is null is impossible to divide by 0", Log::ELogSeverity::ERROR);
                return Mat4<T>::Identity;
            }
        }
        return Mat4<T>(
                      _value / _mat.elements[0],
                      _value / _mat.elements[1],
                      _value / _mat.elements[2],
                      _value / _mat.elements[3],
                      _value / _mat.elements[4],
                      _value / _mat.elements[5],
                      _value / _mat.elements[6],
                      _value / _mat.elements[7],
                      _value / _mat.elements[8],
                      _value / _mat.elements[9],
                      _value / _mat.elements[10],
                      _value / _mat.elements[11],
                      _value / _mat.elements[12],
                      _value / _mat.elements[13],
                      _value / _mat.elements[14],
                      _value / _mat.elements[15]
        );
    }


#pragma endregion

}
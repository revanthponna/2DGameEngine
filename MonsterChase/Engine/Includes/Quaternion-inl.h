#include <assert.h>
#include <math.h>

#include "Matrix4.h"
#include "Vector3.h"
#include "Utilities.h"

#include "Quaternion.h"

namespace Engine
{
	namespace Math
	{
		inline Quaternion::Quaternion(float i_w, float i_x, float i_y, float i_z) :
			m_w(i_w),
			m_x(i_x),
			m_y(i_y),
			m_z(i_z)
		{
			assert(!IsNAN(m_w));
			assert(!IsNAN(m_x));
			assert(!IsNAN(m_y));
			assert(!IsNAN(m_z));

		}

		inline Quaternion::Quaternion(float i_w, const Vector3& i_Vec) :
			m_w(i_w),
			m_x(i_Vec.x()),
			m_y(i_Vec.y()),
			m_z(i_Vec.z())
		{
			assert(!IsNAN(m_w));
			assert(!IsNAN(m_x));
			assert(!IsNAN(m_y));
			assert(!IsNAN(m_z));

		}

		inline Quaternion::Quaternion(const Quaternion& i_other) :
			m_w(i_other.m_w),
			m_x(i_other.m_x),
			m_y(i_other.m_y),
			m_z(i_other.m_z)
		{
			assert(!IsNAN(m_w));
			assert(!IsNAN(m_x));
			assert(!IsNAN(m_y));
			assert(!IsNAN(m_z));

		}

		inline Quaternion& Quaternion::operator=(const Quaternion& i_other)
		{
			assert(!IsNAN(i_other.m_w));
			assert(!IsNAN(i_other.m_x));
			assert(!IsNAN(i_other.m_y));
			assert(!IsNAN(i_other.m_z));

			m_w = i_other.m_w;
			m_x = i_other.m_x;
			m_y = i_other.m_y;
			m_z = i_other.m_z;

			return *this;
		}

		inline float Quaternion::real(void) const
		{
			return m_w;
		}

		inline Vector3 Quaternion::imag(void) const
		{
			return Vector3(m_x, m_y, m_z);
		}

		inline Quaternion& Quaternion::operator-(void)
		{
			m_x = -m_x;
			m_y = -m_y;
			m_z = -m_z;

			return *this;
		}

		inline Quaternion& Quaternion::operator*=(float i_other)
		{
			assert(!IsNAN(i_other));

			m_w *= i_other;
			m_x *= i_other;
			m_y *= i_other;
			m_z *= i_other;

			return *this;
		}

		inline Quaternion& Quaternion::operator/=(float i_other)
		{
			assert(!IsNAN(i_other));
			assert(!IsZero(i_other));

			m_w /= i_other;
			m_x /= i_other;
			m_y /= i_other;
			m_z /= i_other;

			return *this;
		}

		inline Quaternion& Quaternion::operator+=(const Quaternion& i_other)
		{
			m_w += i_other.m_w;
			m_x += i_other.m_x;
			m_y += i_other.m_y;
			m_z += i_other.m_z;

			return *this;
		}

		inline Quaternion& Quaternion::operator-=(const Quaternion& i_other)
		{
			m_w -= i_other.m_w;
			m_x -= i_other.m_x;
			m_y -= i_other.m_y;
			m_z -= i_other.m_z;

			return *this;
		}

		inline float Quaternion::dot(const Quaternion& i_other)
		{
			return m_w * i_other.m_w + m_x * i_other.m_x + m_y * i_other.m_y + m_z * i_other.m_z;
		}

		inline float Quaternion::Magnitude(void) const
		{
			return sqrtf(m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z);
		}

		inline Quaternion Quaternion::CreateRotation(const Vector3& i_Axis, float i_Rotation)
		{
			assert(!IsNAN(i_Rotation));

			float sin = sinf(i_Rotation * 0.5f);
			float cos = cosf(i_Rotation * 0.5f);

			return Quaternion(cos, i_Axis.x() * sin, i_Axis.y() * sin, i_Axis.z() * sin);

		}

		inline Quaternion operator*(const Quaternion& i_lhs, float i_rhs)
		{
			assert(!IsNAN(i_rhs));

			return Quaternion(i_lhs.real() * i_rhs, i_lhs.imag().x() * i_rhs, i_lhs.imag().y() * i_rhs, i_lhs.imag().z() * i_rhs);
		}

		inline Quaternion operator*(const float i_lhs, const Quaternion& i_rhs)
		{
			assert(!IsNAN(i_lhs));

			return Quaternion(i_rhs.real() * i_lhs, i_rhs.imag().x() * i_lhs, i_rhs.imag().y() * i_lhs, i_rhs.imag().z() * i_lhs);
		}

		inline Quaternion operator/(const Quaternion& i_lhs, float i_rhs)
		{
			assert(!IsNAN(i_rhs));
			assert(!IsZero(i_rhs));

			return Quaternion(i_lhs.real() / i_rhs, i_lhs.imag().x() / i_rhs, i_lhs.imag().y() / i_rhs, i_lhs.imag().z() / i_rhs);
		}

		inline Quaternion operator+(const Quaternion& i_lhs, const Quaternion& i_rhs)
		{
			return Quaternion(i_lhs.real() + i_rhs.real(),
				i_lhs.imag().x() + i_rhs.imag().x(), i_lhs.imag().y() + i_rhs.imag().y(), i_lhs.imag().z() + i_rhs.imag().z());
		}

		inline Quaternion operator-(const Quaternion& i_lhs, const Quaternion& i_rhs)
		{
			return Quaternion(i_lhs.real() - i_rhs.real(), i_lhs.imag().x() - i_rhs.imag().x(), i_lhs.imag().y() - i_rhs.imag().y(), i_lhs.imag().z() - i_rhs.imag().z());
		}

		inline float dot(const Quaternion& i_lhs, const Quaternion& i_rhs)
		{
			return i_lhs.real() * i_rhs.real() + i_lhs.imag().x() * i_lhs.imag().x() + i_rhs.imag().y() * i_rhs.imag().y() + i_lhs.imag().z() * i_rhs.imag().z();
		}
	} // namespace Math
} // namespace Engine
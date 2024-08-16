#pragma once

#include "Physics.hpp"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"


struct AABB
{
	Engine::Math::Vector3	m_Center;
	Engine::Math::Vector3	m_Extents;
};

class Object
{

public:

	~Object() noexcept {};

	Engine::Math::Vector3	m_Position;
	Engine::Math::Vector3	m_Velocity;
	Engine::Math::Matrix3	m_Orientation;

	AABB					m_BB;
};

struct CollisionPair
{
	Object* m_pObjects[2];
	float			m_CollisionTime;
};

class CollisionSystem {

public:

	static	void						SingleCollisionCheck			() noexcept;

	static	CollisionPair 				FindCollision					(float i_dt) noexcept;

	static	void						InitTestObjects					() noexcept;

	static	bool						DetectCrossTimes				(float i_Center, float i_Extent, float i_Point, float i_Travel, float& o_tEnter, float& o_tExit) noexcept;

	static	bool						Check2DCollision				(const Object& i_Obj1, const Object& i_Obj2, float i_dt, float& i_tCollision, float& o_) noexcept;

	static	bool						CheckSeparationAlongAxis		(const Engine::Math::Vector3& i_CollisionAxis, const Object& i_Obj1, const Engine::Math::Vector3& i_Obj1BBCenterInWorld, const Object& i_Obj2,
		                                                                 const Engine::Math::Vector3& i_Obj2BBCenterInWorld, float i_dt, float& i_tEnter, float& i_tExit) noexcept;

	static	bool						CheckCollision					(const Object& i_Obj1, const Object& i_Obj2, float i_dt, float& o_tCollision) noexcept;

	static  void						ClearAll						() noexcept;

private:

	static	std::vector<Object*>	m_CollisionObjects;

};

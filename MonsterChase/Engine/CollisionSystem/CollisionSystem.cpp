#pragma once

#include "CollisionSystem.hpp"
#include <stdio.h>
#include <Physics.hpp>
#include <Console/ConsolePrint.h>

std::vector<Object*>	CollisionSystem::m_CollisionObjects{};

float RandInRange(float i_Min, float i_Max) noexcept
{
	return i_Min + (static_cast<float>(rand()) / (RAND_MAX + 1) * (i_Max - i_Min + 1.0f));
}

Engine::Math::Vector3 GenerateRandomVector(const Engine::Math::Vector3& i_Min, const Engine::Math::Vector3& i_Max) noexcept
{
	return Engine::Math::Vector3(
		RandInRange(i_Min.x(), i_Max.x()),
		RandInRange(i_Min.y(), i_Max.y()),
		RandInRange(i_Min.z(), i_Max.z()));
}

void 
CollisionSystem::SingleCollisionCheck () noexcept
{
	using namespace Engine::Math;

	Matrix4 mRot = Matrix4::CreateZRotationCV(DegreesToRadians(90.0f));
	Matrix4 mTrans = Matrix4::CreateTranslationCV(0.0f, 100.0f, 0.0f);
	Matrix4 mToWorld = mTrans * mRot;

	Vector4 VXAxis(Vector3(1.0f, 0.0f, 0.0f), 1.0f);

	Vector4 VWorld = mToWorld * VXAxis;

	Matrix4 mFromWorld = mToWorld.GetInverseRotTransCV();
	Vector4 VBack = mFromWorld * VWorld;

	Object	Obj1;

	Obj1.m_Position = Vector3(-1.0f, 0.0f, 0.0f);
	Obj1.m_Velocity = Vector3(1.0f, 0.0f, 0.0f);
	Obj1.m_Orientation = Matrix3::CreateZRotationCV(45.0f);
	Obj1.m_BB.m_Center = Vector3(0.0f, 0.0f, 0.0f);
	Obj1.m_BB.m_Extents = Vector3(0.5f, 0.5f, 0.5f);

	Object	Obj2;

	Obj2.m_Position = Vector3(1.0f, 0.0f, 0.0f);
	Obj2.m_Velocity = Vector3(-1.0f, 0.0f, 0.0f);
	Obj2.m_Orientation = Matrix3::CreateXRotationCV(45.0f);
	Obj2.m_BB.m_Center = Vector3(0.0f, 0.0f, 0.0f);
	Obj2.m_BB.m_Extents = Vector3(0.5f, 0.5f, 0.5f);

	float tCollision;

	bool bCollided = CheckCollision(Obj1, Obj2, 1.0f, tCollision);

	printf("Collided: %s\n", bCollided ? "true" : "false");
}

void
CollisionSystem::InitTestObjects () noexcept
{
	using namespace Engine::Math;

	const float AreaExtent = 10.0f;
	Vector3 PositionMin(-AreaExtent, -AreaExtent, -AreaExtent);
	Vector3 PositionMax(AreaExtent, AreaExtent, AreaExtent);

	Vector3 VelocityMin(-1.0f, -1.0f, -1.0f);
	Vector3 VelocityMax(1.0f, 1.0f, 1.0f);

	const size_t numObjects = 10;

	for (size_t i = 0; i < numObjects; i++)
	{
		Object* pObj = new Object;

		pObj->m_Position = GenerateRandomVector(PositionMin, PositionMax);
		pObj->m_Velocity = GenerateRandomVector(VelocityMin, VelocityMax);

		switch (rand() & 0x03)
		{
		case 01:
			pObj->m_Orientation = Matrix3::CreateXRotationCV(RandInRange(0.0f, 90.0f));
			break;
		case 02:
			pObj->m_Orientation = Matrix3::CreateYRotationCV(RandInRange(0.0f, 90.0f));
			break;
		case 03:
			pObj->m_Orientation = Matrix3::CreateZRotationCV(RandInRange(0.0f, 90.0f));
			break;
		default:
			pObj->m_Orientation = Matrix3::Identity;
			break;
		}

		pObj->m_BB.m_Center = Vector3(0.0f, 0.0f, 0.0f);
		pObj->m_BB.m_Extents = Vector3(0.5f, 0.5f, 0.5f);

		m_CollisionObjects.push_back(pObj);
	}
}

CollisionPair 
CollisionSystem::FindCollision(float i_dt) noexcept
{
	CollisionPair CurrentCollision = { { 0, 0 }, 0.0f };

	const size_t count = m_CollisionObjects.size();

	for (size_t i = 0; i < (count - 1); i++)
	{
		for (size_t j = i + 1; j < count; j++)
		{
			assert(m_CollisionObjects[i]);
			assert(m_CollisionObjects[j]);

			float tCollision = 0.0f;

			if (CheckCollision(*m_CollisionObjects[i], *m_CollisionObjects[j], i_dt, tCollision))
			{
				if (CurrentCollision.m_pObjects[0])
				{
					if (tCollision < CurrentCollision.m_CollisionTime)
					{
						CurrentCollision.m_pObjects[0] = m_CollisionObjects[i];
						CurrentCollision.m_pObjects[1] = m_CollisionObjects[j];
						CurrentCollision.m_CollisionTime = tCollision;
					}
				}
				else
				{
					CurrentCollision.m_pObjects[0] = m_CollisionObjects[i];
					CurrentCollision.m_pObjects[1] = m_CollisionObjects[j];
					CurrentCollision.m_CollisionTime = tCollision;
				}
			}
		}
	}
	return CurrentCollision;
}

bool 
CollisionSystem::DetectCrossTimes(float i_Center, float i_Extent, float i_Point, float i_Travel, float& o_tEnter, float& o_tExit) noexcept
{
	// Extent must be a positive scalar
	assert(i_Extent > 0.0f);

	using namespace Engine::Math;

	// calculate left (assumed) and right extents 
	float i_Start = i_Center - i_Extent;
	float i_End = i_Center + i_Extent;

	// handle case where comparator is not moving along this axis
	if (IsZero(i_Travel))
	{
		// if check point is outside of extents it will never be in collision state
		// otherwise it will always be in collision state
		if ((i_Point < i_Start) || (i_Point > i_End))
			return false;
		else
		{
			o_tEnter = 0.0f;
			o_tExit = 1.0f;
			return true;
		}
	}

	// calculate time to enter collision state and time to exit collision state
	o_tEnter = (i_Start - i_Point) / i_Travel;
	o_tExit = (i_End - i_Point) / i_Travel;

	// if time to enter collision state is AFTER time to exit collision state we need to reverse them
	// as we assumed comparator was on left (more negative) side and moving in a positive direction.
	// reversing the values fixes this
	if (o_tEnter > o_tExit)
		Swap(o_tEnter, o_tExit);

	return !((o_tEnter >= 1.0f) || (o_tExit <= 0.0f));
}

bool 
CollisionSystem::Check2DCollision(const Object& i_Obj1, const Object& i_Obj2, float i_dt, float& i_tCollision, float& o_) noexcept
{
	using namespace Engine::Math;

	Matrix4 Obj1ToWorld = Matrix4::CreateTranslationCV(i_Obj1.m_Position) * i_Obj1.m_Orientation.ToTransform();
	Matrix4 Obj2ToWorld = Matrix4::CreateTranslationCV(i_Obj2.m_Position) * i_Obj2.m_Orientation.ToTransform();

	Vector3 Obj1BBCenterInWorld = Obj1ToWorld.TransformPointCV(i_Obj1.m_BB.m_Center);
	Vector3 Obj2BBCenterInWorld = Obj2ToWorld.TransformPointCV(i_Obj2.m_BB.m_Center);

	float tEnter = 0.0f;
	float tExit = 1.0f;

	// Obj2 vs Obj1
	Vector3 Obj2RelVel = i_Obj2.m_Velocity - i_Obj1.m_Velocity;
	Vector3 Obj2Travel = Obj2RelVel * i_dt;

	// Obj1 X Axis
	{
		Vector3 Obj1XAxisInWorld = i_Obj1.m_Orientation.Col(0).Normalized();

		float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj1XAxisInWorld);
		float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj1XAxisInWorld);

		float Obj2ProjectedExtents = float(
			fabs(dot(Obj1XAxisInWorld, i_Obj2.m_Orientation.Col(0).Normalized() * i_Obj2.m_BB.m_Extents.x())) +
			fabs(dot(Obj1XAxisInWorld, i_Obj2.m_Orientation.Col(1).Normalized() * i_Obj2.m_BB.m_Extents.y())) +
			fabs(dot(Obj1XAxisInWorld, i_Obj2.m_Orientation.Col(2).Normalized() * i_Obj2.m_BB.m_Extents.z()))
			);

		float Obj1ExpandedExtents = i_Obj1.m_BB.m_Extents.x() + Obj2ProjectedExtents;

		float Obj2TravelAlongAxis = dot(Obj2Travel, Obj1XAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj1 Y Axis
	{
		Vector3 Obj1YAxisInWorld = i_Obj1.m_Orientation.Col(1).Normalized();

		float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj1YAxisInWorld);
		float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj1YAxisInWorld);

		float Obj2ProjectedExtents = float(
			fabs(dot(Obj1YAxisInWorld, i_Obj2.m_Orientation.Col(0).Normalized() * i_Obj2.m_BB.m_Extents.x())) +
			fabs(dot(Obj1YAxisInWorld, i_Obj2.m_Orientation.Col(1).Normalized() * i_Obj2.m_BB.m_Extents.y())) +
			fabs(dot(Obj1YAxisInWorld, i_Obj2.m_Orientation.Col(2).Normalized() * i_Obj2.m_BB.m_Extents.z()))
			);

		float Obj1ExpandedExtents = i_Obj1.m_BB.m_Extents.y() + Obj2ProjectedExtents;

		float Obj2TravelAlongAxis = dot(Obj2Travel, Obj1YAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj1 Z Axis
	{
		Vector3 Obj1ZAxisInWorld = i_Obj1.m_Orientation.Col(2).Normalized();

		float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj1ZAxisInWorld);
		float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj1ZAxisInWorld);

		float Obj2ProjectedExtents = float(
			fabs(dot(Obj1ZAxisInWorld, i_Obj2.m_Orientation.Col(0).Normalized() * i_Obj2.m_BB.m_Extents.x())) +
			fabs(dot(Obj1ZAxisInWorld, i_Obj2.m_Orientation.Col(1).Normalized() * i_Obj2.m_BB.m_Extents.y())) +
			fabs(dot(Obj1ZAxisInWorld, i_Obj2.m_Orientation.Col(2).Normalized() * i_Obj2.m_BB.m_Extents.z()))
			);

		float Obj1ExpandedExtents = i_Obj1.m_BB.m_Extents.z() + Obj2ProjectedExtents;

		float Obj2TravelAlongAxis = dot(Obj2Travel, Obj1ZAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj1 vs Obj2
	Vector3 Obj1RelVel = i_Obj1.m_Velocity - i_Obj2.m_Velocity;
	Vector3 Obj1Travel = Obj1RelVel * i_dt;

	// Obj2 X Axis
	{
		Vector3 Obj2XAxisInWorld = i_Obj2.m_Orientation.Col(0).Normalized();

		float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj2XAxisInWorld);
		float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj2XAxisInWorld);

		float Obj1ProjectedExtents = float(
			fabs(dot(Obj2XAxisInWorld, i_Obj1.m_Orientation.Col(0).Normalized() * i_Obj1.m_BB.m_Extents.x())) +
			fabs(dot(Obj2XAxisInWorld, i_Obj1.m_Orientation.Col(1).Normalized() * i_Obj1.m_BB.m_Extents.y())) +
			fabs(dot(Obj2XAxisInWorld, i_Obj1.m_Orientation.Col(2).Normalized() * i_Obj1.m_BB.m_Extents.z()))
			);

		float Obj2ExpandedExtents = i_Obj2.m_BB.m_Extents.x() + Obj1ProjectedExtents;

		float Obj1TravelAlongAxis = dot(Obj1Travel, Obj2XAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj2 Y Axis
	{
		Vector3 Obj2YAxisInWorld = i_Obj2.m_Orientation.Col(1).Normalized();

		float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj2YAxisInWorld);
		float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj2YAxisInWorld);

		float Obj1ProjectedExtents = float(
			fabs(dot(Obj2YAxisInWorld, i_Obj1.m_Orientation.Col(0).Normalized() * i_Obj1.m_BB.m_Extents.x())) +
			fabs(dot(Obj2YAxisInWorld, i_Obj1.m_Orientation.Col(1).Normalized() * i_Obj1.m_BB.m_Extents.y())) +
			fabs(dot(Obj2YAxisInWorld, i_Obj1.m_Orientation.Col(2).Normalized() * i_Obj1.m_BB.m_Extents.z()))
			);

		float Obj2ExpandedExtents = i_Obj2.m_BB.m_Extents.y() + Obj1ProjectedExtents;

		float Obj1TravelAlongAxis = dot(Obj1Travel, Obj2YAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj2 Z Axis
	{
		Vector3 Obj2ZAxisInWorld = i_Obj2.m_Orientation.Col(2).Normalized();

		float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj2ZAxisInWorld);
		float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj2ZAxisInWorld);

		float Obj1ProjectedExtents = float(
			fabs(dot(Obj2ZAxisInWorld, i_Obj1.m_Orientation.Col(0).Normalized() * i_Obj1.m_BB.m_Extents.x())) +
			fabs(dot(Obj2ZAxisInWorld, i_Obj1.m_Orientation.Col(1).Normalized() * i_Obj1.m_BB.m_Extents.y())) +
			fabs(dot(Obj2ZAxisInWorld, i_Obj1.m_Orientation.Col(2).Normalized() * i_Obj1.m_BB.m_Extents.z()))
			);

		float Obj2ExpandedExtents = i_Obj2.m_BB.m_Extents.z() + Obj1ProjectedExtents;

		float Obj1TravelAlongAxis = dot(Obj1Travel, Obj2ZAxisInWorld);

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	// Obj1X x Obj2X
	{
		Vector3 Obj1XAxisInWorld = i_Obj1.m_Orientation.Col(0);
		Vector3 Obj2XAxisInWorld = i_Obj2.m_Orientation.Col(0);

		Vector3 CollisionAxis = cross(Obj1XAxisInWorld, Obj2XAxisInWorld).Normalized();

		float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, CollisionAxis);
		float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, CollisionAxis);

		float Obj1ProjectedExtents = float(
			fabs(dot(CollisionAxis, i_Obj1.m_Orientation.Col(0).Normalized() * i_Obj1.m_BB.m_Extents.x())) +
			fabs(dot(CollisionAxis, i_Obj1.m_Orientation.Col(1).Normalized() * i_Obj1.m_BB.m_Extents.y())) +
			fabs(dot(CollisionAxis, i_Obj1.m_Orientation.Col(2).Normalized() * i_Obj1.m_BB.m_Extents.z()))
			);

		float Obj2ProjectedExtents = float(
			fabs(dot(CollisionAxis, i_Obj2.m_Orientation.Col(0).Normalized() * i_Obj2.m_BB.m_Extents.x())) +
			fabs(dot(CollisionAxis, i_Obj2.m_Orientation.Col(1).Normalized() * i_Obj2.m_BB.m_Extents.y())) +
			fabs(dot(CollisionAxis, i_Obj2.m_Orientation.Col(2).Normalized() * i_Obj2.m_BB.m_Extents.z()))
			);

		float Obj2ExpandedExtents = Obj2ProjectedExtents + Obj1ProjectedExtents;

		float Obj1VelOnAxis = dot(i_Obj1.m_Velocity, CollisionAxis);
		float Obj2VelOnAxis = dot(i_Obj2.m_Velocity, CollisionAxis);

		float Obj1RelVel = Obj1VelOnAxis - Obj2VelOnAxis;

		float Obj1TravelAlongAxis = Obj1RelVel * i_dt;

		float axisEnter = 0.0f;
		float axisExit = 1.0f;

		if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
			return false;

		if (axisEnter > tEnter)
			tEnter = axisEnter;
		if (axisExit < tExit)
			tExit = axisExit;
	}

	return tEnter < tExit;
}

bool 
CollisionSystem::CheckSeparationAlongAxis(const Engine::Math::Vector3& i_CollisionAxis, const Object& i_Obj1, const Engine::Math::Vector3& i_Obj1BBCenterInWorld,
	const Object& i_Obj2, const Engine::Math::Vector3& i_Obj2BBCenterInWorld, float i_dt,
	float& i_tEnter, float& i_tExit) noexcept
{
	using namespace Engine::Math;

	float Obj1BBCenterOnAxis = dot(i_Obj1BBCenterInWorld, i_CollisionAxis);
	float Obj2BBCenterOnAxis = dot(i_Obj2BBCenterInWorld, i_CollisionAxis);

	float Obj1ProjectedExtents = float(
		fabs(dot(i_CollisionAxis, i_Obj1.m_Orientation.Col(0).Normalized() * i_Obj1.m_BB.m_Extents.x())) +
		fabs(dot(i_CollisionAxis, i_Obj1.m_Orientation.Col(1).Normalized() * i_Obj1.m_BB.m_Extents.y())) +
		fabs(dot(i_CollisionAxis, i_Obj1.m_Orientation.Col(2).Normalized() * i_Obj1.m_BB.m_Extents.z()))
		);


	float Obj2ProjectedExtents = float(
		fabs(dot(i_CollisionAxis, i_Obj2.m_Orientation.Col(0).Normalized() * i_Obj2.m_BB.m_Extents.x())) +
		fabs(dot(i_CollisionAxis, i_Obj2.m_Orientation.Col(1).Normalized() * i_Obj2.m_BB.m_Extents.y())) +
		fabs(dot(i_CollisionAxis, i_Obj2.m_Orientation.Col(2).Normalized() * i_Obj2.m_BB.m_Extents.z()))
		);

	float Obj2ExpandedExtents = Obj2ProjectedExtents + Obj1ProjectedExtents;

	float Obj1VelOnAxis = dot(i_Obj1.m_Velocity, i_CollisionAxis);
	float Obj2VelOnAxis = dot(i_Obj2.m_Velocity, i_CollisionAxis);

	float Obj1RelVel = Obj1VelOnAxis - Obj2VelOnAxis;

	float Obj1TravelAlongAxis = Obj1RelVel * i_dt;

	i_tEnter = 0.0f;
	i_tExit = 1.0f;

	return !DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, i_tEnter, i_tExit);
}

bool 
CollisionSystem::CheckCollision(const Object& i_Obj1, const Object& i_Obj2, float i_dt, float& o_tCollision) noexcept
{
	using namespace Engine::Math;

	Matrix4 Obj1ToWorld = Matrix4::CreateTranslationCV(i_Obj1.m_Position) * i_Obj1.m_Orientation.ToTransform();
	Matrix4 Obj2ToWorld = Matrix4::CreateTranslationCV(i_Obj2.m_Position) * i_Obj2.m_Orientation.ToTransform();

	Vector3 Obj1BBCenterInWorld = Obj1ToWorld.TransformPointCV(i_Obj1.m_BB.m_Center);
	Vector3 Obj2BBCenterInWorld = Obj2ToWorld.TransformPointCV(i_Obj2.m_BB.m_Center);

	float tEnter = 0.0f;
	float tExit = 1.0f;

	float axisEnter = 0.0f;
	float axisExit = 1.0f;

	Vector3 CollisionAxis;

	CollisionAxis = i_Obj1.m_Orientation.Col(0).Normalized();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj1 X Axis");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj1 X Axis from %f to %f", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.Col(1).Normalized();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj1 Y Axis");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj1 Y Axis from %f to %f", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj1.m_Orientation.Col(2).Normalized();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj1 Z Axis");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj1 Z Axis from %f to %f", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj2.m_Orientation.Col(0).Normalized();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj2, Obj2BBCenterInWorld, i_Obj1, Obj1BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj2 X Axis");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj2 X Axis from %f to %f", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj2.m_Orientation.Col(1).Normalized();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj2, Obj2BBCenterInWorld, i_Obj1, Obj1BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj2 Y Axis");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj2 Y Axis from %f to %f", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = i_Obj2.m_Orientation.Col(2).Normalized();
	if (CheckSeparationAlongAxis(CollisionAxis, i_Obj2, Obj2BBCenterInWorld, i_Obj1, Obj1BBCenterInWorld, i_dt, axisEnter, axisExit))
	{
		DEBUG_PRINT("Found Separation along Obj1 Z Axis");
		return false;
	}
	else
	{
		DEBUG_PRINT("Found Overlap along Obj1 Z Axis from %f to %f", axisEnter, axisExit);
		if (axisEnter > tEnter)
		{
			DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
			tEnter = axisEnter;
		}
		if (axisExit < tExit)
		{
			DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
			tExit = axisExit;
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(0).Normalized(), i_Obj2.m_Orientation.Col(0).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 X x Obj2 X Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 X x Obj2 X Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(0).Normalized(), i_Obj2.m_Orientation.Col(1).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 X x Obj2 Y Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 X x Obj2 Y Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(0).Normalized(), i_Obj2.m_Orientation.Col(2).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 X x Obj2 Z Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 X x Obj2 Z Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(1).Normalized(), i_Obj2.m_Orientation.Col(0).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Y x Obj2 X Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Y x Obj2 X Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(1).Normalized(), i_Obj2.m_Orientation.Col(1).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Y x Obj2 Y Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Y x Obj2 Y Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(1).Normalized(), i_Obj2.m_Orientation.Col(2).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Y x Obj2 Z Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Y x Obj2 Z Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(2).Normalized(), i_Obj2.m_Orientation.Col(0).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Z x Obj2 X Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Z x Obj2 X Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(2).Normalized(), i_Obj2.m_Orientation.Col(1).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Z x Obj2 Y Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Z x Obj2 Y Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	CollisionAxis = cross(i_Obj1.m_Orientation.Col(2).Normalized(), i_Obj2.m_Orientation.Col(2).Normalized());
	if (!IsZero(CollisionAxis.LengthSq()))
	{
		if (CheckSeparationAlongAxis(CollisionAxis, i_Obj1, Obj1BBCenterInWorld, i_Obj2, Obj2BBCenterInWorld, i_dt, axisEnter, axisExit))
		{
			DEBUG_PRINT("Found Separation along Obj1 Z x Obj2 Z Axis");
			return false;
		}
		else
		{
			DEBUG_PRINT("Found Overlap along Obj1 Z x Obj2 Z Axis from %f to %f", axisEnter, axisExit);
			if (axisEnter > tEnter)
			{
				DEBUG_PRINT("Updating tEnter to %f\n", axisEnter);
				tEnter = axisEnter;
			}
			if (axisExit < tExit)
			{
				DEBUG_PRINT("Updating tExit to %f\n", axisEnter);
				tExit = axisExit;
			}
		}
	}

	if (tEnter < tExit)
	{
		o_tCollision = tEnter;
		return true;
	}
	else
		return false;
}

void 
CollisionSystem::ClearAll() noexcept
{
	size_t size;

	size = m_CollisionObjects.size();

	for (size_t ndx = 0; size > ndx; ++ndx)
		delete (m_CollisionObjects[ndx]);

	m_CollisionObjects.clear();
}


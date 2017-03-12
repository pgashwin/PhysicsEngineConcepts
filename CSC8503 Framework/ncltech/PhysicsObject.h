/******************************************************************************
Class: PhysicsObject
Implements:
Author: Rich Davison	<richard.davison4@newcastle.ac.uk>, Pieran Marris<p.marris@newcastle.ac.uk>
Description: This class represents the physical properties of your game's
entities - their position, orientation, mass, collision volume, and so on.

		(\_/)								-_-_-_-_-_-_-_,------,
		( '_')								_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
	 /""""""""""""\=========     -----D		-_-_-_-_-_-_-~|__( ^ .^) /
	/"""""""""""""""""""""""\				_-_-_-_-_-_-_-""  ""
....\_@____@____@____@____@_/			

*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <nclgl\Quaternion.h>
#include <nclgl\Matrix3.h>
#include "CollisionShape.h"
#include <functional>

class PhysicsEngine;

class PhysicsObject
{
	friend class PhysicsEngine;
	friend class GameObject; //Just to allow construction 

public:
	PhysicsObject();
	~PhysicsObject();

	//<--------- GETTERS ------------->
	bool				IsEnabled()				const 	{ return m_Enabled; }

	float				GetFriction()			const 	{ return m_Friction; }

	const Vector3&		GetPosition()			const 	{ return m_Position; }
	const Vector3&		GetLinearVelocity()		const 	{ return m_LinearVelocity; }
	const Vector3&		GetForce()				const 	{ return m_Force; }
	float				GetInverseMass()		const 	{ return m_InvMass; }

	const Quaternion&	GetOrientation()		const 	{ return m_Orientation; }
	const Vector3&		GetAngularVelocity()	const 	{ return m_AngularVelocity; }
	const Vector3&		GetTorque()				const 	{ return m_Torque; }
	const Matrix3&		GetInverseInertia()		const 	{ return m_InvInertia; }

	CollisionShape*		GetCollisionShape()		const 	{ return m_colShape; }

	const Matrix4& GetWorldSpaceTransform()     const;

	



	//<--------- SETTERS ------------->
	void SetFriction(float friction)				{ m_Friction = friction; }

	void SetPosition(const Vector3& v)				{ m_Position = v;	m_wsTransformInvalidated = true; }
	void SetLinearVelocity(const Vector3& v)		{ m_LinearVelocity = v; }
	void SetForce(const Vector3& v)					{ m_Force = v; }
	void SetInverseMass(const float& v)				{ m_InvMass = v; }

	void SetOrientation(const Quaternion& v)		{ m_Orientation = v; m_wsTransformInvalidated = true; }
	void SetAngularVelocity(const Vector3& v)		{ m_AngularVelocity = v; }
	void SetTorque(const Vector3& v)				{ m_Torque = v; }
	void SetInverseInertia(const Matrix3& v)		{ m_InvInertia = v; }
	
	void SetCollisionShape(CollisionShape* colShape)	{ m_colShape = colShape; }

	//rest state
	bool	rest;
	bool restState()						{ return rest; }
	void SettoRest(bool paused)		    	{ rest = paused; }
	//
	bool scored()						{ return m_scored; }
	void Setscore(bool q)			{ m_scored = q; }
	


	void SetOnCollisionCallback(std::function<bool(PhysicsObject*)> onCollisionFunction) { m_OnCollisionCallback = onCollisionFunction; }

protected:
	GameObject*			m_Parent;

	bool m_scored=false;

	bool				m_Enabled;

	mutable bool		m_wsTransformInvalidated;
	mutable Matrix4		m_wsTransform;

	float				m_Friction;

	//<---------LINEAR-------------->
	Vector3		m_Position;
	Vector3		m_LinearVelocity;
	Vector3		m_Force;
	float		m_InvMass;

	//<----------ANGULAR-------------->
	Quaternion  m_Orientation;
	Vector3		m_AngularVelocity;
	Vector3		m_Torque;
	Matrix3     m_InvInertia;

	//<----------COLLISION------------>
	CollisionShape* m_colShape;
	std::function<bool(PhysicsObject*)> m_OnCollisionCallback;  //Returns true to process the collision, false to ignore
};
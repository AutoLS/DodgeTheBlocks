#ifndef PHYSICS_H
#define PHYSICS_H

#include "polygon.h"

struct rigid_body
{
	polygon Shape;
	
	v2 Force;
	real32 Mass;
	
	v2 Pos;
	v2 Vel;
	v2 Dim;
	
	real32 Angle;
	real32 AngleVel;
};

rigid_body InitBody(v2 Pos, v2 Vel, v2 Dim,
					real32 Mass, real32 Angle, 
					polygon_type Type)
{
	rigid_body Result = {};
	Result.Mass = (Mass == 0) ? 10 : Mass;
	Result.Pos = Pos;
	Result.Vel = Vel;
	Result.Dim = Dim;
	Result.Angle = Angle;
	Result.Shape = CreatePolygon(Pos, Dim, Angle, Type);
	
	return Result;
}

void IntegrateLinearMotion(rigid_body* Body, real32 t, real32 Drag = 0)
{
	v2 Accel = Body->Force / Body->Mass;
	Accel -= Body->Vel * Drag;
	Body->Vel += Accel * t;
	
	v2 Delta = {};
	Delta += Body->Vel * t;
	
	Body->Pos += Delta;
	UpdateVertices(&Body->Shape, Delta);
}

void SetBodyPosition(rigid_body* Body, v2 NewPos)
{
	v2 Delta = NewPos - Body->Pos;
	
	Body->Pos = NewPos;
	UpdateVertices(&Body->Shape, Delta);
}

#endif
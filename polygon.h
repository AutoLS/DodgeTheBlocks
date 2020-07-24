#ifndef POLYGON_H
#define POLYGON_H

enum polygon_type
{
	POLYGON_TRIANGLE,
	POLYGON_RECTANGLE,
	POLYGON_MAX,
};

struct polygon
{
	v2 Vertices[36];
	
	int Count;
	
	v2 Pos;
	v2 Dim;
	
	real32 Angle;
	polygon_type Type;
};

void RotateVertices(polygon* Shape, real32 Theta);

polygon CreatePolygon(v2 Pos, v2 Dim, real32 Angle, polygon_type Type)
{
	polygon Polygon = {};
	Polygon.Type = Type;
	Polygon.Pos = Pos;
	Polygon.Dim = Dim;
	
	switch(Type)
	{
		case POLYGON_TRIANGLE:
		{
			Polygon.Count = 3;
			Polygon.Vertices[0] = V2(0, 0.5f) * Dim + Pos;
			Polygon.Vertices[1] = V2(-0.5f, -0.5f) * Dim + Pos;
			Polygon.Vertices[2] = V2(0.5f, -0.5f) * Dim + Pos;
			
			Polygon.Angle = Angle - 90;
		} break;
		case POLYGON_RECTANGLE:
		{
			Polygon.Count = 4;
			Polygon.Vertices[0] = V2(-0.5f, 0.5f) * Dim + Pos;
			Polygon.Vertices[1] = V2(-0.5f, -0.5f) * Dim + Pos;
			Polygon.Vertices[2] = V2(0.5f, -0.5f) * Dim + Pos;
			Polygon.Vertices[3] = V2(0.5f, 0.5f) * Dim + Pos;
			
			Polygon.Angle = Angle;
		} break;
	}
	
	RotateVertices(&Polygon, Polygon.Angle);
	
	return Polygon;
}

void UpdateVertices(polygon* Shape, v2 Delta)
{
	Shape->Pos += Delta;
	for(int i = 0; i < Shape->Count; ++i)
	{
		Shape->Vertices[i] += Delta;
	}
}

void ResizeVertices(polygon* Shape, v2 Pos, v2 Dim)
{
	Shape->Dim = Dim;
	switch(Shape->Type)
	{
		case POLYGON_TRIANGLE:
		{
			Shape->Vertices[0] = V2(0, 0.5f) * Dim + Pos;
			Shape->Vertices[1] = V2(-0.5f, -0.5f) * Dim + Pos;
			Shape->Vertices[2] = V2(0.5f, -0.5f) * Dim + Pos;
		} break;
		case POLYGON_RECTANGLE:
		{
			Shape->Vertices[0] = V2(-0.5f, 0.5f) * Dim + Pos;
			Shape->Vertices[1] = V2(-0.5f, -0.5f) * Dim + Pos;
			Shape->Vertices[2] = V2(0.5f, -0.5f) * Dim + Pos;
			Shape->Vertices[3] = V2(0.5f, 0.5f) * Dim + Pos;
		} break;
	}
}

void ResetVertices(polygon* Shape, v2 Dim, v2 Pos)
{
	switch(Shape->Type)
	{
		case POLYGON_TRIANGLE:
		{
			Shape->Vertices[0] = V2(0, 0.5f) * Dim + Pos;
			Shape->Vertices[1] = V2(-0.5f, -0.5f) * Dim + Pos;
			Shape->Vertices[2] = V2(0.5f, -0.5f) * Dim + Pos;
		} break;
		case POLYGON_RECTANGLE:
		{
			Shape->Vertices[0] = V2(-0.5f, 0.5f) * Dim + Pos;
			Shape->Vertices[1] = V2(-0.5f, -0.5f) * Dim + Pos;
			Shape->Vertices[2] = V2(0.5f, -0.5f) * Dim + Pos;
			Shape->Vertices[3] = V2(0.5f, 0.5f) * Dim + Pos;	
		} break;
	}
}

void RotateVertices(polygon* Shape, real32 Theta)
{
	Shape->Angle += Theta;
	for(int i = 0; i < Shape->Count; ++i)
	{
		Shape->Vertices[i] -= Shape->Pos;
		Shape->Vertices[i] = Rotate(Shape->Vertices[i], Radians(Theta));
		Shape->Vertices[i] += Shape->Pos;
	}
}

#endif 
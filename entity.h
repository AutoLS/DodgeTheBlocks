#ifndef ENTITY_H
#define ENTITY_H

#include "physics.h"

enum entity_type
{
	ENTITY_PLAYER,
	ENTITY_BLOCK,
	ENTITY_BLOCK_METEOR,
	ENTITY_METEOR_FRAGMENTS,
	ENTITY_ITEM,
	ENTITY_LASER,
	ENTITY_ENEMY_1,
	ENTITY_ENEMY_2,
	ENTITY_ENEMY_3,
};

struct entity
{
	rigid_body Body;
	entity_type Type;
	bool Alive;
	
	object_data* RenderShape;
	texture_data* Texture;
	//NOTE: For testing purposes only will replace with textures later
	v4 RenderColor;
};

struct entity_pool
{
	entity* Entities;
	uint32 UsedEntities;
	uint32 EntitySize;
	stack DeadStack;
};

entity_pool InitializeEntityPool(uint32 EntitySize, uint32 StackCapacity)
{
	entity_pool EntityPool = {};
	EntityPool.EntitySize = EntitySize;
	EntityPool.Entities = (entity*)malloc(sizeof(entity) * EntitySize);
	EntityPool.DeadStack.Size = 0;
	EntityPool.DeadStack.Capacity = StackCapacity;
	EntityPool.DeadStack.MyStack = (int*)malloc(sizeof(int) * EntityPool.DeadStack.Capacity);
	Push(&EntityPool.DeadStack, -1);
	
	return EntityPool;
}

void ClearEntityPool(entity_pool* Pool)
{
	Pool->UsedEntities = 0;
	if(Pool->Entities)
	{
		free(Pool->Entities);
	}
	Pool->Entities = (entity*)malloc(sizeof(entity) * Pool->EntitySize);
	Pool->DeadStack.Size = 0;
	Pool->DeadStack.Capacity = 5;
	free(Pool->DeadStack.MyStack);
	Pool->DeadStack.MyStack = 
	(int*)malloc(sizeof(int) * Pool->DeadStack.Capacity);
	Push(&Pool->DeadStack, -1);
}

int AddEntity(entity_pool* Pool, 
			  rigid_body Body, 
			  object_data* RenderShape,
			  texture_data* Texture,
			  v4 RenderColor,
			  entity_type Type)
{
	entity Entity = {};
	Entity.Body = Body;
	Entity.Type = Type;
	Entity.Alive = true;
	Entity.RenderShape = RenderShape;
	Entity.Texture = Texture;
	Entity.RenderColor = RenderColor;
	
	int DeadIndex = Peek(&Pool->DeadStack);
	if(DeadIndex == -1)
	{
		Pool->Entities[Pool->UsedEntities++] = Entity;
		return Pool->UsedEntities - 1;
	}
	else
	{
		Pool->Entities[DeadIndex] = Entity;
		Pop(&Pool->DeadStack);
		return DeadIndex;
	}
}

void KillEntity(entity_pool* Pool, int Index)
{
	Pool->Entities[Index].Alive = false;
	Push(&Pool->DeadStack, Index);	
#if DEBUG_MODE
	switch(Pool->Entities[Index].Type)
	{
		case ENTITY_LASER:
		printf("Laser Killed. Entity index: %d\n", Index);
		break;
	}
#endif
}

void SetEntitySpeed(entity_pool* Pool, entity_type Type, real32 Speed)
{
	for(uint32 EntityIndex = 0;
		EntityIndex < Pool->UsedEntities;
		++EntityIndex)
	{
		entity* Entity = &Pool->Entities[EntityIndex];
		if(Entity->Alive && Entity->Type == Type)
		{
			Entity->Body.Vel = V2(0, -1) *
			Speed;
		}
	}
}

bool OutOfBound(entity* Entity, v2 MinBound, v2 MaxBound)
{
	v2 Pos = Entity->Body.Pos;
	return Pos.x > MaxBound.x ||
		   Pos.y > MaxBound.y ||
		   Pos.x < MinBound.x ||
		   Pos.y < MinBound.y;
}

#endif
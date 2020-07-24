#ifndef PARTICLE_H
#define PARTICLE_H

struct particle
{
	v2 Position;
	v2 Dim;
	
	v4 Color;
	
	real32 Life;
	real32 Strength;
	texture_data Texture;
	bool Alive;
	real32 Brightness;
};

struct particle_generator
{
	particle* Particles;
	stack DeadStack;
	int UsedParticles;
	int ParticleSize;
};

particle_generator 
CreateParticleGenerator(int ParticleSize, uint32 StackCapacity)
{
	particle_generator Generator = {};
	Generator.ParticleSize = ParticleSize;
	Generator.Particles = (particle*)malloc(sizeof(particle) * ParticleSize);
	Generator.DeadStack.Size = 0;
	Generator.DeadStack.Capacity = StackCapacity;
	Generator.DeadStack.MyStack = (int*)malloc(sizeof(int) * Generator.DeadStack.Capacity);
	Push(&Generator.DeadStack, -1);
	
	return Generator;
}

void ClearParticleGenerator(particle_generator* Generator)
{
	Generator->UsedParticles = 0;
	if(Generator->Particles) free(Generator->Particles);
	if(Generator->DeadStack.MyStack) free(Generator->DeadStack.MyStack);
}

void SpawnParticle(particle_generator* Generator, v2 Pos, v2 Dim, 
					real32 Strength,
					texture_data Texture, v4 ParticleColor = Color())
{
	particle Particle = {};
	Particle.Position = Pos;
	Particle.Dim = Dim;
	Particle.Life = 255;
	Particle.Strength = Strength;
	Particle.Alive = true;
	Particle.Texture = Texture;
	Particle.Brightness = Rand32(0.8f, 1.0f);
	Particle.Color = ParticleColor;
	
	int DeadIndex = Peek(&Generator->DeadStack);
	if(DeadIndex == -1)
	{
		Generator->Particles[Generator->UsedParticles++] = Particle;
	}
	else
	{
		Generator->Particles[DeadIndex] = Particle;
		Pop(&Generator->DeadStack);
	}
}

void KillParticle(particle_generator* Generator, int Index)
{
	Generator->Particles[Index].Alive = false;
	Push(&Generator->DeadStack, Index);
	//printf("particle dead\n");
}

void UpdateParticles(particle_generator* Generator, real32 t)
{
	for(int i = 0; i < Generator->UsedParticles; ++i)
	{
		if(Generator->Particles[i].Alive)
		{
			Generator->Particles[i].Life -= Generator->Particles[i].Strength * t;
			if(Generator->Particles[i].Life <= 0)
			{
				KillParticle(Generator, i);
			}
		}
	}
}

void RenderParticles(particle_generator* Generator, 
					 game_renderer* Renderer,
					 render* Graphics,
					 v2 PlayArea)
{	
	for(int i = 0; i < Generator->UsedParticles; ++i)
	{
		particle* Particle = &Generator->Particles[i];
		if(Generator->Particles[i].Alive)
		{
			Particle->Color.a = Particle->Life / 255.0f;
			DrawObject(Renderer->Shaders[RENDERER_TEXTURED],
					   &Renderer->RenderObjects[R_OBJECT_RECT],
					   V3(Particle->Position), V3(Particle->Dim),
					   0, {}, Particle->Color,
					   Particle->Texture.ID, true);
		}
	}
}

#endif
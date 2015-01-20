#pragma once
#include "stdafx.h"

class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth,gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth,terrDepth;

	typedef  GLfloat vector3[3];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector3 *vertices;
	vector3 *colors;
	
	bool loadImage();

	float getHeight(float x, float y);
	void setPoint(vector3, float, float,float);
	vector<sf::Texture> textures;

public:
	Terrain(void);
	~Terrain(void);

	string index;

	void LoadImages(string filepath);
	void Init();
	void Draw();
};


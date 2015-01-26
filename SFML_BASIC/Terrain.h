#pragma once
#include "stdafx.h"

using sf::Vector2f;

class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth, gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth,terrDepth;

	typedef  GLfloat vector3[3];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector3 *vertices;
	vector3 *colors;
	
	bool loadImage();

	Vector2f normalisePos(Vector2f pos, Vector2f max);
	float normalisePos(float pos, float max);

	float heightMapLookup(Vector2f pos);
	float heightMapLookup(float x, float y);

	float getHeight(float x, float y);
	void setPoint(vector3, float, float,float);

	vector<sf::Image> heightMaps;
	vector<sf::Image>::iterator currentHeightMap;
	bool crementIter(int direction);


	enum process { AVERAGE = 0, ADD = 1, EXTADD, ENUMCOUNT };
	int currentProcess;

	float colorProcess(sf::Color c);
	float heightProcess(float h);

	float hAdjust;

public:
	Terrain(void);
	~Terrain(void);

	string index;

	void LoadImages(string filepath);

	void nextMap();
	void prevMap();

	void checkInputKB(sf::Keyboard k);
	void Init();
	void Draw();
};


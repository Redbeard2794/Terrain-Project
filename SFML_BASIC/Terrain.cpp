#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain(void) : index("index.txt"), currentProcess(AVERAGE), hAdjust(0.1)
{
	gridWidth=100; //squares in grid
	gridDepth=100;

	terrWidth=50; //size of terrain in world units
	terrDepth=50;
	vertices=NULL;
	colors=NULL;	
	
	//num squares in grid will be width*height,  two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth * gridWidth * 2 * 3;


}

Terrain::~Terrain(void)
{
	delete [] vertices;
	delete [] colors;
}

//interpolate between two values
float lerp(float start,  float end,  float t){
	return start + (end-start) * t;
}

void Terrain::setPoint(vector3 v, float x,  float y,  float z){

		v[0] = x;
		v[1] = y;
		v[2] = z;
}

//Wrapper function for vector2f support (max is maximum x,  maximum y)
Vector2f Terrain::normalisePos(Vector2f pos,  Vector2f max)
{
	Vector2f newPos;
	newPos.x = normalisePos(pos.x,  max.x);
	newPos.y = normalisePos(pos.y,  max.y);

	return newPos;
}

//Normalises a position given the current one and max
float Terrain::normalisePos(float pos,  float max)
{
	//float newX = targLeft + (targWidth / srcWidth) * x;
	return 0 + (1 / max) * pos;

	//(pos + start) / (end - start);
}

//Wrapper function for vector2f support
float Terrain::heightMapLookup(Vector2f pos)
{
	return heightMapLookup(pos.x,  pos.y);
}

//Grab data from the current map (x,  y should be in normalised coords [0 -> 1])
float Terrain::heightMapLookup(float x,  float y)
{
	float height = 0;

	//Return flat if:
		//Heightmap is broke
		//Invalid coords
	if (!(currentHeightMap._Ptr == NULL 
		|| x < 0 || x > 1
		|| y < 0 || y > 1))
	{
		sf::Image &hm = (*currentHeightMap);

		unsigned int mapWidth = hm.getSize().x;
		unsigned int mapHeight = hm.getSize().y;

		sf::Color col = hm.getPixel(x * mapWidth, y * mapHeight);

		if (col.r > 128)
			int d = 0;

		//Do some stuff with the color for calculations
		height = (heightProcess(colorProcess(col)));
	}

	else cout << "Invalid heightMapLookup" << endl;

	return height;
}

//Take the color and turn it into a float
//Has a multitude of options controlled by the currentProcess var
float Terrain::colorProcess(sf::Color c)
{
	switch (currentProcess)
	{
		//Average of rgb values, works for greyscale too.
	case AVERAGE:
		return (c.r + c.g + c.b) / 3;
		break;

		//Addition of rgb values, straight up.
	case ADD:
		return c.r + c.g + c.b;
		break;

		//Extended range addition, for more values
	case EXTADD:
		return c.r * 255 * 255 + c.g * 255 + c.b;
		break;
	
		//Defaults to average
	default:
		return (c.r + c.g + c.b) / 3;
		break;
	}
}

//Make the height not so awfully big
//Use the same switch as colorProcess, controlled by currentProcess as well
float Terrain::heightProcess(float h)
{
	float hie;
	switch (currentProcess)
	{
		//Average of rgb values, works for greyscale too.
	case AVERAGE:
		hie = h / (h * hAdjust);
		return hie;
		break;

		//Addition of rgb values, straight up.
	case ADD:
		return h;
		break;

		//Extended range addition, for more values
	case EXTADD:
		return h;
		break;

		//Defaults to average
	default:
		return h;
		break;
	}
}

/* KEN'S METHOD
//helper function to calculate height of terrain at a given point in space
//you will need to modify this significantly to pull height from a map
float  Terrain::getHeight(float x,  float y){

	//for the sample we will calculate height based on distance form origin
	float dist= sqrt(x * x + y * y);

	//center will be the highest point
	dist = 30 - dist;
	//put a nice curve in it
	dist *= dist;
	dist *= dist;
	//whoah,  way to high,  make it smaller
	dist /= 50000;

	return dist;
}
*/

//getHeight takes OpenGL coords [-1 -> 1]
float Terrain::getHeight(float x, float y){

	float normX = normalisePos(x + 25, 50);
	float normY = normalisePos(y + 25, 50);

	return heightMapLookup(normX, normY);
}

//Positive goes right, negative goes left, will loop around if you go beyond the edge.
//Returns true if we actually moved, else false
bool Terrain::crementIter(int direction)
{
	bool moved = false;

	//Back
	if (direction < 0)
	{
		//If we hit the end, loop around
		if (currentHeightMap == heightMaps.begin())
		{
			currentHeightMap = --heightMaps.end();
		}

		//Move backwards
		else currentHeightMap--;
		
		moved = true;
	}

	//Forward
	if (direction > 0)
	{
		//Move forward
		currentHeightMap++;

		//If we hit the end, loop around
		if (currentHeightMap == heightMaps.end())
		{
			currentHeightMap = heightMaps.begin();
		}

		moved = true;
	}

	return moved;
}

//PUBLIC FUNCTIONS BELOW//

void Terrain::LoadImages(string indexfile)
{
	std::cout << "Loading images" << endl;
	string c;
	std::ifstream myfile;

	sf::Image temp;
	
	myfile.open(basepath + indexfile);
	while (myfile >> c) 
	{
		if (!temp.loadFromFile(basepath + c))
		{
			heightMaps.push_back(temp);
			continue;
		}
		std::cout << "\t" << "Loaded: " << c << endl;
		heightMaps.push_back(temp);
	}
	myfile.close();

	currentHeightMap = heightMaps.begin();
}

//Moves to the next map and reloads
void Terrain::nextMap()
{
	//If we've actually changed
	if (crementIter(1))
	{
		cout << "NEXT MAP" << endl;
		Init();
	}
}

//Moves to the previous map and reloads
void Terrain::prevMap()
{
	if (crementIter(-1))
	{
		cout << "PREV MAP" << endl;
		Init();
	}
}

void Terrain::checkInputKB(sf::Keyboard k)
{
	static bool comma;
	static bool period;

	//< : Previous Map
	if (k.isKeyPressed(k.Comma))
	{
		if (!comma)
			prevMap();

		comma = true;
	}

	else comma = false;

	//> : Next Map
	if (k.isKeyPressed(k.Period))
	{
		if (!period)
			nextMap();

		period = true;
	}

	else period = false;
}

void Terrain::Init(){
	
	delete [] vertices;//just in case we've called init before
	vertices=new vector3[numVerts];
	delete [] colors;
	colors=new vector3[numVerts];

	//If we're empty try to load
	if (heightMaps.empty())
	{
		LoadImages(index);
	}

	//interpolate along the edges to generate interior points
	for (int i = 0; i < gridWidth - 1; i++) //iterate left to right
	{
		for(int j = 0; j < gridDepth - 1; j++)//iterate front to back
		{
			int sqNum = (j + i * gridDepth);
			int vertexNum = sqNum * 3 * 2; //6 vertices per square (2 tris)
			float front = lerp(-terrDepth / 2, terrDepth / 2, (float)j / gridDepth);
			float back  = lerp(-terrDepth / 2, terrDepth / 2, (float)(j + 1) / gridDepth);
			float left = lerp(-terrWidth / 2, terrWidth / 2, (float)i / gridDepth);
			float right = lerp(-terrDepth / 2, terrDepth / 2, (float)(i + 1) / gridDepth);
			
			/*
			back  + ----- + 	looking from above,  the grid is made up of regular squares
			       |tri1/|	'left & 'right' are the x cooded of the edges of the square
				   |   / |	'back' & 'front' are the y coords of the square
				   |  /  |	each square is made of two triangles (1 & 2)
				   | /   |	
				   |/tri2|
			front + ----- + 
			     left   right
				 */
			//tri1
			setPoint(colors[vertexNum], (rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0);
			setPoint(vertices[vertexNum++], left, getHeight(left, front), front);

			setPoint(colors[vertexNum], (rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0);
			setPoint(vertices[vertexNum++], right, getHeight(right, front), front);

			setPoint(colors[vertexNum], (rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0);
			setPoint(vertices[vertexNum++], right, getHeight(right, back), back);


			//declare a degenerate triangle
			//TODO: fix this to draw the correct triangle

			//setPoint(colors[vertexNum], 0, 0, 0);
			setPoint(colors[vertexNum], (rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0);
			//setPoint(vertices[vertexNum++], 0, 0, 0);
			setPoint(vertices[vertexNum++], right, getHeight(right, back), back);

			//setPoint(colors[vertexNum], 0, 0, 0);
			setPoint(colors[vertexNum], (rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0);
			//setPoint(vertices[vertexNum++], 0, 0, 0);
			setPoint(vertices[vertexNum++], left, getHeight(left, back), back);

			//setPoint(colors[vertexNum], 0, 0, 0);
			setPoint(colors[vertexNum], (rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0);
			//setPoint(vertices[vertexNum++], 0, 0, 0);
			setPoint(vertices[vertexNum++], left, getHeight(left, front), front);
		}
	}




}

void Terrain::Draw(){

	glBegin(GL_TRIANGLES);
	for(int i =0;i<numVerts;i++){
			glColor3fv(colors[i]);
			glVertex3fv(vertices[i]);
	}
	glEnd();
}
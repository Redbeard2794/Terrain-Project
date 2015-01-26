#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class Camera{
    static aiVector3D zero,yaxis,zaxis;
public:
    aiVector3D position;
    aiVector3D forward;
    aiVector3D up;

	aiVector3D look;//point that the camera will look at
 
    float forwardSpeed;
    float roationSpeed;
     
    Camera():forwardSpeed(0.5f),roationSpeed(0.1f){}
 
    void Init(aiVector3D& p=zero, aiVector3D& f=zaxis, aiVector3D& u=yaxis){
        position=p;
        forward=f;
        up=u;
		look = aiVector3D(0, 0, 0);
    }
 
	void CheckInputKB(sf::Keyboard k)
	{
		//W : Move forward
		if (k.isKeyPressed(k.W))
		{
			MoveForwardBack(+1);
		}

		//A : Move left
		if (k.isKeyPressed(k.A))
		{
			MoveLeftRight(-1);
		}

		//S : Move backward
		if (k.isKeyPressed(k.S))
		{
			MoveForwardBack(-1);
		}

		//D : Move right
		if (k.isKeyPressed(k.D))
		{
			MoveLeftRight(+1);
		}

		//Q : Move up
		if (k.isKeyPressed(k.Q))
		{
			MoveUpDown(+1);
		}

		//E : Move down
		if (k.isKeyPressed(k.E))
		{
			MoveUpDown(-1);
		}

		//Up : Look up
		if (k.isKeyPressed(k.Up))
		{
			TurnUpDown(+1);
		}

		//Left : Look left
		if (k.isKeyPressed(k.Left))
		{
			TurnLeftRight(-1);
		}

		//Down : Look down
		if (k.isKeyPressed(k.Down))
		{
			TurnUpDown(-1);
		}

		//Right : Look right
		if (k.isKeyPressed(k.Right))
		{
			TurnLeftRight(+1);
		}

	}

	void MoveLeftRight(int dir){ //Dir=+1=>Right, dir=-1=> Left
		//TODO
		position.x += (forwardSpeed*dir);
	}

	void MoveUpDown(int dir){ //Dir=+1=>Right, dir=-1=> Left
		//TODO
		position.y += (up.y*(forwardSpeed*dir));
	}

	void MoveForwardBack(int dir){ //Dir=+1=>Forward, dir=-1=> Back

		position += (forward*(forwardSpeed*dir));
	}

	void TurnLeftRight(int dir){ //Dir=+1=>Right, dir=-1=> Left
		//TODO
		forward.x += dir;//or look?
	}

	void TurnUpDown(int dir){ //Dir=+1=>Up, dir=-1=> Down
		//TODO
		forward.y += dir;//or look?
	}
 
    void ViewingTransform(){
        gluLookAt(	position.x,position.y,position.z,// camera position
			forward.x, forward.y, forward.z, //look at this point //TODO: BUG here!! what is it ??
					0,1,0); //camera up
		//cout << forward.x << forward.y << forward.z << endl;
    }
 
};

//create some default vectors
aiVector3D Camera::zero(0.0f);
aiVector3D Camera::yaxis(0.0f,1.0f,0.0f);
aiVector3D Camera::zaxis(0.0f,0.0f,1.0f);
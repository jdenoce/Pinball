#include "object.h"
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <Magick++.h>
#include <string>
#include <GL/glew.h>
#include "texture.h"
#include <btBulletDynamicsCommon.h>


using namespace std;

//Object constructor
Object::Object(string myFile, btTriangleMesh &objMesh)
{ 
	//Object importer
	Assimp::Importer importer;

        //Ensure that RNG is possibly random
	srand(time(0));

        //File reader
	const char* fileName = myFile.c_str();

        //Declare variables for obtaining the models
	const aiScene *pScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	const aiMesh *pai_mesh = pScene->mMeshes[0]; 

        //Model the vertices
	for (int i = 0; i < pai_mesh->mNumVertices; i++)
	{
	        aiVector3D pPos = pai_mesh->mVertices[i];

	        glm::vec3 position = glm::vec3(pPos.x, pPos.y, pPos.z);

	        glm::vec2 uvcoords;

		aiVector3D pnormal = pai_mesh->mNormals[i];
		glm::vec3 normal = glm::vec3(pnormal.x, pnormal.y, pnormal.z);
//normals

	        Vertices.push_back(Vertex(position, uvcoords, normal));
	}

        //Model the faces
	for (int i = 0; i < pai_mesh->mNumFaces; i++)
	{
	        const aiFace* Face = &pai_mesh->mFaces[i];  

	        for (int j = 0; j < 3; j++)
	        {                        
	                Indices.push_back(Face->mIndices[j]);
	                aiVector3D uv = pai_mesh->mTextureCoords[0][Face->mIndices[j]];

	                glm::vec2 uvcoords;

	                uvcoords.x = uv.x;
	                uvcoords.y = uv.y;

	                Vertices[(i*3)+j].uv = uvcoords;
	                
	        }
	}

	btVector3 triArray[3];
	for (unsigned int i = 0; i < pai_mesh->mNumFaces; i++)
	{
		aiFace* Face = &pai_mesh->mFaces[i];
		
		for (unsigned int j = 0; j < 3; j++)
		{
			aiVector3D position = pai_mesh->mVertices[Face->mIndices[j]];
			triArray[j] = btVector3(position.x, position.y, position.z);
		}
		
		objMesh.addTriangle(triArray[0], triArray[1], triArray[2]);
	}

        //Initialize the angles for rotation and orbits
	angle= 0.0f;

        //Initialize the vertices and indices
	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);	
}

//Object destructor
Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}



//Updates position, rotation, and orbit of a planet
void Object::planet_update(unsigned int dt, float xMove, btRigidBody *rigidBody, btDiscreteDynamicsWorld *dynamicsWorld)
{
	//angle += dt * M_PI/10000;  
	dynamicsWorld->stepSimulation(1 / 300.f, 10);

	btTransform trans;
	
	if(xMove > 0)
	{
		//
	}

	rigidBody->getMotionState()->getWorldTransform(trans);

	btScalar m[16];
	trans.getOpenGLMatrix(m);

	model = glm::make_mat4(m);

}

//Returns the model
glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render(Texture* pTexture)
{
  pTexture->Bind(GL_TEXTURE0);

/*
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
*/

glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	// bind indice buffer
	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	// bind indice buffer
	//glGenBuffers(1, &normal);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normal);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

//glEnable(GL_LIGHTING);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
  glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}


#include "Scene.h"
#include "StdAfx.h"


Scene::Scene() : camera(new Camera()), isBBoxOn(false), vertexNormals(false), 
	polyNormals(false), isCalcNormal(true), bgColor((AL_BLACK)), fineNess(20),
	selectedModelIndex(-1)
{
}


Scene::~Scene()
{
	while (models.size() > 0)
	{
		Model* model = models.back();
		models.pop_back();
		delete model;
	}
	delete camera;
}

void Scene::CreateModel()
{
	Model* model = new Model();
	models.push_back(model);
	selectedModelIndex = models.size() - 1;
}

const std::vector<Model*>& Scene::GetModels() const
{
	return models;
}

Camera * Scene::GetCamera() const
{
	return camera;
}

void Scene::SetBBoxState(bool isBBoxOn)
{
	this->isBBoxOn = isBBoxOn;
}

bool Scene::GetBBoxState() const
{
	return isBBoxOn;
}

void Scene::SetNormals(bool vertexNormals, bool polyNormals)
{
	this->vertexNormals = vertexNormals;
	this->polyNormals = polyNormals;
}

bool Scene::AreVertexNormalsOn() const
{
	return vertexNormals;
}

bool Scene::ArePolyNormalsOn() const
{
	return polyNormals;
}

void Scene::SetCalcNormalState(bool isCalcNorm)
{
	this->isCalcNormal = isCalcNorm;
}

bool Scene::GetCalcNormalState() const
{
	return isCalcNormal;
}

void Scene::SetFineNess(int fineNess)
{
	this->fineNess = fineNess;
}

int Scene::GetFineNess() const
{
	return fineNess;
}

Model * Scene::GetSelectedModel()
{
	if (selectedModelIndex < 0)
		return nullptr;

	return models[selectedModelIndex];
}

void Scene::SelectNextModel()
{
	if (models.size() == 0)
		return;

	selectedModelIndex++;
	selectedModelIndex = (selectedModelIndex >= models.size()) ? 0 : selectedModelIndex;
}

void Scene::SelectPreviousModel()
{
	if (models.size() == 0)
		return;

	selectedModelIndex--;
	selectedModelIndex = (selectedModelIndex < 0) ? (models.size() - 1) : selectedModelIndex;
}

void Scene::UnselectAllModels()
{
	selectedModelIndex = -1;
}

void Scene::DeleteModels()
{
	while (models.size() > 0)
	{
		Model* model = models.back();
		models.pop_back();
		delete model;
	}
	selectedModelIndex = -1;
}

void Scene::ClearScene()
{
	DeleteModels();
	delete camera;

	camera = new Camera();
}

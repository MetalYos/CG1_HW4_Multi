#pragma once

#include "Geometry.h"
#include "Model.h"
#include "Camera.h"
#include "ALMath.h"
#include "Light.h"
#include "Animation.h"

class Scene
{
private:
	std::vector<Model*> models;
	Camera* camera;

	bool isBBoxOn;
	bool vertexNormals;
	bool polyNormals;
	bool isCalcNormal;
	Vec4 bgColor;
	int fineNess;
	int selectedModelIndex;

	Scene();

public:
	static Scene& GetInstance()
	{
		static Scene instance; // Guaranteed to be destroyed.
							   // Instantiated on first use.
		return instance;
	}
	
	~Scene();

	Scene(const Scene& s) = delete;
	Scene& operator=(const Scene& s) = delete;

	void CreateModel();

	const std::vector<Model*>& GetModels() const;
	Camera* GetCamera() const;

	void SetBBoxState(bool isBBoxOn);
	bool GetBBoxState() const;

	void SetNormals(bool vertexNormals, bool polyNormals);
	bool AreVertexNormalsOn() const;
	bool ArePolyNormalsOn() const;

	void SetCalcNormalState(bool isCalcNorm);
	bool GetCalcNormalState() const;

	void SetFineNess(int fineNess);
	int GetFineNess() const;

	Model* GetSelectedModel();
	void SelectNextModel();
	void SelectPreviousModel();
	void UnselectAllModels();
	void DeleteModels();

	void ClearScene();
};


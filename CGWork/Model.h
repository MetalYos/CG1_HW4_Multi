#pragma once

#include "ALMath.h"
#include "Geometry.h"
#include "Material.h"
#include "Animation.h"

class Model
{
private:
	std::vector<Geometry*> geos;
	Material* material;
	Mat4 objToWorld;
	Mat4 viewTransform;
	Vec4 color;
	Vec4 normalColor;
	Animation* anim;

	// BBox parameters
	std::vector<Poly*> bbox;
	Vec4 minCoord;
	Vec4 maxCoord;

public:
	int NormalSign;

public:
	Model() : material(new Material()), color(AL_WHITE), normalColor(AL_RED),
		anim(new Animation), NormalSign(1) { }
	~Model();

	void AddGeometry(Geometry* geo);

	const Mat4& GetObjToWorldTransform() const;
	const Mat4& GetViewTransform() const;
	void Translate(const Mat4& T, bool isObjectSpace = true);
	void Scale(const Mat4& S, bool isObjectSpace = true);
	void Rotate(const Mat4& R, bool isObjectSpace = true);

	const std::vector<Geometry*>& GetGeometries() const;
	const std::vector<Poly*>& GetBBox() const;

	void SetColor(int r, int g, int b);
	void SetColor(const Vec4& color);
	const Vec4& GetColor() const;

	void SetNormalColor(int r, int g, int b);
	void SetNormalColor(const Vec4& color);
	const Vec4& GetNormalColor() const;

	void BuildBoundingBox();
	Vec4 GetBBoxDimensions() const;
	Vec4 GetBBoxCenter() const;

	Material* GetMaterial();
	Animation* GetAnimation();
	
	void DeleteGeometries();
};


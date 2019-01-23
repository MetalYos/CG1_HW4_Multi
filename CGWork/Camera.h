#pragma once

#include "ALMath.h"

struct PerspectiveParams
{
	double Left;
	double Right;
	double Top;
	double Bottom;
	double Near = 1.0;
	double Far = 1000.0;
	double FOV;
	double AspectRatio;
};

struct OrthographicParams
{
	double Left;
	double Right;
	double Top;
	double Bottom;
	double Near = 1.0;
	double Far = 1000.0;
};

struct CameraParameters
{
	Vec4 Eye;
	Vec4 EyeCam;
	Vec4 Front;
	Vec4 Side;
	Vec4 Up;
	Vec4 WorldUp;
	Vec4 WorldAt;

	double Yaw;
	double Pitch;

	CameraParameters()
		: Yaw(-90.0), Pitch(0.0) {}
};

class Camera
{
private:
	Mat4 cTransform;
	Mat4 projection;

	Mat4 perspective;
	Mat4 orthographic;
	bool isPerspective;

	CameraParameters camParams;
	PerspectiveParams perspectiveParams;
	OrthographicParams orthographicParams;

public:
	Camera();
	~Camera();

	Mat4 GetTranform() const;
	void OrbitCamera(double yawOffset, double pitchOffset);
	void ZoomCamera(double zoomOffset);
	void PanCamera(double xOffset, double yOffset);

	void SetOrthographic(double left, double right, double top, double bottom, double z_near, double z_far);
	void SetOrthographic(double height, double aspectR, double z_near, double z_far);
	void SetPerspective(double left, double right, double top, double bottom, double z_near, double z_far);
	void SetPerspective(double fovy, double aspectR, double z_near, double z_far);
	void SetPerspective2(double fovy, double aspectR, double alpha, double d);
	Mat4 GetProjection() const;

	const CameraParameters& GetCameraParameters() const;
	const PerspectiveParams& GetPerspectiveParameters() const;
	const OrthographicParams& GetOrthographicParameters() const;

	void SwitchProjection(bool isPerspective);
	bool IsPerspective() const;

	void LookAt(const Vec4& eye, const Vec4& at, const Vec4& up);
};


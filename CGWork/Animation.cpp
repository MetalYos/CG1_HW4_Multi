#include "Animation.h"
#include <assert.h>
#include "Scene.h"

void Animation::AddKeyFrame(Frame* keyFrame)
{
	if (keyFrame == nullptr)
		return;

	assert(keyFrame->FrameNumber > maxFrame);

	keyFrames.push_back(keyFrame);
	if (keyFrame->FrameNumber > 0)
	{
		Frame* prevKF = keyFrames[keyFrames.size() - 2];
		keyFrame->OriginalFrame = prevKF->OriginalFrame + (keyFrame->FrameNumber - prevKF->FrameNumber);
		keyFrame->FrameNumber = (int)(keyFrame->OriginalFrame * speedFactor) + keyFrames[0]->OriginalFrame;
	}
	maxFrame = keyFrame->FrameNumber;

	if (keyFrame->FrameNumber == 0)
		currentFrame = GetFrame(0);
}

Frame* Animation::GetFrame(int frame, bool linearInterpolation)
{
	if ((frame < 0) || (frame > maxFrame) || (keyFrames.size() == 0))
		return nullptr;

	Frame* frameToReturn = nullptr;
	if (linearInterpolation)
	{
		for (unsigned int i = 0; i < keyFrames.size(); i++)
		{
			if (keyFrames[i]->FrameNumber == frame)
			{
				frameToReturn = new Frame();
				frameToReturn->ObjToWorld = keyFrames[i]->ObjToWorld;
				frameToReturn->ViewTransform = keyFrames[i]->ViewTransform;
				frameToReturn->FrameNumber = frame;
				break;
			}
			else if ((keyFrames[i]->FrameNumber < frame) &&
				(keyFrames[i + 1]->FrameNumber > frame))
			{
				frameToReturn = GetFrameLinearInterpolation(keyFrames[i], keyFrames[i + 1], frame);
				break;
			}
		}
	}
	else
	{
		frameToReturn = GetFrameBezierInterpolation(frame);
	}

	return frameToReturn;
}

int Animation::GetLastFrameNumber() const
{
	return maxFrame;
}

void Animation::ResetAnimation()
{
	if (keyFrames.size() == 0)
		return;

	currentFrame = GetFrame(0);
}

bool Animation::StepToNextFrame(bool linearInterpolation)
{
	if ((keyFrames.size() == 0) || (currentFrame->FrameNumber == maxFrame))
		return false;

	int currentFrameNum = currentFrame->FrameNumber;
	delete currentFrame;
	currentFrame = GetFrame(++currentFrameNum, linearInterpolation);
	
	if (currentFrame == nullptr)
		return false;

	return true;
}

const Frame* Animation::GetCurrentFrame() const
{
	return currentFrame;
}

void Animation::IncreasePlaybackSpeed(double percentage)
{
	double factor = 1.0 - percentage / 100.0;
	for (unsigned int i = 1; i < keyFrames.size(); i++)
	{
		keyFrames[i]->FrameNumber = (int)(keyFrames[i]->FrameNumber * factor) + keyFrames[0]->OriginalFrame;
	}
	maxFrame = keyFrames[keyFrames.size() - 1]->FrameNumber;
	speedFactor *= factor;
}

void Animation::DecreasePlaybackSpeed(double percentage)
{
	double factor = 1.0 + percentage / 100.0;
	for (unsigned int i = 1; i < keyFrames.size(); i++)
	{
		keyFrames[i]->FrameNumber = (int)(keyFrames[i]->FrameNumber * factor) + keyFrames[0]->OriginalFrame;
	}
	maxFrame = keyFrames[keyFrames.size() - 1]->FrameNumber;
	speedFactor *= factor;
}

void Animation::NormalPlaybackSpeed()
{
	for (Frame* keyFrame : keyFrames)
	{
		keyFrame->FrameNumber = keyFrame->OriginalFrame;
	}
	maxFrame = keyFrames[keyFrames.size() - 1]->FrameNumber;
	speedFactor = 1.0;
}

void Animation::ClearAnimation()
{
	while (keyFrames.size() > 0)
	{
		Frame* frame = keyFrames.back();
		keyFrames.pop_back();
		delete frame;
	}
	currentFrame = nullptr;
	maxFrame = -1;
	speedFactor = 1.0;
}

Frame* Animation::GetFrameLinearInterpolation2(Frame* before, Frame* after, int frame) const
{
	Frame* result = new Frame();
	double t = (double)(frame - before->FrameNumber) / (after->FrameNumber - before->FrameNumber);

	result->ObjToWorld = before->ObjToWorld * (1.0 - t) + after->ObjToWorld * t;
	result->ViewTransform = before->ViewTransform * (1.0 - t) + after->ViewTransform * t;
	result->FrameNumber = frame;

	return result;
}

Frame* Animation::GetFrameLinearInterpolation(Frame * before, Frame * after, int frame) const
{
	Frame* result = new Frame();
	double t = (double)(frame - before->FrameNumber) / (after->FrameNumber - before->FrameNumber);

	// Set initial matricies
	Mat4 objToWorld = before->ObjToWorld;
	Mat4 viewTransform = before->ViewTransform;

	// Calculate delta interpolation
	Vec4 transform = Vec4(0.0, 0.0, 0.0, 1.0) * (1.0 - t) + after->Translation * t;
	Vec4 scale = Vec4(1.0, 1.0, 1.0, 1.0) * (1.0 - t) + after->Scale * t;
	Vec4 rotation = Vec4(0.0, 0.0, 0.0, 1.0) * (1.0 - t) + after->Rotation * t;

	// Multiply according to coordinates space and action
	if (after->ObjectSpace)
	{
		if (after->Action[0])
			objToWorld = Mat4::Translate(transform) * objToWorld;
		else if (after->Action[1])
			objToWorld = Mat4::Scale(scale) * objToWorld;
		else
			objToWorld = Mat4::RotateX(rotation[0]) * Mat4::RotateY(rotation[1])
			* Mat4::RotateZ(rotation[2]) * objToWorld;
	}
	else
	{
		if (after->Action[0])
			viewTransform = viewTransform * Mat4::Translate(transform);
		else if (after->Action[1])
		{
			Mat4 S = Mat4::Scale(scale);
			if (after->AroundEye)
			{
				CameraParameters camParams = Scene::GetInstance().GetCamera()->GetCameraParameters();
				viewTransform = viewTransform * Mat4::Translate(camParams.EyeCam) * S *
					Mat4::Translate(-camParams.EyeCam);
			}
			else
				viewTransform = viewTransform * S;
		}
		else
		{
			Mat4 R = Mat4::RotateX(rotation[0])
				* Mat4::RotateY(rotation[1]) * Mat4::RotateZ(rotation[2]);
			if (after->AroundEye)
			{
				CameraParameters camParams = Scene::GetInstance().GetCamera()->GetCameraParameters();
				viewTransform = viewTransform * Mat4::Translate(camParams.EyeCam) * R *
					Mat4::Translate(-camParams.EyeCam);
			}
			else
				viewTransform = viewTransform * R;
		}
	}

	// Create the frame to return
	result->ObjToWorld = objToWorld;
	result->ViewTransform = viewTransform;
	result->FrameNumber = frame;

	return result;
}

Frame* Animation::GetFrameBezierInterpolation(int frameNum) const
{
	double t = frameNum / (double)maxFrame;
	Mat4 sumModel(0.0);
	Mat4 sumView(0.0);
	int n = keyFrames.size() - 1;

	for (unsigned int i = 0; i < keyFrames.size(); i++)
	{
		double temp1 = Factorial(n);
		double temp2 = pow(1.0 - t, n - i);
		double temp3 = pow(t, i);
		double temp4 = Factorial(i) * Factorial(n - i);

		double fact = (temp1 * temp2 * temp3) / temp4;

		sumModel = sumModel + keyFrames[i]->ObjToWorld * fact;
		sumView = sumView + keyFrames[i]->ViewTransform * fact;
	}
	sumModel[3][3] = 1.0;
	sumView[3][3] = 1.0;

	Frame* frame = new Frame();

	if (isOnlyTranslation)
	{
		frame->ObjToWorld = keyFrames[0]->ObjToWorld;
		frame->ViewTransform = keyFrames[0]->ViewTransform;
		// extract out the translation part of the interpolated matriceis
		// and implent it in the original model and cam matricies
		frame->ObjToWorld[3] = sumModel[3];
		frame->ViewTransform[3] = sumView[3];
	}
	else
	{
		frame->ObjToWorld = sumModel;
		frame->ViewTransform = sumView;
	}
	frame->FrameNumber = frameNum;

	return frame;
}

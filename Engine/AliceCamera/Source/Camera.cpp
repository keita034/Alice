#include "Camera.h"

IWindowsApp* Camera::sWindowsApp = nullptr;
AliceMathF::Matrix4* Camera::projectionMatrixPtr = nullptr;
AliceMathF::Matrix4* Camera::viewMatrixPtr = nullptr;
Camera* Camera::cameraPtr = nullptr;



void Camera::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWindowsApp = windowsApp_;
}

AliceMathF::Matrix4* Camera::GetViewMatrixPtr()
{
	return viewMatrixPtr;
}

AliceMathF::Matrix4* Camera::GetProjectionMatrixPtr()
{
	return projectionMatrixPtr;
}

Camera* Camera::GetCameraPtr()
{
	return cameraPtr;
}

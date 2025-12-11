#include "Camera.h"

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
	this->eye = eye;

	// create camera axis
	//this->forward = normalized direction vector (target <-- eye)
	//this->right = normalized vector from the cross product of the forward and up vector
	//this->up = normalized vector from the cross product of the right and forward vector

	forward = glm::normalize(target - eye);
	right = glm::normalize(glm::cross(up, forward));
	this->up = glm::normalize(glm::cross(right, forward));

	CalculateViewPlane();
}

Ray Camera::GetRay(const glm::vec2& uv) const {
	Ray ray;

	ray.origin = eye;
	ray.direction = lowerLeft + horizontal * uv.x + vertical * uv.y - eye;

	return ray;
}

void Camera::CalculateViewPlane() {
	//float theta = convert fov (degrees) to radians
	float theta = fov * 3.14159265f / 180.0f;

	//float halfHeight = trig function that is opposite over adjacent, use half theta as parameter
	//float halfWidth = scale halfHeight by aspect ratio
	float halfHeight = tan(theta / 2.0f);
	float halfWidth = aspectRatio * halfHeight;

	//horizontal = right vector * width (how do you get full width from half width?)
	//vertical = up vector * height (how do you get full height from half height?)
	horizontal = right * (2.0f * halfWidth);
	vertical = up * (2.0f * halfHeight);

	//lowerLeft = eye - (half horizontal) - (half vertical) + forward;
	lowerLeft = eye - (horizontal * 0.5f) - (vertical * 0.5f) + forward;
}


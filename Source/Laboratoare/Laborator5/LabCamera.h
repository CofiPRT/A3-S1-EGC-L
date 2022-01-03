#pragma once
#include <include/glm.h>
#include <include/math.h>

#define OX_AXIS (glm::vec3(1, 0, 0))
#define OY_AXIS (glm::vec3(0, 1, 0))
#define OZ_AXIS (glm::vec3(0, 0, 1))

namespace Laborator
{
	class Camera
	{
		public:
			Camera()
			{
				position = glm::vec3(0, 2, 5);
				forward = glm::vec3(0, 0, -1);
				up		= glm::vec3(0, 1, 0);
				right	= glm::vec3(1, 0, 0);
				distanceToTarget = 2;
			}

			Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				Set(position, center, up);
			}

			~Camera()
			{ }

			// Update camera
			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				this->position = position;
				forward = glm::normalize(center-position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			void MoveForward(float distance)
			{
				position += glm::normalize(glm::vec3(forward.x, 0, forward.z)) * distance;
			}

			void TranslateForward(float distance)
			{
				position += glm::normalize(forward) * distance;
			}

			void TranslateUpword(float distance)
			{
				position += glm::normalize(up) * distance;
			}

			void TranslateRight(float distance)
			{
				position += glm::normalize(right) * distance;
			}

			void RotateFirstPerson_OX(float angle)
			{
				glm::vec4 newForward = glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1);

				// Compute the new "forward" and "up" vectors
				forward = glm::normalize(glm::vec3(newForward));
				up = glm::cross(right, forward);
			}

			void RotateFirstPerson_OY(float angle)
			{
				glm::vec4 newForward = glm::rotate(glm::mat4(1), angle, OY_AXIS) * glm::vec4(forward, 1);
				glm::vec4 newRight = glm::rotate(glm::mat4(1), angle, OY_AXIS) * glm::vec4(right, 1);

				// Compute the new "forward", "right" and "up" vectors
				forward = glm::normalize(glm::vec3(newForward));
				right = glm::normalize(glm::vec3(newRight));
				up = glm::cross(right, forward);
			}

			void RotateFirstPerson_OZ(float angle)
			{
				glm::vec4 newUp = glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(up, 1);

				// Compute the new "up" and "right" vectors
				up = glm::normalize(glm::vec3(newUp));
				right = glm::cross(forward, up);
			}

			void RotateThirdPerson_OX(float angle)
			{
				// Rotate the camera in Third Person mode - OX axis
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OX(angle);
				TranslateForward(-distanceToTarget);
			}

			void RotateThirdPerson_OY(float angle)
			{
				// Rotate the camera in Third Person mode - OY axis
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OY(angle);
				TranslateForward(-distanceToTarget);
			}

			void RotateThirdPerson_OZ(float angle)
			{
				// Rotate the camera in Third Person mode - OZ axis
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OZ(angle);
				TranslateForward(-distanceToTarget);
			}

			glm::mat4 GetViewMatrix()
			{
				// Returns the View Matrix
				return glm::lookAt(position, position + forward, up);
			}

			glm::vec3 GetTargetPosition()
			{
				return position + forward * distanceToTarget;
			}

		public:
			float distanceToTarget;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
		};
}
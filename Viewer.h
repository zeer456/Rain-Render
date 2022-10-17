#pragma once

#include <glm/glm.hpp>

/**
 * \brief A class to control the viewing parameters.
 *
 * This class maintains and provides access to parameters of a simple viewing model, 
 * and it supports basic viewing operations like translation, rotation, and zooming.
 * The view parameters are:
 *		- The view point (where the camera is located)
 *		- The view center (a point that is being looked at; the closer it is to the
 *		   view point, the greater the degree of zooming)
 *		- The up vector (defines the global vertical axis; typically this is the y axis)
 *		- The field of view (defined as the vertical angular span of the viewing
 *		   frustrum in radians
 *		- The aspect ratio (the ratio width/height for the resultant image)
*/

class Viewer {
	public:

		Viewer( 
			const glm::vec3 &viewPoint, const  glm::vec3 &viewCenter, const  glm::vec3 &upVector,
			float fieldOfView, float aspectRatio
		);

		glm::vec3 getViewPoint() const;

		glm::vec3 getViewCenter() const;

		glm::vec3 getUpVector() const;
		float getFieldOfView() const;
		float getAspectRatio() const;

		// Diplay Plane Widht [Far]
		float getDisplayWidth() const;
		
		// Display Plane Height [Far]
		float getDisplayHeight() const;
		
		float length_far;

		glm::vec3 getViewDir() const;

		glm::vec3 getImagePlaneHorizDir() const;

		glm::vec3 getImagePlaneVertDir() const;

		/** 
		 * Translate
		 *
		 * These methods alter the view based on mouse movement.  The arguments
		 * changeHoriz and changeVert specify, respectively, the amount
		 * the mouse has moved horizontally/vertically as a fraction of the
		 * total screen width/height.
		 *
		 * translate: The final argument determines whether the translation is
		 * parallel to the view plane; if not, then only the vertical mouse 
		 * movement (second argument) is used.  The actual amount of 
		 * translation is determined by the distance between the view center 
		 * and the view point.
		 *
		*/
		void translate(float changeHoriz, float changeVert, bool parallelToViewPlane);

		/** 
		 * Zoom
		 *
		 * These methods alter the view based on mouse movement.  The arguments
		 * changeHoriz and changeVert specify, respectively, the amount
		 * the mouse has moved horizontally/vertically as a fraction of the
		 * total screen width/height.
		 *
		 * zoom: Zoom in or out by changing the distance between the view
		 * center and the viewpoint; the view center is held fixed.  The 
		 * distance is changed by a fraction 2.0 ^ (m_zoomFraction*changeVert)
		 * of the current distance; m_zoomFraction can be altered to adjust
		 * zoom speed.
		 *
		*/
		void zoom(float changeVert);

		/** 
		 * Rotate
		 *
		 * These methods alter the view based on mouse movement.  The arguments
		 * changeHoriz and changeVert specify, respectively, the amount
		 * the mouse has moved horizontally/vertically as a fraction of the
		 * total screen width/height.
		 *
		 *
		 * rotate: Rotate about the view plane axes; the first argument is for 
		 * rotation about the vertical view axis and the second is for
		 * rotation about the horizontal view axis.  The amount of rotation
		 * is controlled by m_rotateSpeed.
		*/
		void rotate(float changeHoriz, float changeVert);

		/** 
		 * Center at
		 *
		 * These methods alter the view based on mouse movement.  The arguments
		 * changeHoriz and changeVert specify, respectively, the amount
		 * the mouse has moved horizontally/vertically as a fraction of the
		 * total screen width/height.
		 *
		 * centerAt: Centers the view at the given location without changing the
		 * global orientation or scaling.
		*/
		void centerAt(const  glm::vec3 &pos);

		/** 
		 * Look From
		*/
		void lookFrom(const  glm::vec3 &pos);

		/** Set the Field Of View [FOV] */
		void setFieldOfView(float fieldOfView);
		
		/** Set up the aspect ratio [Aspect Ratio]*/
		void setAspectRatio(float aspectRatio);
		
		/** Set up the translate speed [Translate Speed]*/
		void setTranslateSpeed(float translateSpeed);
		
		/** Set up the zoom factor [Zoom Factor]*/
		void setZoomFraction(float zoomFraction);
		
		/** Set up the roation speed [Rotation Speed]*/
		void setRotateSpeed(float rotateSpeed);

	private:

		glm::vec3 m_viewPoint;
		glm::vec3 m_viewCenter;
		glm::vec3 m_upVector;
		float m_fieldOfView;
		float m_aspectRatio;

		float m_translateSpeed;
		float m_zoomFraction;
		float m_rotateSpeed;

		glm::vec3 m_viewDir;
		glm::vec3 m_imagePlaneHorizDir;
		glm::vec3 m_imagePlaneVertDir;

		/**
		 * These member variables hold the width and height of the plane that
		 * 1) is parallel to the clipping planes
		 * 2) passes through the the view center
		 * 3) is clipped by the frustrum boundaries.
		 * This is useful for converting mouse input into view transformations
		 * of an appropriate magnitude.
		 */

		float m_displayWidth;
		float m_displayHeight;

		/** These are used for tracking */
		float m_lastDesired[3];

		/**
		 * The following values contain the geometry of our viewing volume: the
		 * field of view, the aspect ratio, the height and width of view plane 
		 * containing the center of projection, the world-coordinate directions
		 * of the image x and y axes, and the dimensions of the viewport.
		 */
		void getFrustrumInfo();
};

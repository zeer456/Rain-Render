#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

#include <iostream>
#include <string>
#include <stack>

// Third Party
#include "Loader.h"
#include "Viewer.h"
#include "ModelView.h"
#include "timing.h"

// Objects
#include "Plane.h"
#include "Particle.h"
#include "Mesh.h"

// ImGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#pragma warning(pop)

class Engine;

class Engine {
public:
	Engine(int w, int h);
	~Engine();
	void draw();
	void update();
	void setSize(int w, int h) { m_width = w; m_height = h; }
	void setAspect(float r) { m_viewer->setAspectRatio(r); }

	Viewer *m_viewer;
	float m_rotate;
private:
	int m_width;
	int m_height;
	void initialize();

	void drawPlane(ShaderProgram * shader, glm::mat4 & modelview, glm::mat3 & normalMatrix, glm::mat4 & mvp);
	void drawPlane(ShaderProgram * shader, glm::mat4 & view, glm::mat4 & projection, glm::vec3 eye);
	void drawStreetLamp(ShaderProgram * shader, glm::mat4 & view, glm::mat4 & projection, glm::vec3 eye);

	Model m_model;
	Plane *m_plane;
	Mesh *m_streetLamp;

	float pTime, deltaT;

	glm::vec3 camera_dir;

	glm::vec3 v_right;

	glm::vec3 far_center;
	glm::vec3 far_bottom_right;
	glm::vec3 far_bottom_left;
	glm::vec3 far_top_right;
	glm::vec3 far_top_left;

	glm::vec3 near_center;
	glm::vec3 near_bottom_right;
	glm::vec3 near_bottom_left;
	glm::vec3 near_top_right;
	glm::vec3 near_top_left;

	glm::vec3 v_U, v_V, side_V;

	// Plane [Right]
	glm::vec3 nbr_fbr, nbr_ntr;
	glm::vec3 right_plane_vector;
	
	// Plane [Left]
	glm::vec3 nbl_fbl, nbl_ntl;
	glm::vec3 left_plane_vector;
	
	// Plane [Bottom]
	glm::vec3 nbl_nbr;
	glm::vec3 bottom_plane_vector;
	
	// Plane [Top]
	glm::vec3 ntl_ntr, ntl_ftl;
	glm::vec3 top_plane_vector;
	
	float far_height, far_width;
	float near_height, near_width;
	float fov;
	float near, /*far,*/ far_particle;

	glm::vec3 frustum_center;
	float frustum_radius;

public:

	// Shaders
	ShaderProgram * m_planeShader;
	ShaderProgram * m_particleShader;
	ShaderProgram * m_streetLampShader;

	// Particle
	Particle *m_particles;

	// Light Attributes
	glm::vec4 spotLightPos;			
	glm::vec3 spotLightIntensity;	
	glm::vec3 spotLightColor;		

	// Light Attributes
	float spotExponent;		
	float spotInnerCutoff;	
	float spotOuterCutoff;	

	// Light Attributes
	glm::vec3 spotLightAmbient;		
	glm::vec3 spotLightDiffuse;		
	glm::vec3 spotLightSpecular;	
};


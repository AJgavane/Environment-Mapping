#pragma once

#define WIDTH 1280	
#define HEIGHT 720
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024
#define HEIGHT_BY_2 512
#define QUERY_BUFFERS 2
#define QUERY_COUNT 1

Display display(WIDTH, HEIGHT, "Environment Mapping");

GLuint queryID_VIR[QUERY_BUFFERS][QUERY_COUNT];
int queryBackBuffer = 0, queryFrontBuffer = 1;
GLuint queryID_lightPass[QUERY_BUFFERS][QUERY_COUNT];

/**************Camera ********************/
bool printCameraCoord = true;
const glm::vec3 cameraDefaultPosition(5.0f, 6.0f, 8.0f);
const glm::vec3 lookAtDefault(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPosition(0.290603, 0.70916, -0.306382);
glm::vec3 lookAt(1.89082, -4.52584, 6.43899);
glm::vec3 Up(0.0f, 1.0f, 0.0f);
glm::vec3 front = glm::normalize(cameraPosition - lookAt);
glm::vec3 side = glm::normalize(cameraPosition - lookAt);
float fov = glm::radians(60.0f);
float defaultFOV = fov;// glm::radians(90.0f);
float zNear = 0.10f;
float zFar = 100.0;
float counter = 0.0f;
float bbox = 0.750f;

/**************Lights********* ***/
glm::vec3 lightPosition(1.3, 1.0, -2.196);
glm::vec3 lightLookAt(-1.400, -5.613, 11.009);
glm::vec3 defaultLightPosition(5.0f, 10.0f, 10.0f);
float lightFOV = glm::radians(45.0f);
float l_zNear = 01.0f;
float LIGHT_SIZE = 0.02f;
float LIGHT_STEP_SIZE = LIGHT_SIZE / 10;
float FrustumWidth = 2 * std::tanf(lightFOV * 0.5f);
float FrustumHeight = FrustumWidth;


float dt = 0.1f;
// FPS parameters
double lastTime = 0;
int numFrames = 1;
int avgNumFrames = 628;
bool runtime = false; bool csv = true;
bool CountNumberOfPoints = true;
bool debug = false;

GLuint64 virTime;
float avgVIR_time = 0.0f;

GLuint quadVAO = 0;
GLuint quadVBO;



/**************************************/
GLuint m_shadowMapFBO = 0;

GLuint depthTexture;

static const GLint ShadowDepthTextureUnit = 0;
static const GLint ShadowPcfTextureUnit = 1;
static const GLint NumTextureUnits = 2;

GLuint m_samplers[NumTextureUnits];
GLuint m_textures[NumTextureUnits];

/*************************************/
GLuint cubemapTexture;
GLuint cubemapTexture_temp;
GLuint cm_fbo; GLuint cm_depthBuffer;
enum
{
	POSITIVE_X = 0,
	NEGATIVE_X = 1,
	POSITIVE_Y = 2,
	NEGATIVE_Y = 3,
	POSITIVE_Z = 4,
	NEGATIVE_Z = 5
};
GLuint cubeVAO, cubeVBO;
GLuint skyboxVAO, skyboxVBO;
float cubeVertices[] = {
	// positions          // normals
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces
{
	std::string("./res/skybox/right.jpg"),
	std::string("./res/skybox/left.jpg"),
	std::string("./res/skybox/top.jpg"),
	std::string("./res/skybox/bottom.jpg"),
	std::string("./res/skybox/front.jpg"),
	std::string("./res/skybox/back.jpg"),
};

std::vector<std::string> faces_temp
{
	std::string("./res/skybox/+x.jpg"),
	std::string("./res/skybox/-x.jpg"),
	std::string("./res/skybox/+y.jpg"),
	std::string("./res/skybox/-y.jpg"),
	std::string("./res/skybox/-z.jpg"),
	std::string("./res/skybox/+z.jpg"),
};
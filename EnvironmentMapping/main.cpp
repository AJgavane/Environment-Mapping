#include <iostream>
#include<vector>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "model.h"
#include "camera.h"
#include "Constants.h"
#include "HandelKeys.h"
#include <GL/GL.h>
#include <GL/glext.h>

void genQueries(GLuint qid[][1]);
void swapQueryBuffers();

void DrawQuadGL();

void initRendering();
void initCube();
void initSkyBox();
GLuint initCubeMap();
GLuint loadCubeMap(std::vector<std::string> faces);

void printVec(const std::string& str, const glm::vec3& v);


int main(int args, char** argv)
{
    float dTheta = 0.001;

	Model floor("./res/models/floor/floor.obj");
    glm::mat4 modelFloor;
    modelFloor = glm::translate(modelFloor, glm::vec3(0.00f, -1.00f, 1.00f));
    modelFloor = glm::translate(modelFloor, -floor.GetCenter());
    modelFloor = glm::scale(modelFloor, glm::vec3(0.30f, 1.0f, 0.3f));	// it's a bit too big for our scene, so scale it down

	Model wall("./res/models/wall/wall.obj");
	glm::mat4 modelWall;
	modelWall = glm::rotate(modelWall, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelWall = glm::translate(modelWall, glm::vec3(0.0, 3.5, -1.0  ));
	modelWall = glm::scale(modelWall, glm::vec3(0.30f, 1.0, 0.3));	// it's a bit too big for our scene, so scale it down

	glm::mat4 modelScene;
	Model scene("./res/models/sphere/sphere.obj");
	modelScene = glm::translate(modelScene, glm::vec3(0.50f, 1.800f, 2.0f));
	runtime = true; avgNumFrames = int(glm::radians(360.0f) * 20); csv = true;//  dTheta = 0;

	glm::mat4 ModelLight;
	Model light("./res/models/square/square.obj");
	ModelLight = glm::mat4();
	ModelLight = glm::scale(ModelLight, glm::vec3(LIGHT_SIZE*10.0f, LIGHT_SIZE*10.0f, 1.0f));	
	ModelLight = glm::translate(ModelLight, lightPosition);

	glm::mat4 ModelSphere;
	Model sphere("./res/models/sphere/sphere.obj");
	glm::vec3 renderPosition = sphere.GetCenter(); std::cout << renderPosition.x << ", " << renderPosition.y << ", " << renderPosition.z << std::endl;
	ModelSphere = glm::mat4();
	ModelSphere = glm::translate(ModelSphere, glm::vec3(0.0));
	renderPosition = glm::vec3(ModelSphere * glm::vec4(renderPosition, 1.0));
	printVec("renderPosition: ", renderPosition);
	// Load Shader for Environment mapping
	Shader cubemapShader("./res/cubemap.vs", "./res/cubemap.fs");
	cubemapShader.use();
	cubemapShader.setInt("skybox", 0);
	cubemapShader.setInt("dynSkybox", 1);
	cubemapShader.disable();

	Shader skyboxShader("./res/skybox.vs", "./res/skybox.fs");
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	skyboxShader.setInt("dynSkybox", 1);
	skyboxShader.disable();

	Shader dynCubeMapShader("./res/dyCubeMap.vs", "./res/dyCubeMap.fs");
	dynCubeMapShader.use();
	skyboxShader.setInt("skybox", 0);
	dynCubeMapShader.setInt("dynSkybox", 1);
	dynCubeMapShader.disable();

	glm::mat4 ModelSkybox = glm::mat4();

	initRendering();

	// Create fbo and depthbuffer, and attack depth buffer to fbo
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);		// hardcoded
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubemap, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "..Error in creating frame buffer" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	/* End*/
	
	glEnable(GL_CULL_FACE);
	genQueries(queryID_VIR);
	genQueries(queryID_lightPass);
	lastTime = SDL_GetTicks();
	
	// Render:
    while (!display.isClosed())
	{
		if (printCameraCoord) {
			printVec("CameraPos: ", cameraPosition);
			printVec("Center ", lookAt);
			printCameraCoord = !printCameraCoord;
		}
		numFrames++;
        glFinish();
        handleKeys();
    	
        Camera camera(cameraPosition, fov, (float)WIDTH, (float)HEIGHT, zNear, zFar, lookAt, bbox);
        glm::mat4 projection = camera.GetPerspProj();
        glm::mat4 view = camera.GetView();
		//glm::mat4 viewProj = camera.GetPersViewProj();
    	
        display.Clear(0.0f, 0.0f, 0.0f, 1.0f);
    	if(dTheta > 0)
			modelScene = glm::rotate(modelScene, glm::sin(dTheta), glm::vec3(0.0f, 1.0f, 1.0f)); // If dTheta is non-zero.
		glBeginQuery(GL_TIME_ELAPSED, queryID_VIR[queryBackBuffer][0]);
    	
		/* Environment Mapping */
		glEnable(GL_DEPTH_TEST);
		// 1. REnder the scene as normal
		//glViewport(0, 0, WIDTH, HEIGHT);
		cubemapShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		cubemapShader.setVec3("cameraPos", cameraPosition);
		cubemapShader.setMat4("u_projection", projection);
		cubemapShader.setMat4("u_view", view);
		cubemapShader.setMat4("u_model", modelScene);
		scene.Draw(cubemapShader);
		cubemapShader.setMat4("u_model", ModelSphere);
		sphere.Draw(cubemapShader);
		cubemapShader.disable();//*/

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		for(int face = 0; face < 6; face++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)face, cubemap, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "..Error in creating frame buffer inside rendering loop!" << std::endl;

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glm::mat4 persp_cm = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 100.0f);
			glm::mat4 view_cm = glm::mat4();
			switch (face)
			{
				case POSITIVE_X:
					view_cm = glm::lookAt(glm::vec3(renderPosition), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
					break;

				case NEGATIVE_X:
					view_cm = glm::lookAt(glm::vec3(renderPosition), glm::vec3(01.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
					break;

				case POSITIVE_Y:
					view_cm = glm::lookAt(glm::vec3(renderPosition), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
					break;

				case NEGATIVE_Y:
					view_cm = glm::lookAt(glm::vec3(renderPosition), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
					break;

				case POSITIVE_Z:
					view_cm = glm::lookAt(glm::vec3(renderPosition), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
					break;

				case NEGATIVE_Z:
					view_cm = glm::lookAt(glm::vec3(renderPosition), glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
					break;

				default:
					break;
			};
			view_cm = glm::translate(view_cm, -renderPosition);
			//view_cm = glm::mat4(glm::mat3(view_cm));;
			dynCubeMapShader.use();
			dynCubeMapShader.setVec3("cameraPos", cameraPosition);
			dynCubeMapShader.setMat4("u_view", view_cm);
			dynCubeMapShader.setMat4("u_projection", persp_cm);
				// scene
			//glm::mat4 modelScene_cm = glm::mat4(glm::mat3(modelScene));
			dynCubeMapShader.setMat4("u_model", modelScene);
			scene.Draw(dynCubeMapShader);
		}
		dynCubeMapShader.disable();
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //*/

		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		//glViewport(0, 0, WIDTH, HEIGHT);
		glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
		skyboxShader.setMat4("u_view", skyboxView);
		skyboxShader.setMat4("u_projection", projection);
		skyboxShader.setMat4("u_model", glm::mat4());
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		skyboxShader.disable();
		glDepthFunc(GL_LESS);
    	   	//*/
		glEndQuery(GL_TIME_ELAPSED);
        /*****************************************
         * Display time and number of points     *
        /*****************************************/
        glGetQueryObjectui64v(queryID_VIR[queryFrontBuffer][0], GL_QUERY_RESULT, &virTime);
        avgVIR_time += virTime / 1000000.0;
        if (numFrames % avgNumFrames == 0 && runtime) {
			std::cout << "PCSS: " << avgVIR_time / avgNumFrames;
			std::cout << "\tLIGHT SIZE: " << LIGHT_SIZE;
            std::cout << std::endl;
			avgVIR_time = 0;
        }		
        swapQueryBuffers();
        display.Update();        
    }

	// Free the buffers
	glDeleteFramebuffers(1, &m_shadowMapFBO);
	glDeleteTextures(1, &depthTexture);
	for(int i = 0; i < NumTextureUnits; i++)
	{
		glDeleteTextures(1, &m_textures[i]);
		glDeleteTextures(1, &m_samplers[i]);
	}
    return 0;
}

void initRendering()
{
	initCube();
	initSkyBox();
	cubemapTexture = loadCubeMap(faces);
	cubemap = initCubeMap();
}	


void initCube()
{
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void initSkyBox()
{
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

GLuint loadCubeMap(std::vector<std::string> faces)
{
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	int width, height, nrComponents;
	for(unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		
		std::cout << faces[i] << ": " << width << ", " << height << std::endl;
		if(data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texID;
}

GLuint initCubeMap()
{
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	for(int i =0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return texID;
}

void DrawQuadGL()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

// call this function when initializating the OpenGL settings
void genQueries(GLuint qid[][1]) {

    glGenQueries(1, qid[queryBackBuffer]);
    glGenQueries(1, qid[queryFrontBuffer]);

    // dummy query to prevent OpenGL errors from popping out
    glQueryCounter(qid[queryFrontBuffer][0], GL_TIME_ELAPSED);
}

// aux function to keep the code simpler
void swapQueryBuffers() {

    if (queryBackBuffer) {
        queryBackBuffer = 0;
        queryFrontBuffer = 1;
    }
    else {
        queryBackBuffer = 1;
        queryFrontBuffer = 0;
    }
}

void printVec(const std::string& str, const glm::vec3& v)
{
	std::cout << str << " {" << v[0] << ", " << v[1] << ", " << v[2] << "}" << std::endl;
}

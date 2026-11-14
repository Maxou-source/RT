#include <string>
#include <iostream>
#include <stdlib.h>
#include "Sphere.hpp"
#include "Ray.hpp"
#include "parse.hpp"
#include "Matrix.hpp"
#include "rt.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include "Light.hpp"
#include "World.hpp"
#include "Intersection.hpp"
#include "Computations.hpp"
#include "Camera.hpp"
#include "OpenGLObject.hpp"
#include <string>

#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>

// GpuScenePrinter.hpp / .cpp
#include <iostream>
#include <iomanip>



class FPSCounter {
public:
    void update() {
        frames++;
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - lastTime).count();
        if (elapsed >= 1.0) {
            fps = frames / elapsed;
            frames = 0;
            lastTime = now;
			std::cout << "FPS: " << fps << std::endl;
        }
    }

    double getFPS() const { return fps; }

private:
    int frames = 0;
    double fps = 0.0;
    std::chrono::high_resolution_clock::time_point lastTime =
        std::chrono::high_resolution_clock::now();
};

void debugMessages(const OpenGLObject& gl)
{
	GpuScene back{};
	glBindBuffer(GL_UNIFORM_BUFFER, gl.getUbo());          // add a getUbo() getter
	glGetBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GpuScene), &back);

	std::cout << "\nshaderProgram: " << gl.getComputeProgram() << std::endl;
	std::cout << "half_width:  " << back.cam.half_width  << "\n"
	          << "half_height: " << back.cam.half_height << "\n"
	          << "pixel_size:  " << back.cam.pixel_size  << "\n"
	          << "objects: " << back.object_count
	          << "  lights: " << back.light_count << std::endl;

	std::cout << "inv_transform:\n";
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
			std::cout << back.cam.inv_transform[r][c] << ", ";
		std::cout << "\n";
	}
}

void printMat4(const float* m, const char* name)
{
    std::cout << colorprint::green << name << colorprint::reset << std::endl;
    for (int row = 0; row < 4; ++row)
    {
        std::cout << "  | ";
        for (int col = 0; col < 4; ++col)
            std::cout << std::setw(10) << std::fixed << std::setprecision(4)
                      << m[col * 4 + row] << " ";
        std::cout << "|" << std::endl;
    }
}

// bool runGpuTest()
// {
// 	OpenGLObject	oglo;

// 	std::cout << "running tests" << std::endl;
// 	// oglo.SetVertexShader();
// 	oglo.SetComputeShader("shader/tests/cube_tests.glsl");
// 	oglo.Whatever();

// 	// 1. actually run the shader: draw the fullscreen quad once
// 	glUseProgram(oglo.getComputeProgram());
// 	glBindVertexArray(oglo.getVAO());
// 	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 	glBindVertexArray(0);

// 	// 2. make the shader's SSBO writes visible
// 	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

// 	glBindBuffer(GL_SHADER_STORAGE_BUFFER, oglo.getDebugSSBO());

// 	std::vector<float> debugOut(WIN_HEIGHT * WIN_WIDTH * 4 * 12);
// 	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, debugOut.size() * sizeof(float), debugOut.data());
// 	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
// 	int idx = 0;
// 	std::cout << "debugout 0 " << debugOut[0] << std::endl;
// 	return false;
// }


// int main()
// {
//     OpenGLObject gl;

//     // 1. Compile compute shader
// if (!gl.SetComputeShader("shader/test.glsl"))
//     return 1;

// if (!gl.Whatever())
//     return 1;

//     GLuint program = gl.getComputeProgram();

//     if (program == 0)
//     {
//         std::cerr << "KO:: Invalid compute program" << std::endl;
//         return 1;
//     }

//     // 2. Get framebuffer dimensions
//     GLFWwindow* window = gl.getWindow();

//     int width, height;
//     glfwGetFramebufferSize(window, &width, &height);

//     // 3. Create output texture and read framebuffer
//     gl.CreateOutputTexture(width, height);

//     // 4. Dispatch compute shader
//     glUseProgram(program);

//     glDispatchCompute(
//         (width + 7) / 8,
//         (height + 7) / 8,
//         1
//     );

//     // 5. Make image writes visible to framebuffer operations
//     glMemoryBarrier(
//         GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
//         GL_FRAMEBUFFER_BARRIER_BIT
//     );

//     // 6. Check one pixel
//     unsigned char pixel[4] = {};

//     glBindFramebuffer(GL_READ_FRAMEBUFFER, gl.getReadFBO());
//     glReadBuffer(GL_COLOR_ATTACHMENT0);

//     glReadPixels(
//         width / 2,
//         height / 2,
//         1, 1,
//         GL_RGBA,
//         GL_UNSIGNED_BYTE,
//         pixel
//     );

//     std::cout << "Pixel: "
//               << (int)pixel[0] << " "
//               << (int)pixel[1] << " "
//               << (int)pixel[2] << " "
//               << (int)pixel[3] << std::endl;

//     // 7. Display result
//     while (!glfwWindowShouldClose(window))
//     {
//         glfwPollEvents();

//         glBindFramebuffer(GL_READ_FRAMEBUFFER, gl.getReadFBO());
//         glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

//         glBlitFramebuffer(
//             0, 0, width, height,
//             0, 0, width, height,
//             GL_COLOR_BUFFER_BIT,
//             GL_NEAREST
//         );

//         glfwSwapBuffers(window);
//     }

//     glfwTerminate();
//     return 0;
// }


int main(int ac, char **av)
{
	/*====== GRAPHICAL TESTS======*/
	Parse	par;
	World	*w;

	// if (ac == 2 && !strcmp(av[1], "--test"))
	// 	return runGpuTest();
	if (ac != 2 || par.parseFileAndName(av[1]))
	{
		std::cerr << "KO:: Wrong number of arguments" << std::endl;
		return (1);
	}

	if (!par.getBase()) {
		std::cerr << "KO:: Something wrong with your file" << std::endl;
		return (1);
	}

	w = par.GetMyWorld();
	w->printWorld();
	if (!w->resolveTextures())                     // load files, assign layers
		return 1;
	GpuScene gpuS = w->buildGpuScene();
	// printGpuScene(gpuS);
	
	std::cout << "scene scale is " << w->sceneScale() << std::endl;
	float baseSpeed  = w->sceneScale() * 0.25f;
	float speedMul   = 1.0f;
	OpenGLObject OpenGLObj;
	
	// OpenGLObj.SetVertexShader();
	if (!OpenGLObj.SetComputeShader("shader/raytracer.glsl"))
		return (-1);
	if (!OpenGLObj.Whatever())
		return (-1);
	OpenGLObj.UploadScene(gpuS);
	if (!OpenGLObj.UploadTextures(w->getTextures()))      // GPU array + sampler uniform
		return 1;
	
	debugMessages(OpenGLObj);
	
	FPSCounter fpsCounter;
	bool one = true;
	double last = glfwGetTime();
	int fbw, fbh;
	glfwGetFramebufferSize(OpenGLObj.getWindow(), &fbw, &fbh);
	OpenGLObj.CreateOutputTexture(fbw, fbh);     // creates outTex, the image binding and the read FBO
	bool needsRender = true;
	int nw, nh;
	glfwGetFramebufferSize(OpenGLObj.getWindow(), &nw, &nh);
	if (nw != fbw || nh != fbh)
	{
		fbw = nw; fbh = nh;
		glViewport(0, 0, fbw, fbh);
		OpenGLObj.CreateOutputTexture(fbw, fbh);      // also reattaches the read FBO
		gpuS = w->buildGpuScene();
		OpenGLObj.UploadScene(gpuS);
		needsRender = true;
	}

	while (!glfwWindowShouldClose(OpenGLObj.getWindow()))
	{
		glfwPollEvents();

	// ---------- 1. time ----------
		double now = glfwGetTime();
		float  dt  = std::min(float(now - last), 0.05f);   // clamp spikes
		last = now;

		// ---------- 2. input ----------
		GLFWwindow* win = OpenGLObj.getWindow();
		float step = baseSpeed * speedMul * dt;
		float fwd = 0, rgt = 0, upw = 0;
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) fwd += step;
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) fwd -= step;
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) rgt += step;
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) rgt -= step;
		if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) upw += step;
		if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) upw -= step;

		float turnSpeed = 1.5f;
		float dYaw = 0, dPitch = 0;
		if (glfwGetKey(win, GLFW_KEY_LEFT)  == GLFW_PRESS) dYaw   -= turnSpeed * dt;
		if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) dYaw   += turnSpeed * dt;
		if (glfwGetKey(win, GLFW_KEY_UP)    == GLFW_PRESS) dPitch += turnSpeed * dt;
		if (glfwGetKey(win, GLFW_KEY_DOWN)  == GLFW_PRESS) dPitch -= turnSpeed * dt;

		// ---------- 3. camera update (one upload per frame) ----------
		bool changed = false;
		if (fwd || rgt || upw)   { w->getCamera().move(fwd, rgt, upw);      changed = true; }
		if (dYaw || dPitch)      { w->getCamera().rotate(dYaw, dPitch);     changed = true; }
		if (changed)
		{
			w->getCamera().packIntoGpuScene(gpuS);
			OpenGLObj.UploadCamera(gpuS);
		}

		// ---------- 4. render ----------
		if (changed || needsRender)                        // skip if nothing moved
		{
			glUseProgram(OpenGLObj.getComputeProgram());
			glDispatchCompute((fbw + 7) / 8, (fbh + 7) / 8, 1);

			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_FRAMEBUFFER_BARRIER_BIT);
			needsRender = false;
		}

		// ---------- 5. present ----------
		glBindFramebuffer(GL_READ_FRAMEBUFFER, OpenGLObj.getReadFBO());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, fbw, fbh, 0, 0, fbw, fbh, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glfwSwapBuffers(win);

		fpsCounter.update();
		if (one == true)
		{
			std::vector<float> debugOut(WIN_HEIGHT * WIN_WIDTH * 4 * 12);
			glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, debugOut.size() * sizeof(float), debugOut.data());
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			int px = 0, py = 0;
			while (py < WIN_HEIGHT)
			{
				while (px < WIN_WIDTH)
				{
					int idx = (py * WIN_WIDTH + px) * 4;
					if (px != 200 || py != 200)
					{
						px++;
						continue;
					}
					std::cout << colorprint::green <<"CAMERA :: " << colorprint::reset << "half_width: " << debugOut[idx] << " half_height: " << debugOut[idx+1] << " half_view: " << debugOut[idx+2] << " pixel_size: " << debugOut[idx+3] << std::endl;
					idx = idx + 4;
					printMat4(&debugOut[idx], "CAMERA TRANSFORM :: ");
					idx = idx + 16;
					printMat4(&debugOut[idx], "CAMERA TRANSFORM :: ");
					// idx = idx + 16;
					std::cout << colorprint::red << "RAY_FOR_PIXEL :: " << colorprint::reset << std::endl;
					std::cout << " world_point x " << debugOut[idx+1] << ", y" << debugOut[idx+2] << ", z" << debugOut[idx+3] << "w " << debugOut[idx+4] << std::endl;
					idx = idx + 4;
					std::cout << " r.origin x " << debugOut[idx+1] << ", y" << debugOut[idx+2] << ", z" << debugOut[idx+3] << "w " << debugOut[idx+4] << std::endl;
					idx = idx + 4;
					std::cout << " r.direction x " << debugOut[idx+1] << ", y" << debugOut[idx+2] << ", z" << debugOut[idx+3] << "w " << debugOut[idx+4] << std::endl;
					px++;
				}
				px = 0;
				py++;
			}

		}
		one = false;
	}
	glfwTerminate();
}


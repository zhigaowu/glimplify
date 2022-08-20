

#include "vertices.hpp"
#include "program.hpp"

#include "texture.hpp"

#include "camera.hpp"

#include "context.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


bool firstMouse = true;
float lastFrame = 0.0f; // Time of last frame
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, glimplify::camera& camera)
{
	float now = glfwGetTime();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.forward(now - lastFrame);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.backward(now - lastFrame);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.left(now - lastFrame);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.right(now - lastFrame);

    lastFrame = now;
}

float lastX = SCR_WIDTH >> 1, lastY = SCR_HEIGHT >> 1;

glimplify::camera camera(SCR_WIDTH, SCR_HEIGHT);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.rotate(yoffset, xoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.zoom((float)-yoffset);
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;           \n"
"layout (location = 1) in vec2 aTexCoord;      \n"
"                                              \n"
"out vec2 TexCoord;                            \n"
"uniform mat4 model;                           \n"
"uniform mat4 view;                            \n"
"uniform mat4 projection;                      \n"
"                                              \n"
"void main()                                   \n"
"{                                             \n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);             \n"
"	TexCoord = vec2(aTexCoord.x, aTexCoord.y); \n"
"}                                             \n";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec2 TexCoord;\n"
"\n"
"uniform sampler2D texture1;\n"
"uniform sampler2D texture2;\n"
"void main()\n"
"{\n"
"    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);\n"
"}\n";

void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

int main(int argc, char** argv)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	// During init, enable debug output
    glimplify::context context(MessageCallback);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    char desc[512] = { 0 };
    glimplify::program program;
    if (!program.compile(vertexShaderSource, fragmentShaderSource, 512, desc))
    {
        fprintf(stderr, "compile shader failed: %s\n", desc);
    }

    program.bind();
	program.set_uniform_1i("texture1", 0);
	program.set_uniform_1i("texture2", 1);
    program.unbind();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices_data[] = {
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices_data[] = {  // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
    };
    
    glimplify::vertices vertices(2, 5 * sizeof(float));
    
    vertices.bind();
    vertices.allocate_vertices(sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);
	vertices.allocate_index(sizeof(indices_data), indices_data, GL_STATIC_DRAW);
    vertices.format_vertices(0, GL_FLOAT, 3);
	vertices.format_vertices(1, GL_FLOAT, 2);
    vertices.unbind();

    glimplify::texture text1(0), text2(1);

	text1.bind();
	text1.wrap_mode(GL_REPEAT, GL_REPEAT);
	text1.filter_mode(GL_LINEAR, GL_LINEAR);
    text1.load("E:/images/container.jpg", true, true);
    text1.unbind();

	text2.bind();
	text2.wrap_mode(GL_REPEAT, GL_REPEAT);
	text2.filter_mode(GL_LINEAR, GL_LINEAR);
	text2.load("E:/images/awesomeface.png", true, true);
	text2.unbind();
    
    // uncomment this call to draw in wireframe polygons.
    //context.wireframe_mode();
    context.testing_depth();

    float radius = 10.0f;
    camera.move_to(glm::vec3(0.0f, 0.0f, 20.0f));

	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(55.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    // render loop
    // -----------
    lastFrame = glfwGetTime();
    while (!glfwWindowShouldClose(window))
	{
        // input
        // -----
        processInput(window, camera);

        // render
        // ------
        context.clear(0.2f, 0.3f, 0.3f, 1.0f);

        // draw our first triangle
        program.bind();
		program.set_uniform_matrix4fv("model", model);
		program.set_uniform_matrix4fv("view", camera.view_matrix());
		program.set_uniform_matrix4fv("projection", camera.perspective_matrix());

        text1.bind();
        text2.bind();

        vertices.bind(); 

        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		vertices.unbind();

		text2.unbind();
		text1.unbind();

        program.unbind();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    scanf("%d", &argc);

    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

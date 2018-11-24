//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "drawcall.h"

#include "PerlinNoise.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "shader_m.h"

#include "ImportSceneObject.h"

//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <random>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

static const GLsizei WIDTH = 1270, HEIGHT = 720; //размеры окна
static int filling = 0;
static bool keys[1024]; //массив состояний кнопок - нажата/не нажата
static GLfloat lastX = 400, lastY = 300; //исходное положение мыши
static bool firstMouse = true;
static bool g_captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
static bool g_capturedMouseJustNow = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Camera camera(float3(0.0f, 5.0f, 30.0f));

//функция для обработки нажатий на кнопки клавиатуры
void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//std::cout << key << std::endl;
	switch (key)
	{
	case GLFW_KEY_ESCAPE: //на Esc выходим из программы
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_SPACE: //на пробел переключение в каркасный режим и обратно
		if (action == GLFW_PRESS)
		{
			if (filling == 0)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				filling = 1;
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				filling = 0;
			}
		}
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
  case GLFW_KEY_B:
    do_a_barrel_roll();
    break;
	default:
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

//функция для обработки клавиш мыши
void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    g_captureMouse = !g_captureMouse;


  if (g_captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    g_capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

//функция для обработки перемещения мыши
void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = float(xpos);
    lastY = float(ypos);
    firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - lastX;
  GLfloat yoffset = lastY - float(ypos);  

  lastX = float(xpos);
  lastY = float(ypos);

  if (g_captureMouse)
    camera.ProcessMouseMove(xoffset, yoffset);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(GLfloat(yoffset));
}

void doCameraMovement(Camera &camera, GLfloat deltaTime)
{
  if (keys[GLFW_KEY_W])
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[GLFW_KEY_A])
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_S])
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[GLFW_KEY_D])
    camera.ProcessKeyboard(RIGHT, deltaTime);
}


/**
\brief создать triangle strip плоскость и загрузить её в шейдерную программу
\param rows - число строк
\param cols - число столбцов
\param size - размер плоскости
\param vao - vertex array object, связанный с созданной плоскостью
*/
static int createTriStrip(int rows, int cols, float size, GLuint &vao)
{

  int numIndices = 2 * cols*(rows - 1) + rows - 1;

  std::vector<GLfloat> vertices_vec; //вектор атрибута координат вершин
  vertices_vec.reserve(rows * cols * 3);

  std::vector<GLfloat> normals_vec; //вектор атрибута нормалей к вершинам
  normals_vec.reserve(rows * cols * 3);

  std::vector<GLfloat> texcoords_vec; //вектор атрибут текстурных координат вершин
  texcoords_vec.reserve(rows * cols * 2);

  std::vector<float3> normals_vec_tmp(rows * cols, float3(0.0f, 0.0f, 0.0f)); //временный вектор нормалей, используемый для расчетов

  std::vector<int3> faces;         //вектор граней (треугольников), каждая грань - три индекса вершин, её составляющих; используется для удобства расчета нормалей
  faces.reserve(numIndices / 3);

  std::vector<GLuint> indices_vec; //вектор индексов вершин для передачи шейдерной программе
  indices_vec.reserve(numIndices);

  for (int z = 0; z < rows; ++z)
  {
    for (int x = 0; x < cols; ++x)
    {
      //вычисляем координаты каждой из вершин 
      float xx = -size / 2 + x*size / cols;
      float zz = -size / 2 + z*size / rows;
      
      vertices_vec.push_back(xx);
      vertices_vec.push_back(0);
      vertices_vec.push_back(zz);

      texcoords_vec.push_back(x / float(cols - 1)); // вычисляем первую текстурную координату u, для плоскости это просто относительное положение вершины
      texcoords_vec.push_back(z / float(rows - 1)); // аналогично вычисляем вторую текстурную координату v
    }
  }

  //primitive restart - специальный индекс, который обозначает конец строки из треугольников в triangle_strip
  //после этого индекса формирование треугольников из массива индексов начнется заново - будут взяты следующие 3 индекса для первого треугольника
  //и далее каждый последующий индекс будет добавлять один новый треугольник пока снова не встретится primitive restart index

  int primRestart = cols * rows;

  for (int x = 0; x < cols - 1; ++x)
  {
    for (int z = 0; z < rows - 1; ++z)
    {
      int offset = x*cols + z;

      //каждую итерацию добавляем по два треугольника, которые вместе формируют четырехугольник
      if (z == 0) //если мы в начале строки треугольников, нам нужны первые четыре индекса
      {
        indices_vec.push_back(offset + 0);
        indices_vec.push_back(offset + rows);
        indices_vec.push_back(offset + 1);
        indices_vec.push_back(offset + rows + 1);
      }
      else // иначе нам достаточно двух индексов, чтобы добавить два треугольника
      {
        indices_vec.push_back(offset + 1);
        indices_vec.push_back(offset + rows + 1);

        if (z == rows - 2) indices_vec.push_back(primRestart); // если мы дошли до конца строки, вставляем primRestart, чтобы обозначить переход на следующую строку
      }
    }
  }

  ///////////////////////
  //формируем вектор граней(треугольников) по 3 индекса на каждый
  int currFace = 1;
  for (int i = 0; i < indices_vec.size() - 2; ++i)
  {
    int3 face;

    int index0 = indices_vec.at(i);
    int index1 = indices_vec.at(i + 1);
    int index2 = indices_vec.at(i + 2);

    if (index0 != primRestart && index1 != primRestart && index2 != primRestart)
    {
      if (currFace % 2 != 0) //если это нечетный треугольник, то индексы и так в правильном порядке обхода - против часовой стрелки
      {
        face.x = indices_vec.at(i);
        face.y = indices_vec.at(i + 1);
        face.z = indices_vec.at(i + 2);

        currFace++;
      }
      else //если треугольник четный, то нужно поменять местами 2-й и 3-й индекс;
      {    //при отрисовке opengl делает это за нас, но при расчете нормалей нам нужно это сделать самостоятельно
        face.x = indices_vec.at(i);
        face.y = indices_vec.at(i + 2);
        face.z = indices_vec.at(i + 1);

        currFace++;
      }
      faces.push_back(face);
    }
  }


  ///////////////////////
  //расчет нормалей
  for (int i = 0; i < faces.size(); ++i)
  {
    //получаем из вектора вершин координаты каждой из вершин одного треугольника
    float3 A(vertices_vec.at(3 * faces.at(i).x + 0), vertices_vec.at(3 * faces.at(i).x + 1), vertices_vec.at(3 * faces.at(i).x + 2));
    float3 B(vertices_vec.at(3 * faces.at(i).y + 0), vertices_vec.at(3 * faces.at(i).y + 1), vertices_vec.at(3 * faces.at(i).y + 2));
    float3 C(vertices_vec.at(3 * faces.at(i).z + 0), vertices_vec.at(3 * faces.at(i).z + 1), vertices_vec.at(3 * faces.at(i).z + 2));

    //получаем векторы для ребер треугольника из каждой из 3-х вершин
    float3 edge1A(normalize(B - A));
    float3 edge2A(normalize(C - A));

    float3 edge1B(normalize(A - B));
    float3 edge2B(normalize(C - B));

    float3 edge1C(normalize(A - C));
    float3 edge2C(normalize(B - C));

    //нормаль к треугольнику - векторное произведение любой пары векторов из одной вершины
    float3 face_normal = cross(edge1A, edge2A);

    //простой подход: нормаль к вершине = средняя по треугольникам, к которым принадлежит вершина
    normals_vec_tmp.at(faces.at(i).x) += face_normal;
    normals_vec_tmp.at(faces.at(i).y) += face_normal;
    normals_vec_tmp.at(faces.at(i).z) += face_normal;
  }

  //нормализуем векторы нормалей и записываем их в вектор из GLFloat, который будет передан в шейдерную программу
  for (int i = 0; i < normals_vec_tmp.size(); ++i)
  {
    float3 N = normalize(normals_vec_tmp.at(i));

    normals_vec.push_back(N.x);
    normals_vec.push_back(N.y);
    normals_vec.push_back(N.z);
  }


  GLuint vboVertices, vboIndices, vboNormals, vboTexCoords;

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vboVertices);
  glGenBuffers(1, &vboIndices);
  glGenBuffers(1, &vboNormals);
  glGenBuffers(1, &vboTexCoords);


  glBindVertexArray(vao); GL_CHECK_ERRORS;
  {

    //передаем в шейдерную программу атрибут координат вершин
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices); GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, vertices_vec.size() * sizeof(GL_FLOAT), &vertices_vec[0], GL_STATIC_DRAW); GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(0); GL_CHECK_ERRORS;

    //передаем в шейдерную программу атрибут нормалей
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals); GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, normals_vec.size() * sizeof(GL_FLOAT), &normals_vec[0], GL_STATIC_DRAW); GL_CHECK_ERRORS;
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(1); GL_CHECK_ERRORS;

    //передаем в шейдерную программу атрибут текстурных координат
    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords); GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, texcoords_vec.size() * sizeof(GL_FLOAT), &texcoords_vec[0], GL_STATIC_DRAW); GL_CHECK_ERRORS;
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*)0); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(2); GL_CHECK_ERRORS;

    //передаем в шейдерную программу индексы
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices); GL_CHECK_ERRORS;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_vec.size() * sizeof(GLuint), &indices_vec[0], GL_STATIC_DRAW); GL_CHECK_ERRORS;

    glEnable(GL_PRIMITIVE_RESTART); GL_CHECK_ERRORS;
    glPrimitiveRestartIndex(primRestart); GL_CHECK_ERRORS;
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);


  return numIndices;
}


int initGL()
{
	int res = 0;

	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	//выводим в консоль некоторую информацию о драйвере и контексте opengl
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "Controls: "<< std::endl;
    std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
    std::cout << "press spacebar to alternate between shaded wireframe and fill display modes" << std::endl;
    std::cout << "press ESC to exit" << std::endl << std::endl;
    
	return 0;
}

enum Mode {
    DEBUG_TRIANGLE,
    SCENE
};

///////////////////////////////////////////////////////////////////

// renderQuad() renders a 1x1 XY quad in NDC
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
          // positions   // texCoords
          -1.0f,  1.0f,  0.0f, 1.0f,
          -1.0f, -1.0f,  0.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 0.0f,

          -1.0f,  1.0f,  0.0f, 1.0f,
           1.0f, -1.0f,  1.0f, 0.0f,
           1.0f,  1.0f,  1.0f, 1.0f
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    glBindVertexArray(0);
}

///////////////////////////////////////////////////////////////////


///////////////////for cubemaps begin/////////////////////
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
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

    return textureID;
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
///////////////////for cubemaps end/////////////////////

float Noise(float x, float y) {
    int n = int(x + y * 57);
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);  
}  

float SmoothedNoise(float x, float y) {
    float corners = (Noise(x-1, y-1) + Noise(x+1, y-1) + Noise(x-1, y+1) + Noise(x+1, y+1)) / 16;
    float sides   = (Noise(x-1, y) + Noise(x+1, y) + Noise(x, y-1) + Noise(x, y+1)) /  8;
    float center  =  Noise(x, y) / 4;
    return corners + sides + center;
}

float Interpolate(float a, float b, float x) {
    float ft = x * 3.1415927;
    float f = (1 - cos(ft)) * 0.5;
    return  a*(1-f) + b*f;
}


float InterpolatedNoise(float x, float y) {

      int integer_X    = int(x);
      float fractional_X = x - integer_X;

      int integer_Y    = int(y);
      float fractional_Y = y - integer_Y;

      float v1 = SmoothedNoise(integer_X,     integer_Y);
      float v2 = SmoothedNoise(integer_X + 1, integer_Y);
      float v3 = SmoothedNoise(integer_X,     integer_Y + 1);
      float v4 = SmoothedNoise(integer_X + 1, integer_Y + 1);

      float i1 = Interpolate(v1 , v2 , fractional_X);
      float i2 = Interpolate(v3 , v4 , fractional_X);

      return Interpolate(i1 , i2 , fractional_Y);
}

float PerlinNoise_2D(float x, float y, float persistence, float Number_Of_Octaves) {

      float total = 0;

      float p = persistence;
      float n = Number_Of_Octaves - 1;

      for (float i = 0.0; i < n; i+=1.0) {

        float frequency = pow(2,i);
        float amplitude= pow(p,i);

        total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;

      }

      return total;

}

int main(int argc, char** argv) {
    Mode mode = DEBUG_TRIANGLE;
    GLFWwindow*  window = nullptr;

    if (argc >= 2) {
        mode = string(argv[1]) == "-planescene" ? SCENE : DEBUG_TRIANGLE;
    }

    try {
    
    if(!glfwInit())
    return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL basic sample", nullptr, nullptr);
	
    if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	//регистрируем коллбеки для обработки сообщений от пользователя - клавиатура, мышь..
	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);
	glfwSetInputMode          (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	if(initGL() != 0) 
		return -1;
	
    //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

    //создание шейдерной программы из двух файлов с исходниками шейдеров
    //используется класс-обертка ShaderProgram
    std::unordered_map<GLenum, std::string> shaders;
    shaders[GL_VERTEX_SHADER] = "shaders/vertex.glsl";
    shaders[GL_FRAGMENT_SHADER] = "shaders/fragment.glsl";
    ShaderProgram program(shaders); GL_CHECK_ERRORS;

    std::unordered_map<GLenum, std::string> terrain_shaders;
    terrain_shaders[GL_VERTEX_SHADER] = "shaders/terrain_vertex.glsl";
    terrain_shaders[GL_FRAGMENT_SHADER] = "shaders/terrain_fragment.glsl";
    ShaderProgram terrain_program(terrain_shaders); GL_CHECK_ERRORS;

    std::unordered_map<GLenum, std::string> debug_shaders;
    debug_shaders[GL_VERTEX_SHADER] = "shaders/vertex_start.glsl";
    debug_shaders[GL_FRAGMENT_SHADER] = "shaders/fragment_start.glsl";
    ShaderProgram debug_program(debug_shaders); GL_CHECK_ERRORS;

    std::unordered_map<GLenum, std::string> rain_shaders;
    rain_shaders[GL_VERTEX_SHADER] = "shaders/rain_vertex.glsl";
    rain_shaders[GL_FRAGMENT_SHADER] = "shaders/rain_fragment.glsl";
    ShaderProgram rain_program(rain_shaders);

    std::unordered_map<GLenum, std::string> cloud_Shader;
    cloud_Shader[GL_VERTEX_SHADER] = "shaders/cloud_vertex.glsl";
    cloud_Shader[GL_FRAGMENT_SHADER] = "shaders/cloud_fragment.glsl";
    ShaderProgram cloud_program(cloud_Shader); //GL_CHECK_ERRORS;

    ///////////////////////////////////////////////////////////////////
    
    std::unordered_map<GLenum, std::string> blur_shaders;
    blur_shaders[GL_VERTEX_SHADER] = "shaders/blur_vertex.glsl";
    blur_shaders[GL_FRAGMENT_SHADER] = "shaders/blur_fragment.glsl";
    ShaderProgram blur_program(blur_shaders); //GL_CHECK_ERRORS;
                                //blur_shaders
    std::unordered_map<GLenum, std::string> bloom_shaders;
    bloom_shaders[GL_VERTEX_SHADER] = "shaders/bloom_vertex.glsl";
    bloom_shaders[GL_FRAGMENT_SHADER] = "shaders/bloom_fragment.glsl";
    ShaderProgram bloom_program(bloom_shaders); //GL_CHECK_ERRORS;
                                  //bloom_shaders
    // configure (floating point) framebuffers
    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO); //GL_CHECK_ERRORS;
    // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
    
    unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    
    /*
    /////
    unsigned int texture_hdr;
    glGenTextures(1, &texture_hdr);
    glBindTexture(GL_TEXTURE_2D, texture_hdr); //GL_CHECK_ERRORS;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_hdr, 0);
    
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo); //GL_CHECK_ERRORS;
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); //GL_CHECK_ERRORS;
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); //GL_CHECK_ERRORS;
    */
    //////
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //GL_CHECK_ERRORS;

    
    // ping-pong-framebuffer for blurring
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    ////////////////////////////////////////////////////////////////////
    
    //Создаем и загружаем геометрию поверхности
    GLuint vaoTriStrip;
    int triStripIndices = createTriStrip(300, 300, 2000, vaoTriStrip);

    vector<unique_ptr<Mesh>> scene;
    map<uint32_t, Material> materials;

    ImportSceneObjectFromFile("assets/plane.xml", scene, materials);

    glViewport(0, 0, WIDTH, HEIGHT);  GL_CHECK_ERRORS;
    glEnable(GL_DEPTH_TEST);  GL_CHECK_ERRORS;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  ////////for cube maps//////////////////
  std::unordered_map<GLenum, std::string> skybox_Shader;
  skybox_Shader[GL_VERTEX_SHADER] = "shaders/skybox_vertex.glsl";
  skybox_Shader[GL_FRAGMENT_SHADER] = "shaders/skybox_fragment.glsl";
  ShaderProgram skyboxShader(skybox_Shader); GL_CHECK_ERRORS;

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

    // skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0); GL_CHECK_ERRORS;
    glEnableVertexAttribArray(0); GL_CHECK_ERRORS;
    

    // load textures
    vector<std::string> faces
    {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg",
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader.SetUniform("skybox",0);
    /////////for cube maps end///////////////

/*
    ////////for clouds//////////////////
  std::unordered_map<GLenum, std::string> cube_Shader;
  cube_Shader[GL_VERTEX_SHADER] = "shaders/cube_vertex.glsl";
  cube_Shader[GL_FRAGMENT_SHADER] = "shaders/cube_fragment.glsl";
  ShaderProgram cubeShader(cube_Shader); //GL_CHECK_ERRORS;

    float cubeVertices[] = {
        // positions          // texture Coords
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

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    //unsigned int cubeTexture = loadTexture("textures/cloudtex/cl_tex.jpg");

    //cubeShader.SetUniform("texture1",1);


*/  
/*

  std::unordered_map<GLenum, std::string> cloud_Shader;
  cloud_Shader[GL_VERTEX_SHADER] = "shaders/cloud_vertex.glsl";
  cloud_Shader[GL_FRAGMENT_SHADER] = "shaders/cloud_fragment.glsl";
  ShaderProgram cloudShader(cloud_Shader); //GL_CHECK_ERRORS;

    float cloudVertices[] = {
      50.5f,  0.0f, 50.5f,  // Верхний правый угол
      50.5f, -0.0f, -50.5f,  // Нижний правый угол
      -50.5f, -0.0f, -50.5f,  // Нижний левый угол
      -50.5f,  0.0f, 50.5f
    };

    GLuint indices[] = {  // Помните, что мы начинаем с 0!
      0, 1, 3,   // Первый треугольник
      1, 2, 3    // Второй треугольник
    };

    // skybox VAO
    GLuint cloudVBO, cloudVAO, cloudEBO;
    glGenVertexArrays(1, &cloudVAO);
    glGenBuffers(1, &cloudVBO);
    glGenBuffers(1, &cloudEBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(cloudVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cloudVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cloudVertices), cloudVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cloudEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0);
*/

/*
    unsigned int cubes_num = 12;

    float4x4 offsets[cubes_num];
    int j = 0;
    float offset = 7.0f;
    for(int i = -5; i <= 7; i += 1) {
        float3 translation;
        translation.x = abs((float)i *  offset / 10);
        //translation.z = (float)i * offset / 10;
        //translation.y = abs((float)i * offset);
        offsets[j++] = transpose(translate4x4(translation));
    }
*/
    /////////for cclouds end///////////////

    auto raindrop = CreateRaindropMesh();

    auto clouds = CreateCloudMesh();

    int frame_counter = 0;

    int rand_param_1 = rand()%15;
    int rand_param_2 = rand()%20;
    int rand_param_3 = rand()%7;

    glEnable(GL_DEPTH_TEST);

	//цикл обработки сообщений и отрисовки сцены каждый кадр
	while (!glfwWindowShouldClose(window))
	{
		//считаем сколько времени прошло за кадр
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
        doCameraMovement(camera, deltaTime);

        switch (mode) {
        case SCENE: {
            glClearColor(0.1f, 0.6f, 0.8f, 1.0f); 
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

            ///////////////////////////////////////////////////////////////////
            
            glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO); //GL_CHECK_ERRORS;
            glClearColor(0.1f, 0.6f, 0.8f, 1.0f); //GL_CHECK_ERRORS;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //GL_CHECK_ERRORS;
            //glEnable(GL_DEPTH_TEST);  //GL_CHECK_ERRORS;
            ///////////////////////////////////////////////////////////////////

            float4x4 view = camera.GetViewMatrix();
            float4x4 projection = projectionMatrixTransposed(camera.zoom, float(WIDTH) / float(HEIGHT), 0.1f, 1000.0f);

            skyboxShader.StartUseShader();
            glDepthMask(GL_FALSE);
            skyboxShader.SetUniform("view", view);
            skyboxShader.SetUniform("projection", projection);
            // skybox cube
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthMask(GL_TRUE);
            skyboxShader.StopUseShader();


            for (auto &mesh : scene) {
                program.StartUseShader(); 
                //
                
                // Матераил без текстуры содержит -1 в поле tex_id
                if (materials[mesh->material_id].tex_id != -1) {
	                glActiveTexture(GL_TEXTURE2); 
	                glBindTexture(GL_TEXTURE_2D, materials[mesh->material_id].tex_id);
	                //glBindVertexArray(skyboxVAO);
    			        glActiveTexture(GL_TEXTURE3);
    			        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			        
                }
                DrawMesh(program, camera, mesh, WIDTH, HEIGHT, deltaTime);

                program.StopUseShader(); 
            }

            

            float4x4 model = transpose(translate4x4(float3(0, -60, 0)));
        
            terrain_program.StartUseShader();
        
            static float2 plane_pos = float2(0, 0);
            static float2 speed = float2(10, 0);

            plane_pos.x += deltaTime * speed.x;
            plane_pos.y += deltaTime * speed.y;

            terrain_program.SetUniform("view", view);       
            terrain_program.SetUniform("projection", projection); 
            terrain_program.SetUniform("model", model); 
        
            terrain_program.SetUniform("plane_pos", plane_pos); 

            glBindVertexArray(vaoTriStrip);
            glDrawElements(GL_TRIANGLE_STRIP, triStripIndices, GL_UNSIGNED_INT, 0);

            terrain_program.StopUseShader();

            float voxel_size = 0.1;
            
            frame_counter ++;
            if (frame_counter % 50 == 0) {
                rand_param_1 = rand()%15;
                rand_param_2 = rand()%20;
                rand_param_3 = rand()%5;
            }
              
            /*
            //srand(time(NULL));
            for (int i = 0; i < 20; i++) {
              for (int j = 0; j < 20; j++) {
                for (int z = 0; z < 4; z++) {
                    float4x4 model_cube = transpose(translate4x4(float3(0+i*voxel_size, 10+z*voxel_size, 0+j*voxel_size)));

                    float4x4 matrix = transpose(scale4x4(float3(voxel_size,voxel_size,voxel_size)));

                    model_cube = mul(matrix,model_cube);

                    float col = (PerlinNoise_2D(i,j,0.3,15.) + PerlinNoise_2D(i,z,1.0,15.) + PerlinNoise_2D(j,z,1.0,15.))/3 + 0.3;
                    //auto NoiseObj = siv::PerlinNoise();

                    //float col = NoiseObj.noise((double)i,(double)j,(double)z);
                    //printf("%f\n",col);

                    if (col <= 0.1)
                      continue;

                    cubeShader.StartUseShader();
                    glEnable(GL_BLEND);
                    glBlendEquation(GL_FUNC_ADD);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                    cubeShader.SetUniform("model", model_cube);
                    cubeShader.SetUniform("view", view);
                    cubeShader.SetUniform("projection", projection);
                    cubeShader.SetUniform("col",col);
                    // cubes
                    glBindVertexArray(cubeVAO);
                    //glActiveTexture(GL_TEXTURE0);
                    //glBindTexture(GL_TEXTURE_2D, cubeTexture);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(0);

                    cubeShader.StopUseShader();
                    glDisable(GL_BLEND);
                  }
                }
            }
            */

            /* 2d cloud_quad
            float4x4 model_cloud = transpose(translate4x4(float3(0, 250, 0)));

            cloudShader.StartUseShader();
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            
            for(unsigned int i = 0; i < cubes_num; i++) {
		        std::stringstream ss;
		        string index;
		        ss << i; 
		        index = ss.str();
		        cloudShader.SetUniform(("offsets[" + index + "]").c_str(), offsets[i]);
			       
            cloudShader.SetUniform("view", view);
            cloudShader.SetUniform("projection", projection);
            cloudShader.SetUniform("model", model_cloud);

            glBindVertexArray(cloudVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            
            
            glBindVertexArray(cloudVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0); 

            cloudShader.StopUseShader();
            glDisable(GL_BLEND);
            */

            DrawClouds(cloud_program, camera, clouds, WIDTH, HEIGHT, deltaTime);

            DrawRain(rain_program, camera, raindrop, WIDTH, HEIGHT, deltaTime);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            // blur bright fragments with two-pass Gaussian Blur
            bool horizontal = true, first_iteration = true;
            unsigned int amount = 10;


            blur_program.StartUseShader();

            blur_program.SetUniform("image", 0); //GL_CHECK_ERRORS;

            for (unsigned int i = 0; i < amount; i++) {
                glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
                blur_program.SetUniform("horizontal", horizontal);
                // bind texture of other framebuffer (or scene if first iteration)
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
                renderQuad();
                horizontal = !horizontal;
                if (first_iteration)
                    first_iteration = false;
            }

            blur_program.StopUseShader();
            
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            // now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            bool bloom = true;
            float exposure = 1.0f;

            bloom_program.StartUseShader();

            glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, texture_hdr);
            glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);

            bloom_program.SetUniform("bloom", bloom); ////GL_CHECK_ERRORS;
            bloom_program.SetUniform("exposure", exposure); ////GL_CHECK_ERRORS;
            bloom_program.SetUniform("scene", 0); ////GL_CHECK_ERRORS;
            bloom_program.SetUniform("bloomBlur", 1); ////GL_CHECK_ERRORS;
            

            renderQuad();

            bloom_program.StopUseShader();

        } break;
        case DEBUG_TRIANGLE: {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
            DrawSimpleTriangle(debug_program, camera, WIDTH, HEIGHT);
        } break;
        };

        glfwSwapBuffers(window); 
	}

	//очищаем vao перед закрытием программы
	glDeleteVertexArrays(1, &vaoTriStrip);

	glfwTerminate();
    }
    catch (std::exception &e) {
        if (window) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        glfwTerminate();
        std::cout << std::endl << "Exception happened! See details." << std::endl;
        std::cout << e.what() << std::endl;

        std::cout << std::endl << "Press any button to continue..." << std::endl;
        std::cin.get();
    }

    return 0;
}

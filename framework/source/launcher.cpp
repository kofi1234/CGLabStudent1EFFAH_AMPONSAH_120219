#include "launcher.hpp"

#include <glbinding/gl/gl.h>
// load glbinding extensions
#include <glbinding/Binding.h>
// load meta info extension
#include <glbinding/Meta.h>

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "application.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>

// use gl definitions from glbinding 
using namespace gl;

// helper functions

std::string read_resource_path(int argc, char* argv[]) {
  std::string resource_path{};
  //first argument is resource path
  if (argc > 1) {
    resource_path = argv[1];
  }
  // no resource path specified, use default
  else {
    std::string exe_path{argv[0]};
    resource_path = exe_path.substr(0, exe_path.find_last_of("/\\"));
    resource_path += "/../../resources/";
  }

  return resource_path;
}

GLFWwindow* initialize(unsigned width, unsigned height) {

  glfwSetErrorCallback(glsl_error);

  if (!glfwInit()) {
    std::exit(EXIT_FAILURE);
  }

  // set OGL version explicitly 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  //MacOS requires core profile
  #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #else
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  #endif
  // create m_window, if unsuccessfull, quit
  GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Framework", NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  // use the windows context
  glfwMakeContextCurrent(window);
  // disable vsync
  glfwSwapInterval(0);
  // initialize glindings in this context
  glbinding::Binding::initialize();

  // activate error checking after each gl function call
  watch_gl_errors();

  return window;
}
 
void set_callback_object(Application* app, GLFWwindow* window) {
  // set user pointer to access this instance statically
  glfwSetWindowUserPointer(window, app);
  // register key input function
  auto key_func = [](GLFWwindow* w, int a, int b, int c, int d) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->key_callback(w, a, b, c, d);
  };
  glfwSetKeyCallback(window, key_func);
  // allow free mouse movement
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  auto mouse_func = [](GLFWwindow* w, double a, double b) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->mouse_callback(w, a, b);
  };
  glfwSetCursorPosCallback(window, mouse_func);
  // allow free mouse movement
  // register resizing function
  auto resize_func = [](GLFWwindow* w, int a, int b) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->resize_callback(w, a, b);
  };
  glfwSetFramebufferSizeCallback(window, resize_func);  
}

///////////////////////////// update functions ////////////////////////////////
// update viewport and field of view
glm::fmat4 calculate_projection_matrix(unsigned width, unsigned height) {
  float aspect = float(width) / float(height);
  // base fov does not change
  static const float fov_y_base = glm::radians(60.0f);
  float fov_y = fov_y_base;
  // if width is smaller, extend vertical fov 
  if (width < height) {
    fov_y = 2.0f * glm::atan(glm::tan(fov_y * 0.5f) * (1.0f / aspect));
  }
  // projection is hor+ 
  return glm::perspective(fov_y, aspect, 0.1f, 100.0f);
}

// load shader programs and update uniform locations
void update_shader_programs(std::map<std::string, shader_program>& shaders, bool throwing) {
  // actual functionality in lambda to allow update with and without throwing
  auto update_lambda = [&](){
    // reload all shader programs
    for (auto& pair : shaders) {
      // throws exception when compiling was unsuccessfull
      GLuint new_program = shader_loader::program(pair.second.vertex_path,
                                                  pair.second.fragment_path);
      // free old shader program
      glDeleteProgram(pair.second.handle);
      // save new shader program
      pair.second.handle = new_program;
    }
  };

  if (throwing) {
    update_lambda();
  }
  else {
    try {
     update_lambda();
    }
    catch(std::exception&) {
      // dont crash, allow another try
    }
  }
}

///////////////////////////// misc functions ////////////////////////////////

// calculate fps and show in m_window title
void show_fps(GLFWwindow* window) {
    // variables for fps computation
  static double m_last_second_time;
  static unsigned m_frames_per_second;

  ++m_frames_per_second;
  double current_time = glfwGetTime();
  if (current_time - m_last_second_time >= 1.0) {
    std::string title{"OpenGL Framework - "};
    title += std::to_string(m_frames_per_second) + " fps";

    glfwSetWindowTitle(window, title.c_str());
    m_frames_per_second = 0;
    m_last_second_time = current_time;
  }
}

void close_and_quit(GLFWwindow* window, int status) {
  // free opengl resources
  // delete m_application;
  // free glfw resources
  glfwDestroyWindow(window);
  glfwTerminate();

  std::exit(status);
}


void glsl_error(int error, const char* description) {
  std::cerr << "GLSL Error " << error << " : "<< description << std::endl;
}

void watch_gl_errors(bool activate) {
  if(activate) {
    // add callback after each function call
    glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue, {"glGetError", "glBegin", "glVertex3f", "glColor3f"});
    glbinding::setAfterCallback(
      [](glbinding::FunctionCall const& call) {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
          // print name
          std::cerr <<  "OpenGL Error: " << call.function->name() << "(";
          // parameters
          for (unsigned i = 0; i < call.parameters.size(); ++i)
          {
            std::cerr << call.parameters[i]->asString();
            if (i < call.parameters.size() - 1)
              std::cerr << ", ";
          }
          std::cerr << ")";
          // return value
          if(call.returnValue) {
            std::cerr << " -> " << call.returnValue->asString();
          }
          // error
          std::cerr  << " - " << glbinding::Meta::getString(error) << std::endl;
          // throw exception to allow for backtrace
          throw std::runtime_error("Execution of " + std::string(call.function->name()));
          exit(EXIT_FAILURE);
        }
      }
    );
  }
  else {
    glbinding::setCallbackMask(glbinding::CallbackMask::None);
  }
}

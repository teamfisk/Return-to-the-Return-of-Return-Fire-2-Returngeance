#version 400

in vec3 p_eye;
in vec3 n_eye;

layout (location = 0) out vec4 def_p; // "go to GL_COLOR_ATTACHMENT0"
layout (location = 1) out vec4 def_n; // "go to GL_COLOR_ATTACHMENT1"

void main () {
  def_p = vec4(p_eye, 1.0);
  def_n = vec4(n_eye, 1.0);
}
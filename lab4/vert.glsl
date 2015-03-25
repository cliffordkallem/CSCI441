#version 330
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
in vec3 position;
in vec3 color;
out vec3 fcolor;

void main() {
  //gl_Position = model*projection *view*vec4(position.x,position.y,position.z, 1);
    mat4 mvp= projection*view*model;
    //gl_Position= mvp*vec4(position.x,position.y,position.z, 1);
  gl_Position =mvp *vec4(position.x,position.y,position.z, 1);
    //gl_Position = projection *vec4(position.x,position.y,position.z, 1);

        fcolor = color;

}

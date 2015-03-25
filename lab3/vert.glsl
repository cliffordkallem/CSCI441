#version 330
uniform mat4 projection;
in vec2 position;

void main() {

vec4 thing    = projection * vec4(position.x, position.y, 0, 1);
  gl_Position = vec4(-1+thing.x,1-thing.y,0,1);
}

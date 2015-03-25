#version 330
uniform mat4 projection;
in vec3 position;
in vec3 color;
out vec4 out_color;
void main() {
    // vec3 thing = vec3(position.x/window.x*current.x,position.y/window.y*current.y, position.z);
//    gl_Position=projection * vec4(position.x, position.y, 0, 1);
gl_Position    = projection * vec4(position.x, position.y, 0, 1);
 // gl_Position = vec4(thing.x*window.x/current.x,thing.y*window.y/current.y,0,1);
//gl_Position = vec4(position.x, position.y, 0, 1);
  out_color = vec4(color.x,color.y,color.z,1.0);
}

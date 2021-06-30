#ifndef SHADER_H
#define SHADER_H

static unsigned int shader_program = 0;
static unsigned char is_glew_init = 0;

static char *text_vertsrc =
"#version 120\n"
"varying vec2 TexCoords;\n"
"void main() {\n"
"gl_Position = gl_ProjectionMatrix * vec4(gl_Vertex.xy, 0.0, 1.0);\n"
"TexCoords = gl_MultiTexCoord0.st;}\0";

static char *text_fragsrc =
"#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"uniform sampler2D text;\n"
"uniform vec4 textColor;\n"
"void main() {\n"
"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"color = textColor * sampled;}\0";

static int TEXTSHADER = -1;

void forceGlew();

int newShader(char *vertfile, char *fragfile);
void useTextShader(unsigned int texture, int r, int g, int b, int a);
void resetShader();
void useProgram(int program);

#endif
#include "shader.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

void forceGlew() {

	if (!is_glew_init) {
		glewInit();
		is_glew_init = 1;
	}

}

char *readFile(char *filename, int *len) {

    FILE *f = fopen(filename, "r");

    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    int n = ftell(f) -1, i = 0;
    fseek(f, 0, SEEK_SET);
    char c, *text = (char*)malloc(sizeof(char)*(++n));

	fread(text, n, sizeof(char), f);
	text[n] = '\0';
    if (len) *len = n;

    fclose(f);

    return text;
}

int newShaderFromStr(char *vertsrc, char *fragsrc) {

	int vert, frag, success;
	static char infolog[512];

	forceGlew();

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, (const char**)&vertsrc, NULL);
	glCompileShader(vert);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert, 512, NULL, infolog);
		printf("ERROR::SHADER::VERTEX::COMPILE\n%s", infolog);

		glDeleteShader(vert);
		return -1;
	}


	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, (const char**)&fragsrc, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag, 512, NULL, infolog);
		printf("ERROR::SHADER::FRAGMENT::COMPILE\n%s", infolog);

		glDeleteShader(vert);
		glDeleteShader(frag);
		return -1;
	}


	int program;
	program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	glDeleteShader(vert);
	glDeleteShader(frag);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infolog);
		printf("ERROR::SHADER::PROGRAM::LINK\n%s", infolog);

		glDeleteProgram(program);
		return -1;
	}

	return program;

}

int newShader(char *vertfile, char *fragfile) {

	char *vertsrc, *fragsrc;
	vertsrc = readFile(vertfile, NULL);
	fragsrc = readFile(fragfile, NULL);

	if (vertsrc == NULL || fragsrc == NULL) {
		printf("ERROR::SHADER::OPEN_FILE\n");
		return -10; // It's a way to differ from regular -1
	}

	int sh = newShaderFromStr(vertsrc, fragsrc);

	free(vertsrc);
	free(fragsrc);

	return sh;

}

void useTextShader(unsigned int texture, int r, int g, int b, int a) {

	if (TEXTSHADER == -1)
		TEXTSHADER = newShaderFromStr(text_vertsrc, text_fragsrc);

	unsigned int sh = TEXTSHADER;

	float t = 255.0;
	glUseProgram(sh);
	glUniform4f(
		glGetUniformLocation(sh, "textColor"),
		r/t, g/t, b/t, a/t
	);

	unsigned int tex;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(sh, "text"), 0);

}

void resetShader() {

	forceGlew();
	glUseProgram(0);

}

void useProgram(int program) {

	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);

}
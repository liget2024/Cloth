#include "ku.h"
#include "Cloth.h"

GLuint textureObj = 1;
GLFWwindow* globalWindow = nullptr; // 使用全局变量

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow* win, int width, int height) {
    glViewport(0, 0, width, height);
}

void display() {
    // 渲染代码
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float center = (MESH_SIZE - 1) * STRUCTURAL_SPRING_REST_LENGTH / 2.0f;
    gluLookAt(center, center, center * 3, center, center, 0.0f, 0.0f, 1.0f, 0.0f); // Look at the center of the cloth
    static Cloth cloth;
    cloth.Simulate();
    cloth.Draw();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 1.333f, 1.0f, center * 20);

    glfwSwapBuffers(globalWindow);
    glfwPollEvents();
}

void SetTextures(const char* fileName) {
    glGenTextures(1, &textureObj);
    glBindTexture(GL_TEXTURE_2D, textureObj);
    textureObj = SOIL_load_OGL_texture(
        fileName,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if (textureObj == 0) {
        // 处理纹理加载错误
        fprintf(stderr, "Failed to load texture: %s\n", SOIL_last_result());
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void SetLightSource(void) {
    GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position[] = { 0.0f, 0.0f, 1.0f, 0.0f };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void SetMaterial(void) {
    GLfloat ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
}

int main(int argc, char* argv[]) {
    if (!glfwInit()) {
        return -1;
    }

    globalWindow = glfwCreateWindow(1024, 768, "Cloth Simulation", NULL, NULL);
    if (!globalWindow) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(globalWindow);
    glfwSetKeyCallback(globalWindow, key_callback);
    glfwSetFramebufferSizeCallback(globalWindow, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    SetTextures("E:/code/OpenGL100/Fabric_Texture.jpg");
    SetLightSource();
    SetMaterial();

    // 渲染循环
    while (!glfwWindowShouldClose(globalWindow)) {
        display();
    }

    if (globalWindow) {
        glfwDestroyWindow(globalWindow);
    }
    glfwTerminate();
    return 0;
}
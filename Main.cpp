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
        fprintf(stderr, "Failed to load texture: %s\n", SOIL_last_result());
        return; // 纹理加载失败，退出函数
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void SetLightSource(void) {
    GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
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

void SetTextures(const char* fileName) {
    // 加载图像
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
    FIBITMAP* bitmap = FreeImage_Load(format, fileName);

    if (!bitmap) {
        std::cerr << "Failed to load texture" << std::endl;
        return;
    }

    // 转换图像为32位RGBA格式
    FIBITMAP* converted_bitmap = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap); // 卸载原始图像

    // 获取图像数据
    int width = FreeImage_GetWidth(converted_bitmap);
    int height = FreeImage_GetHeight(converted_bitmap);
    unsigned char* pixels = FreeImage_GetBits(converted_bitmap);

    // 生成纹理ID
    glGenTextures(1, &textureObj);
    glBindTexture(GL_TEXTURE_2D, textureObj);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 上传纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

    // 生成Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // 卸载转换后的图像
    FreeImage_Unload(converted_bitmap);
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

    SetTextures("E:\code\OpenGL100\Fabric_Texture");
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
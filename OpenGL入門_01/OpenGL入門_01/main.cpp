//
//  main.cpp
//  OpenGL入門_01
//
//  Created by hw18a014 on 2021/05/25.
//  Copyright © 2021 gakusei. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// シェーダ・オブジェクトのコンパイル結果を表示する
//  shader: シェーダ・オブジェクト名
//  str: コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char *str) {
    // コンパイル結果を取得する
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) cerr << "Compile Error in " << str << endl;
    
    // シェーダのコンパイル時のログの長さを取得する
    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    
    if (bufSize > 1) {
        // シェーダのコンパイル時のログの内容を取得する
        vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        cerr << &infoLog[0] << endl;
    }
    
    return static_cast<GLboolean>(status);
}

// プログラム・オブジェクトのリンク結果を表示する
//  program: プログラム・オブジェクト名
GLboolean printProgramInfoLog(GLuint program) {
    // コンパイル結果を取得する
    GLint status;
    glGetShaderiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) cerr << "Link Error." << endl;
    
    // シェーダのコンパイル時のログの長さを取得する
    GLsizei bufSize;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    
    if (bufSize > 1) {
        // シェーダのコンパイル時のログの内容を取得する
        vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(program, bufSize, &length, &infoLog[0]);
        cerr << &infoLog[0] << endl;
    }
    
    return static_cast<GLboolean>(status);
}



// プログラム・オブジェクトを作る
//  vsrc: バーテックス・シェーダのソース・プログラムの文字列
//  fsrc: フラグメント・シェーダのソース・プログラムの文字列
GLuint createProgram(const char *vsrc, const char *fsrc) {
    // 空のプログラム・オブジェクトを作る
    const GLuint program(glCreateProgram());
    
    if (vsrc != NULL) {
        // バーテックス・シェーダのシェーダ・オブジェクトを作る
        const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
        glShaderSource(vobj, 1, &vsrc, NULL);
        glCompileShader(vobj);
        
        // バーテックス・シェーダのシェーダ・オブジェクトを作る
        glAttachShader(program, vobj);
        glDeleteShader(vobj);
    }
    
    if (fsrc != NULL) {
        // フラグメント・シェーダのシェーダ・オブジェクトを作る
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        glShaderSource(fobj, 1, &fsrc, NULL);
        glCompileShader(fobj);
        
        // フラグメント・シェーダのシェーダ・オブジェクトを作る
        glAttachShader(program, fobj);
        glDeleteShader(fobj);
    }

    // プログラム・オブジェクトをリンクする
    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);
    
    // 作ったプログラム・オブジェクトを返す
    return program;
}

int main(int argc, char * argv[]) {
    // GLFWを初期化する
    if (glfwInit() == GL_FALSE) {
        cerr << "Can't initialize GLFW." << endl;
        return 1;
    }
    // GLFWのウィンドウを作成する
    GLFWwindow *const window(glfwCreateWindow(640, 480, "Hello!", NULL, NULL));
    // ウィンドウを作れなかったとき
    if (window == NULL) {
        cerr << "Can't create GLFW window." << endl;
        // GLFWの終了処理を行う
        glfwTerminate();
        return 1;
    }
    
    // プログラム終了時の処理を登録する
    atexit(glfwTerminate);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // 作ったウィンドウをOpenGLの処理対象にする
    glfwMakeContextCurrent(window);
    
    // GLFWを初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Can't initialize GLEW." << endl;
        return 1;
    }
    
    // 垂直同期のタイミングを待つ
    glfwSwapInterval(1);
    
    // 背景色を指定する
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    // バーテックス・シェーダーソース・プログラム
    static const GLchar vsrc[] =
    "#version 150 core\n"
    "in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_position = position;\n"
    "}\n";
    
    static const GLchar fsrc[] =
    "#version 150 core\n"
    "out vec4 fragment;\n"
    "void main()\n"
    "{\n"
    "   fragment = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";
    
    // プログラム・オブジェクトを作る
    const GLuint program(createProgram(vsrc, fsrc));

    
    // ウィンドウが開いている間繰り返す
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // ウィンドウを消去する
        glClear(GL_COLOR_BUFFER_BIT);
        
        // シェーダー・プログラムの使用開始
        glUseProgram(program);
        
        //
        //ここで描画処理を行う
        //
        // カラー・バッファを入れ替える
        glfwSwapBuffers(window);
        
        // イベントを取り出す
        glfwWaitEvents();
    }
    
    glfwTerminate();
}


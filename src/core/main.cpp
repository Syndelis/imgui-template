/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <iostream>
#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>

/* -----------------------------------------------------------------------------
    FUNCTION DECLARATIONS
----------------------------------------------------------------------------- */

// Nothing Yet

/* -----------------------------------------------------------------------------
    MAIN
----------------------------------------------------------------------------- */

int main(int argc, char** argv) {

    // GLFW Setup ------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4); // AA
    GLFWwindow* window = glfwCreateWindow(
        1280, 720, "ImGui GLFW OpenGL3 Example", NULL, NULL
    );
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // GL Setup --------------------------------------------
    glClearColor(.0, .0, .0, 1.);
    glEnable(GL_MULTISAMPLE); // AA

    // ImGui Setup -----------------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto io = ImGui::GetIO();

    io.ConfigFlags |=   ImGuiConfigFlags_NavEnableKeyboard |\
                        ImGuiConfigFlags_NavEnableSetMousePos;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Main Loop -------------------------------------------
    while (!glfwWindowShouldClose(window)) {

        // Draw Start -------------------------------------
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Application Code -------------------------------

        ImGui::Begin("Hello, World!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        // Draw End ---------------------------------------

        glFlush();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // Cleanup Code ----------------------------------------

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}
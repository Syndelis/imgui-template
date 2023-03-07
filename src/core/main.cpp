/* -----------------------------------------------------------------------------
    INCLUDES & GLOBALS
----------------------------------------------------------------------------- */

#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>

/* -----------------------------------------------------------------------------
    STRUCT & FUNC DECLARATIONS
----------------------------------------------------------------------------- */

enum class DiaSemana : unsigned int {
    Nenhum      = 0,
    Segunda     = 1 << 0,
    Terca       = 1 << 1,
    Quarta      = 1 << 2,
    Quinta      = 1 << 3,
    Sexta       = 1 << 4,
};

static const std::tuple<DiaSemana, std::string> dias_e_nomes[] = {
    {DiaSemana::Segunda, "Segunda"},
    {DiaSemana::Terca, "Terça"},
    {DiaSemana::Quarta, "Quarta"},
    {DiaSemana::Quinta, "Quinta"},
    {DiaSemana::Sexta, "Sexta"},
};

std::string buildNomeFrom(unsigned int dias) {
    std::string full_name = "";

    for (auto &[dia_val, dia_name] : dias_e_nomes)
        if ((dias & (unsigned int) dia_val) != (unsigned int) DiaSemana::Nenhum)
            full_name += dia_name + " & ";

    full_name.pop_back(); // Remove last space
    full_name.pop_back(); // Remove last ampersand

    return full_name;
}

bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

// Returns 0 if no errors
static int hourMask(ImGuiInputTextCallbackData *data) {
    // Mask: \d{2}:\d{2}

    const char *input = (const char *) data->UserData;
    const int input_len = strlen(input);

    if (input_len == 2)
        return data->EventChar != ':';
    
    else if (input_len < 5)
        return !isNumber(data->EventChar);

    else
        return 1;

}

struct Disciplina {
    std::string nome;
    int carga_horaria;
    unsigned int dias_da_semana;
    std::string hora_inicio;
    std::string hora_fim;
};

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

    // Application Setup -----------------------------------

    static std::vector<Disciplina> disciplinas;

    // std::tuple<char, std::string> dia = {DiaSemana::Segunda, "Segunda"};

    // Main Loop -------------------------------------------
    while (!glfwWindowShouldClose(window)) {

        // Draw Start -------------------------------------
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Application Code -------------------------------

        ImGui::Begin("Tabbed Window", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        
        if (ImGui::BeginTabBar("##TabBar")) {
            
            if (ImGui::BeginTabItem("Cadastrar")) {
                static char name_buf[256] = {0}, start_buf[10] = {0}, end_buf[10] = {0};
                static int carga_horaria = 0;
                static unsigned int dias_da_semana = (unsigned int) DiaSemana::Nenhum;

                ImGui::InputText("Nome", name_buf, 256);
                ImGui::InputInt("Carga Horária", &carga_horaria);

                ImGui::PushItemWidth(100);
                ImGui::InputText("Hora Início", start_buf, 256, ImGuiInputTextFlags_CallbackCharFilter, hourMask, (void *)start_buf);
                ImGui::SameLine();
                ImGui::InputText("Hora Fim", end_buf, 256, ImGuiInputTextFlags_CallbackCharFilter, hourMask, (void *)end_buf);
                ImGui::PopItemWidth();

                if (ImGui::TreeNode("Dias da Semana")) {

                    for (auto &[dia_val, dia_name] : dias_e_nomes)
                        ImGui::CheckboxFlags(
                            dia_name.c_str(),
                            (unsigned int *) &dias_da_semana,
                            (unsigned int) dia_val
                        );

                    ImGui::TreePop();
                }

                if (ImGui::Button("Cadastrar")) {
                    disciplinas.push_back({
                        name_buf,
                        carga_horaria,
                        dias_da_semana,
                        start_buf,
                        end_buf
                    });

                    memset(name_buf, 0, 256);
                    memset(start_buf, 0, 10);
                    memset(end_buf, 0, 10);
                    carga_horaria = 0;
                    dias_da_semana = (unsigned int) DiaSemana::Nenhum;

                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Listar")) {

                if (ImGui::BeginTable("##Table", 6)) {
                    ImGui::TableSetupColumn("Nome");
                    ImGui::TableSetupColumn("Carga Horária");
                    ImGui::TableSetupColumn("Dias da Semana");
                    ImGui::TableSetupColumn("Hora Início");
                    ImGui::TableSetupColumn("Hora Fim");
                    ImGui::TableSetupColumn("##Delete");

                    ImGui::TableHeadersRow();

                    for (int idx = 0; idx < disciplinas.size(); idx++) {

                        static const char button_label_fmt_str[] = "X##%d";

                        static char button_label[16] = {0};
                        sprintf(button_label, button_label_fmt_str, idx);

                        Disciplina &disciplina = disciplinas[idx];

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text(disciplina.nome.c_str());
                        ImGui::TableNextColumn();
                        ImGui::Text("%d", disciplina.carga_horaria);
                        ImGui::TableNextColumn();
                        ImGui::Text(buildNomeFrom(disciplina.dias_da_semana).c_str());
                        ImGui::TableNextColumn();
                        ImGui::Text(disciplina.hora_inicio.c_str());
                        ImGui::TableNextColumn();
                        ImGui::Text(disciplina.hora_fim.c_str());
                        ImGui::TableNextColumn();

                        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
                        if (ImGui::Button(button_label))
                            disciplinas.erase(disciplinas.begin() + idx);

                        ImGui::PopStyleColor();
                    }

                    ImGui::EndTable();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        
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
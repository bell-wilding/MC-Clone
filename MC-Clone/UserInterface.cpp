#include "UserInterface.h"

#include <string>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

UserInterface::UserInterface(GLFWwindow* window) : window(window) {

	const char* glsl_version = "#version 130";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	io.Fonts->AddFontFromFileTTF("res/fonts/MinecraftRegular-Bmg3.ttf", 28.0f);

	display = false;
}

UserInterface::~UserInterface() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

void UserInterface::Draw() {

}

void UserInterface::DrawCrosshair() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	//Draw background begin
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, main_viewport->Size.y), ImGuiCond_Always);
	ImGui::Begin("-", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings);

	std::string crosshair = "+";

	ImVec2 pos;
	pos.x = ImGui::GetWindowSize().x * 0.5f - ImGui::CalcTextSize(crosshair.c_str()).x * 0.5f;
	pos.y = ImGui::GetWindowSize().y * 0.5f - ImGui::CalcTextSize(crosshair.c_str()).y * 0.5f;

	ImGui::SetCursorPos(pos);
	ImGui::Text(crosshair.c_str());

	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::DrawDebugInfo(glm::vec3 playerPos, glm::ivec3 playerBlockPos, glm::ivec2 playerChunkPos) {

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static float f = 0.0f;
	static int counter = 0;

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1000, 1000));
	ImGui::Begin("-", (bool*)true, window_flags);                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("Avg frame rate: %.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Text("Avg frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("Player Pos: x(%.3f), y(%.3f), z(%.3f)", playerPos.x, playerPos.y, playerPos.z);
	ImGui::Text("Player Block: x(%d), y(%d), z(%d)", playerBlockPos.x, playerBlockPos.y, playerBlockPos.z);
	ImGui::Text("Player Chunk: x(%d), z(%d)", playerChunkPos.x, playerChunkPos.y);
	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

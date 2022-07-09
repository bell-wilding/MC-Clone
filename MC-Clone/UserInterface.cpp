#include "UserInterface.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "GLFW/glfw3.h"
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

	showDebugInfo = false;
	blockMenuActive = false;
	pressedKey = false;
	selectedBlockType = BlockAtlas::Type::GRASS;
}

UserInterface::~UserInterface() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UserInterface::Update(Player& player) {

	if (blockMenuActive) {
		player.SetCanBreakBlock(false);
		player.SetCanPlaceBlock(false);
		player.ChangeActiveBlockType(DrawBlockMenu());
		if (player.GetActiveBlockType() != selectedBlockType) {
			player.GetPlayerCamera()->SetControlsActive(true);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			blockMenuActive = false;
		}
		selectedBlockType = player.GetActiveBlockType();
	}
	else {
		DrawCrosshair();
		if (showDebugInfo)
			DrawDebugInfo(player.GetPosition(), player.GetBlockCoordinates(), player.GetChunkCoordinates());
	}

	if (!pressedKey && glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		pressedKey = GLFW_KEY_F1;
		showDebugInfo = !showDebugInfo;
	}

	if (pressedKey == GLFW_KEY_F1 && glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {
		pressedKey = false;
	}

	if (!pressedKey && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		pressedKey = GLFW_KEY_TAB;
		blockMenuActive = !blockMenuActive;
		player.GetPlayerCamera()->SetControlsActive(!blockMenuActive);
		if (blockMenuActive)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (pressedKey == GLFW_KEY_TAB && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {
		pressedKey = false;
	}
}

void UserInterface::Draw() {

}

BlockAtlas::Type UserInterface::DrawBlockMenu() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(400, 200));
	ImGui::Begin("Block Menu", NULL);

	BlockAtlas blockAtlas;
	std::map<BlockAtlas::Type, std::string> nameMap = blockAtlas.GetNameMap();
	std::vector<const char*> names;

	for (auto const& block : nameMap) {
		names.push_back(block.second.c_str());
	}

	static int item_current = BlockAtlas::Type::GRASS;
	ImGui::Text("Selected Block:");
	ImGui::Combo("", &item_current, &names[0], names.size());
	ImGui::End();


	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return (BlockAtlas::Type)item_current;
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

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1000, 1000));
	ImGui::Begin("-", (bool*)true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);                          // Create a window called "Hello, world!" and append into it.

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

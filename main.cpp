#include "Includes.h"

// Global Variables
DWORD ProccessID, baseModule, LocalPlayerOffset, RifleAmmoOffset, HealthOffset, CurrentPistolAmmo, RifleReloadAnimationOffset, YPlayerCameraOffset, XPlayerCameraOffset, XPlayerPos, YPlayerPos, ZPlayerPos, ARautoshoot;


//for checkboxes
bool RifleAmmo = false;
bool CheatHealth = false;
bool NoARrecoil = false;

//all my cheated vars that i use to change vars
int CheatedHealth = 100;
int CheatedRifleAmmo = 20;
int RemoveRifleReloadAnimation = 0;
int PlayerYReset = 0.00000000;
int PlayerXReset = 0.00000000;
int PlayerPosXCenter = 1115082938;
int PlayerPosYCenter = 1098902234;
int PlayerPosZCenter = 1116172762;

// Main Function
int main() {

    //gets and handles the proccess ID allowing you to read and write memory
    ProccessID = GetProcessID(L"ac_client.exe");
    baseModule = GetModuleBaseAddress(ProccessID, L"ac_client.exe");
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProccessID);
    if (handle == NULL) {
        std::cerr << "Error: Failed to open process." << std::endl;
        return -1;
    }


    //gets the basic localplayoffset and then puts the address
    ReadProcessMemory(handle, (LPCVOID)(baseModule + 0x17E0A8), &LocalPlayerOffset, sizeof(LocalPlayerOffset), nullptr);

    // All my Offsets
    RifleAmmoOffset = LocalPlayerOffset + 0x140;
    HealthOffset = LocalPlayerOffset + 0xEC;
    CurrentPistolAmmo = LocalPlayerOffset + 0x12c;
    RifleReloadAnimationOffset = LocalPlayerOffset + 0x78;
    YPlayerCameraOffset = LocalPlayerOffset + 0x38;
    XPlayerCameraOffset = LocalPlayerOffset + 0x34;
    XPlayerPos = LocalPlayerOffset + 0x2C;
    YPlayerPos = LocalPlayerOffset + 0x30;
    ZPlayerPos = LocalPlayerOffset + 0x28;
    ARautoshoot = LocalPlayerOffset + 0x204;


    //starts the window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Testing", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 800, 800);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Checkbox for toggling the rifle ammo cheat
        ImGui::Checkbox("Cheat Rifle Ammo", &RifleAmmo);  // This creates a checkbox
        ImGui::Checkbox("Cheat Health", &CheatHealth);
        ImGui::Checkbox("No Recoil", &NoARrecoil);


        if (ImGui::Button("Camera Check"))
        {
            int currentXlook;
            int currentYlook;
            ReadProcessMemory(handle, (LPCVOID)(YPlayerCameraOffset), &currentYlook, sizeof(currentYlook), nullptr);
            ReadProcessMemory(handle, (LPCVOID)(XPlayerCameraOffset), &currentXlook, sizeof(currentXlook), nullptr);
            std::cout << "Y POS: " << currentYlook << std::endl;
            std::cout << "X POS: " << currentXlook << std::endl;

        }
       
        if (ImGui::Button("center"))
        {
            int CurrentXPos;
            int CurrentYPos;
            int CurrentZPos;
            ReadProcessMemory(handle, (LPCVOID)(XPlayerPos), &CurrentXPos, sizeof(CurrentXPos), nullptr);
            ReadProcessMemory(handle, (LPCVOID)(YPlayerPos), &CurrentYPos, sizeof(CurrentYPos), nullptr);
            ReadProcessMemory(handle, (LPCVOID)(ZPlayerPos), &CurrentZPos, sizeof(CurrentZPos), nullptr);
            std::cout << "X Pos: " << CurrentXPos << std::endl;
            std::cout << "Y Pos: " << CurrentYPos << std::endl;
            std::cout << "Z Pos: " << CurrentZPos << std::endl;
            //X Pos: 1115082938
            //Y Pos : 1098902234
            //Z Pos : 1116172762
            WriteProcessMemory(handle, (LPVOID)(XPlayerPos), &PlayerPosXCenter, sizeof(PlayerPosXCenter), nullptr);
            WriteProcessMemory(handle, (LPVOID)(YPlayerPos), &PlayerPosYCenter, sizeof(PlayerPosYCenter), nullptr);
            WriteProcessMemory(handle, (LPVOID)(ZPlayerPos), &PlayerPosZCenter, sizeof(PlayerPosZCenter), nullptr);
        }
        if (ImGui::Button("Reset Camera"))
        {
            WriteProcessMemory(handle, (LPVOID)(YPlayerCameraOffset), &PlayerYReset, sizeof(PlayerYReset), nullptr);
            WriteProcessMemory(handle, (LPVOID)(XPlayerCameraOffset), &PlayerXReset, sizeof(PlayerXReset), nullptr);
        }


        if (NoARrecoil)
        {

            //Ar moved my camera "1064264560" amount
            int shooting;
            int moves = 1064264560;
            int currentLooking;
            ReadProcessMemory(handle, (LPCVOID)(ARautoshoot), &shooting, sizeof(shooting), nullptr);
            while (shooting == 1)
            {
                ReadProcessMemory(handle, (LPCVOID)(XPlayerCameraOffset), &currentLooking, sizeof(currentLooking), nullptr);
                int NoRecoil = currentLooking - moves;
                WriteProcessMemory(handle, LPVOID(YPlayerCameraOffset), &NoRecoil, sizeof(NoRecoil), nullptr);
            }

        }
        if (CheatHealth)
        {
            int CurrentHealth;
            ReadProcessMemory(handle, (LPCVOID)(HealthOffset), &CurrentHealth, sizeof(CurrentHealth), nullptr);
            if (CurrentHealth < 100)
            {
                WriteProcessMemory(handle, (LPVOID)(HealthOffset), &CheatedHealth, sizeof(CheatedHealth), nullptr);
            }
        }

        if (RifleAmmo) 
        {
            int currentRifleAmmo;
            ReadProcessMemory(handle, (LPCVOID)(RifleAmmoOffset), &currentRifleAmmo, sizeof(currentRifleAmmo), nullptr);
            if (currentRifleAmmo < 20) {
                WriteProcessMemory(handle, (LPVOID)(RifleAmmoOffset), &CheatedRifleAmmo, sizeof(CheatedRifleAmmo), nullptr);
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

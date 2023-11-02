#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

int main() {
    // Get the system user's name
    char username[MAX_PATH];
    DWORD usernameSize = sizeof(username);
    if (GetUserNameA(username, &usernameSize)) {
        std::string path = "C:\\Users\\{user}\\AppData\\Local\\FortniteGame\\Saved\\Config\\WindowsClient";

        std::cout << R"(
            
    _______  ______  ________________
   / ____/ |/ / __ \/_  __/  _/ ____/
  / __/  |   / / / / / /  / // /     
 / /___ /   / /_/ / / / _/ // /___   
/_____//_/|_\____/ /_/ /___/\____/   
                                     
        )" << std::endl;

        std::cout << "Fortnite Resolution Changer" << std::endl;

        // Replace "{user}" with the system user's name
        size_t pos = path.find("{user}");
        if (pos != std::string::npos) {
            path.replace(pos, 6, username);
        }

        // Prompt the user to enter X and Y resolutions
        std::string xResolution, yResolution;
        std::cout << "Enter X resolution: ";
        std::cin >> xResolution;
        std::cout << "Enter Y resolution: ";
        std::cin >> yResolution;

        // Check if "GameUserSettings.ini" exists in the directory
        std::string gameUserSettingsPath = path + "\\GameUserSettings.ini";
        if (std::filesystem::exists(gameUserSettingsPath)) {
            // Read and modify the "GameUserSettings.ini" file
            std::ifstream inputFile(gameUserSettingsPath);
            std::ofstream outputFile(gameUserSettingsPath + ".tmp");
            std::regex resolutionPattern("^ResolutionSizeX=(\\d+)|^ResolutionSizeY=(\\d+)");
            std::string line;
            bool modified = false;
            while (std::getline(inputFile, line)) {
                std::smatch match;
                if (std::regex_search(line, match, resolutionPattern)) {
                    if (match.str(0).find("ResolutionSizeX=") == 0) {
                        line = "ResolutionSizeX=" + xResolution;
                    } else if (match.str(0).find("ResolutionSizeY=") == 0) {
                        line = "ResolutionSizeY=" + yResolution;
                    }
                    modified = true;
                }
                outputFile << line << std::endl;
            }
            inputFile.close();
            outputFile.close();

            // Replace the original "GameUserSettings.ini" with the modified one
            if (modified) {
                if (std::filesystem::remove(gameUserSettingsPath)) {
                    std::filesystem::rename(gameUserSettingsPath + ".tmp", gameUserSettingsPath);
                    std::cout << "Resolution settings updated successfully." << std::endl;
                } else {
                    std::cerr << "Failed to replace the 'GameUserSettings.ini' file." << std::endl;
                }
            } else {
                std::cerr << "Failed to find resolution settings in 'GameUserSettings.ini'." << std::endl;
            }
        } else {
            std::cerr << "The 'GameUserSettings.ini' file does not exist in the directory." << std::endl;
        }
    } else {
        std::cerr << "Failed to get the username." << std::endl;
    }

    system("pause"); //Wait for any input to close window.

    return 0;
}

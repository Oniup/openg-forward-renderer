#include <filesystem>

#include "glfwd_core/utility/error.h"
#include "glfwd_engine/create_info.h"
#include "glfwd_example/game.h"

std::string FindAssetDirectory()
{
    namespace fs = std::filesystem;

    std::string_view base_path(glfwd::Application::GetBasePath());
    fs::path         working_directory(base_path);

    // Iterate over parent paths to check if it contains an "assets" directory
    while (true)
    {
        // Check if target asset path exists
        fs::path potential_path = working_directory / "assets";
        if (fs::exists(potential_path) && fs::is_directory(potential_path))
            return potential_path.string();

        // Doesn't exist? Check if parent path exists
        if (!working_directory.has_parent_path() ||
            working_directory == working_directory.parent_path())
            break;

        // Move to that parent path
        working_directory = working_directory.parent_path();
    }
    GLFWD_FATAL("Failed to find asset directory starting from path {}", base_path);
}

int main()
{
    glfwd_example::Game game;
    game.Initialize(glfwd::ApplicationCreateInfo{
        .AssetDirectory = FindAssetDirectory(),
    });
    game.Run();

    return 0;
}

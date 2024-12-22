// #include <iostream>
// #include <filesystem>
//
// namespace fs = std::filesystem;
//
// int main() {
//     fs::path savePath = "save";
//
//     if (fs::exists(savePath)) {
//         std::cout << "Save directory exists." << std::endl;
//
//         try {
//             for (const auto& entry : fs::directory_iterator(savePath)) {
//                 std::cout << entry.path() << std::endl;
//             }
//         } catch (const fs::filesystem_error& e) {
//             std::cerr << "Filesystem error: " << e.what() << '\n';
//             std::cerr << "Path: " << e.path1() << '\n';
//             std::cerr << "Error code: " << e.code().message() << '\n';
//         }
//     } else {
//         std::cerr << "Save directory does not exist!" << std::endl;
//     }
//
//     return 0;
// }

#include <iostream>
#include <string>
#include <filesystem>

int main() {
    // 简洁版lambda函数
    auto extractFilename = [](const std::string& fullPath) {
        std::filesystem::path p(fullPath);
        return p.stem().string();  // stem()直接返回不带扩展名的文件名
    };
    
    std::string testPath = "/juicefs/ST_NOTEBOOK/P20Z10200N0039/workflow/prd/cromwell-executions-volcano/CytoolsWorkflow/6e0473f4-dac5-4aa2-9524-e4a34ee28a26/call-CytoolsTaskGPU2/execution/250F701876011_0_0_198.ccf5";
    
    std::string result = extractFilename(testPath);
    std::cout << "提取的文件名: " << result << std::endl;
    
    return 0;
}
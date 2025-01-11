#include <algorithm>
#include <cstdio>
#include <cstring>
#include <deque>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

class FilePool
{
public:
    explicit FilePool(size_t maxFiles = 100) : m_maxOpenFiles(maxFiles) {}

    ~FilePool()
    {
        CloseAllFiles();
    }

    void WriteToFile(const std::string& filename, const void* data, size_t dataSize)
    {
        FILE* file = GetFileHandle(filename);  // 获取文件句柄
        {
            if (fwrite(data, 1, dataSize, file) != dataSize)
            {
                throw std::runtime_error("Failed to write to file: " + filename);
            }
        }
    }

    void CloseAllFiles()
    {
        std::lock_guard<std::mutex> lock(m_mtx);  // 确保关闭文件操作线程安全
        for (auto& [filename, file] : m_filePool)
        {
            if (file)
            {
                fclose(file);
            }
        }
        m_filePool.clear();
        m_fileQueue.clear();
    }

private:
    size_t                                 m_maxOpenFiles;
    std::unordered_map<std::string, FILE*> m_filePool;
    std::deque<std::string>                m_fileQueue;
    std::mutex                             m_mtx;  // 互斥锁，确保线程安全a

    FILE* GetFileHandle(const std::string& filename)
    {
        std::lock_guard<std::mutex> lock(m_mtx);  // 加锁，确保线程安全

        // 检查文件是否已经打开
        auto it = m_filePool.find(filename);
        if (it != m_filePool.end())
        {
            return it->second;  // 返回已打开文件
        }

        // 如果文件未打开且达到限制，关闭最早的文件
        if (m_fileQueue.size() >= m_maxOpenFiles)
        {
            CloseFile(m_fileQueue.front());
        }

        // 打开新文件并加入池中
        FILE* newFile = fopen(filename.c_str(), "ab");
        if (!newFile)
        {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        m_filePool[filename] = newFile;
        m_fileQueue.push_back(filename);
        return newFile;
    }

    void CloseFile(const std::string& filename)
    {
        auto it = m_filePool.find(filename);
        if (it != m_filePool.end())
        {
            fclose(it->second);    // 关闭文件
            m_filePool.erase(it);  // 从文件池中移除
            m_fileQueue.erase(std::remove(m_fileQueue.begin(), m_fileQueue.end(), filename),
                              m_fileQueue.end());
        }
    }
};
#include "pod5_format/c_api.h"
#include "pod5_format/uuid.h"

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

/**
 * @brief 如果文件存在则删除，防止 pod5_create_file 因为同名文件存在而报错
 */
inline bool remove_file_if_exists(const std::string& filePath)
{
    try
    {
        if (std::filesystem::exists(filePath))
        {
            std::filesystem::remove(filePath);
        }
        return true;
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Error: Failed to remove file: " << e.what() << std::endl;
        return false;
    }
}

int main(int argc, char** argv)
{
    // 1. 初始化 POD5 环境
    pod5_init();

    // 2. 定义文件路径
    const std::string input_binary_file = "/ssdData/reads_test_dat/7_179_426_77604.dat";
    const std::string output_pod5_file  = "./output_signal_easy.pod5";

    // 3. 读取原始二进制信号数据 (int16_t)
    std::vector<int16_t> signal_data;
    {
        std::ifstream ifs(input_binary_file, std::ios::binary | std::ios::ate);
        if (!ifs.is_open())
        {
            std::cerr << "Error: Failed to open input file: " << input_binary_file << "\n";
            return EXIT_FAILURE;
        }

        auto file_size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        if (file_size % sizeof(int16_t) != 0)
        {
            std::cerr << "Error: Invalid file size, not a multiple of int16_t: " << file_size
                      << "\n";
            return EXIT_FAILURE;
        }
        signal_data.resize(file_size / sizeof(int16_t));
        ifs.read(reinterpret_cast<char*>(signal_data.data()), file_size);
    }

    std::cout << "Success: Read " << signal_data.size() << " samples from binary file\n";

    // 4. 开始 POD5 写入流程
    {
        // 写入前清理旧文件
        if (!remove_file_if_exists(output_pod5_file))
        {
            return EXIT_FAILURE;
        }

        // 创建 POD5 文件写入句柄
        auto file = pod5_create_file(output_pod5_file.c_str(), "signal_writer", nullptr);
        if (!file)
        {
            std::cerr << "Error: Failed to create POD5 file: " << pod5_get_error_string() << "\n";
            return EXIT_FAILURE;
        }

        // A. 注册 Pore 信息
        std::int16_t pore_type_id = -1;
        if (pod5_add_pore(&pore_type_id, file, "test_pore") != POD5_OK)
        {
            std::cerr << "Error: Failed to add pore: " << pod5_get_error_string() << "\n";
            return EXIT_FAILURE;
        }
        if (pore_type_id != 0)
        {
            std::cerr << "Get pore_type_id error\n";
            return EXIT_FAILURE;
        }

        // B. 注册 Run Info 元数据
        std::vector<char const*> context_tags_keys{"source", "test"};
        std::vector<char const*> context_tags_values{"binary_file", "compression_test"};
        std::vector<char const*> tracking_id_keys{"device", "operator"};
        std::vector<char const*> tracking_id_values{"test_device", "tester"};

        std::int16_t run_info_id = -1;
        if (pod5_add_run_info(&run_info_id, file, "binary_signal_acq", 15400,
                              4095,   // adc_max
                              -4096,  // adc_min
                              context_tags_keys.size(), context_tags_keys.data(),
                              context_tags_values.data(), "binary_signal_test", "test_flowcell",
                              "TEST001", "test_protocol", "test_run_001", 200000, "test_sample",
                              4000, "test_kit", "position_A1", "test_position", "test_software",
                              "test_system", "test_type", tracking_id_keys.size(),
                              tracking_id_keys.data(), tracking_id_values.data())
            != POD5_OK)
        {
            std::cerr << "Failed to add run_info : " << pod5_get_error_string() << "\n";
            return EXIT_FAILURE;
        }
        if (run_info_id != 0)
        {
            std::cerr << "Get run_info_id error\n";
            return EXIT_FAILURE;
        }

        // C. 生成 Read ID
        std::mt19937 gen{42};
        auto         uuid_gen = pod5::UuidRandomGenerator{gen};
        auto         read_id  = uuid_gen();

        // D. 准备 Read 级别的元数据
        std::uint32_t     read_number                = 1;
        std::uint64_t     start_sample               = 0;
        float             median_before              = 0.0f;
        std::uint16_t     channel                    = 1;
        std::uint8_t      well                       = 1;
        pod5_end_reason_t end_reason                 = POD5_END_REASON_UNKNOWN;
        uint8_t           end_reason_forced          = false;
        float             calibration_offset         = 0.0f;
        float             calibration_scale          = 1.0f;
        float             predicted_scale            = 1.0f;
        float             predicted_shift            = 0.0f;
        float             tracked_scale              = 1.0f;
        float             tracked_shift              = 0.0f;
        std::uint32_t     num_reads_since_mux_change = 0;
        float             time_since_mux_change      = 0.0f;
        std::uint64_t     num_minknow_events         = 0;

        ReadBatchRowInfoArrayV3 row_data{( read_id_t const* )read_id.data(),
                                         &read_number,
                                         &start_sample,
                                         &median_before,
                                         &channel,
                                         &well,
                                         &pore_type_id,
                                         &calibration_offset,
                                         &calibration_scale,
                                         &end_reason,
                                         &end_reason_forced,
                                         &run_info_id,
                                         &num_minknow_events,
                                         &tracked_scale,
                                         &tracked_shift,
                                         &predicted_scale,
                                         &predicted_shift,
                                         &num_reads_since_mux_change,
                                         &time_since_mux_change};

        // E. 使用 pod5_add_reads_data 写入数据（自动压缩）
        std::int16_t const* signal_arr[]  = {signal_data.data()};
        std::uint32_t       signal_size[] = {( std::uint32_t )signal_data.size()};

        auto start = std::chrono::high_resolution_clock::now();
        if (pod5_add_reads_data(file, 1, READ_BATCH_ROW_INFO_VERSION_3, &row_data, signal_arr,
                                signal_size)
            != POD5_OK)
        {
            std::cerr << "Error: Failed to write data: " << pod5_get_error_string() << "\n";
            return EXIT_FAILURE;
        }
        auto end      = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "\nWrite operation completed in " << duration << " ms\n";
        std::cout << "Original data size: " << signal_data.size() * sizeof(int16_t) << " bytes\n";

        // 释放写入句柄
        if (pod5_close_and_free_writer(file) != POD5_OK)
        {
            std::cerr << "Error: Failed to close writer: " << pod5_get_error_string() << "\n";
            return EXIT_FAILURE;
        }
    }

    // 5. 数据回读验证
    {
        std::cout << "\n--- Starting Data Verification ---\n";
        auto file = pod5_open_file(output_pod5_file.c_str());
        if (!file)
        {
            std::cerr << "Error: Failed to open POD5 for validation: " << pod5_get_error_string()
                      << "\n";
            return EXIT_FAILURE;
        }

        std::size_t read_count_out = 0;
        if (pod5_get_read_count(file, &read_count_out) != POD5_OK)
        {
            std::cerr << "Failed to query read count: " << pod5_get_error_string() << "\n";
            return EXIT_FAILURE;
        }
        std::cout << "Query read count: " << read_count_out << "\n";

        pod5::Uuid read_id_out;
        if (pod5_get_read_ids(file, 1, ( read_id_t* )read_id_out.data()) != POD5_OK)
        {
            std::cerr << "Failed to get_read_ids: " << pod5_get_error_string() << "\n";
            return EXIT_FAILURE;
        }

        std::size_t batch_count = 0;
        if (pod5_get_read_batch_count(&batch_count, file) != POD5_OK)
        {
            std::cerr << "Failed to query batch count: " << pod5_get_error_string() << "\n";
            return EXIT_FAILURE;
        }

        std::size_t read_count = 0;

        for (std::size_t batch_index = 0; batch_index < batch_count; ++batch_index)
        {
            std::cout << "Batch: " << batch_index + 1 << "/" << batch_count << "\n";

            Pod5ReadRecordBatch_t* batch = nullptr;
            if (pod5_get_read_batch(&batch, file, batch_index) != POD5_OK)
            {
                std::cerr << "Failed to get batch: " << batch_index
                          << " error: " << pod5_get_error_string() << "\n";
                return EXIT_FAILURE;
            }

            std::size_t batch_row_count = 0;
            if (pod5_get_read_batch_row_count(&batch_row_count, batch) != POD5_OK)
            {
                std::cerr << "Failed to get batch row count\n";
                return EXIT_FAILURE;
            }

            for (std::size_t row = 0; row < batch_row_count; ++row)
            {
                uint16_t           read_table_version = 0;
                ReadBatchRowInfo_t read_data;
                if (pod5_get_read_batch_row_info_data(batch, row, READ_BATCH_ROW_INFO_VERSION,
                                                      &read_data, &read_table_version)
                    != POD5_OK)
                {
                    std::cerr << "Failed to get read " << row << ": " << pod5_get_error_string()
                              << "\n";
                    return EXIT_FAILURE;
                }

                std::cout << "======================\n";
                std::cout << "Calibration offset: " << read_data.calibration_offset << "\n";
                std::cout << "Calibration scale: " << read_data.calibration_scale << "\n";
                std::cout << "Tracked scaling scale: " << read_data.tracked_scaling_scale << "\n";
                std::cout << "Tracked scaling shift: " << read_data.tracked_scaling_shift << "\n";
                std::cout << "Predicted scaling scale: " << read_data.predicted_scaling_scale
                          << "\n";
                std::cout << "Predicted scaling shift: " << read_data.predicted_scaling_shift
                          << "\n";

                read_count += 1;

                if (read_data.num_samples != signal_data.size())
                {
                    std::cerr << "Failed size check! num_samples: " << read_data.num_samples
                              << ", expected: " << signal_data.size() << "\n";
                    return EXIT_FAILURE;
                }

                // 获取解压后的完整信号
                std::size_t sample_count = 0;
                pod5_get_read_complete_sample_count(file, batch, row, &sample_count);

                std::vector<std::int16_t> samples(sample_count);
                pod5_get_read_complete_signal(file, batch, row, samples.size(), samples.data());

                // 深度对比内容
                if (samples != signal_data)
                {
                    std::cerr << "Error: Data content mismatch!\n";
                    return EXIT_FAILURE;
                }

                std::cout << "\nFirst 100 samples:\n";
                for (size_t i = 0; i < std::min(size_t(100), samples.size()); i++)
                {
                    std::cout << ( uint16_t )samples[i] << " ";
                }
                std::cout << "\n======================\n";

                // Run info
                RunInfoDictData_t* run_info = nullptr;
                if (pod5_get_run_info(batch, read_data.run_info, &run_info) != POD5_OK)
                {
                    std::cerr << "Failed to get run info " << read_data.run_info << ": "
                              << pod5_get_error_string() << "\n";
                    return EXIT_FAILURE;
                }

                pod5_free_run_info(run_info);
            }

            if (pod5_free_read_batch(batch) != POD5_OK)
            {
                std::cerr << "Failed to release batch\n";
                return EXIT_FAILURE;
            }
        }

        std::cout << "\nVerification complete: extracted " << read_count << " reads\n";
        std::cout << "All data matches original!\n";
        pod5_close_and_free_reader(file);
    }

    // 6. 终止 POD5 环境
    pod5_terminate();
    return EXIT_SUCCESS;
}
//
//  dump_table.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "dump_table.hpp"

namespace ikan {
    
#define TABLE_TRACE(...)    IK_CORE_TRACE(module_name.c_str(), __VA_ARGS__);
#define TABLE_DEBUG(...)    IK_CORE_DEBUG(module_name.c_str(), __VA_ARGS__);
#define TABLE_INFO(...)     IK_CORE_INFO(module_name.c_str(), __VA_ARGS__);
#define TABLE_WARN(...)     IK_CORE_WARN(module_name.c_str(), __VA_ARGS__);
#define TABLE_ERROR(...)    IK_CORE_ERROR(module_name.c_str(), __VA_ARGS__);
#define TABLE_CRITICAL(...) IK_CORE_CRITICAL(module_name.c_str(), __VA_ARGS__);
  
#define TABLE_LOG(x) LogTable(level, module_name, x);
  
  static void LogTable(Logger::Level level, const std::string& module_name, const std::string& data) {
    switch (level) {
      case Logger::Level::Trace:    TABLE_TRACE("{0}", data); break;
      case Logger::Level::Debug:    TABLE_DEBUG("{0}", data); break;
      case Logger::Level::Info:     TABLE_INFO("{0}", data); break;
      case Logger::Level::Warning:  TABLE_WARN("{0}", data); break;
      case Logger::Level::Error:    TABLE_ERROR("{0}", data); break;
      case Logger::Level::Critical: TABLE_CRITICAL("{0}", data); break;
      default:
        break;
    }
  }

  void Table::AddRow(const std::vector<std::string>& rows) {
    for(uint32_t col_idx = 0; col_idx < (uint32_t)rows.size(); col_idx++) {
      table_entries[col_idx].AddEntry(rows[col_idx]);
    }
  }

  void Table::AddColumn(const std::vector<std::string>& cols) {
    IK_CORE_ASSERT(num_cols_added < num_cols, "Table Overfloa (Add Functionality to resize table)");
    for(uint32_t row_idx = 0; row_idx < (uint32_t)cols.size(); row_idx++) {
      table_entries[num_cols_added].AddEntry(cols[row_idx]);
    }
    num_cols_added++;
  }

  void Table::Dump(Logger::Level level, const std::string& module_name) {
    std::vector<std::string> rows;
    for (int col_idx = 0; col_idx < num_cols; col_idx++) {
      auto& col = table_entries[col_idx];
      rows.resize(col.total_entries);
      
      for (int row_idx = 0; row_idx < col.total_entries; row_idx++) {
        std::string& val = rows[row_idx];
        const std::string& col_entry = col.col_entries[row_idx];
        val += "  ";
        val += col_entry;
        val += std::string(size_t(col.max_entry_width - (uint32_t)col_entry.size()), ' ');
      }
    }
    
    TABLE_LOG(title);
    for (int row_idx = 0; row_idx < rows.size(); row_idx++) {
      if (row_idx == 0 and header)
        TABLE_LOG(std::string(size_t(rows[row_idx].size()), '-').c_str());
      
      TABLE_LOG(rows[row_idx].c_str());
      
      if (row_idx == 0 and header)
        TABLE_LOG(std::string(size_t(rows[row_idx].size()), '-').c_str());
      
      if (row_idx == rows.size() - 1 and header)
        TABLE_LOG(std::string(size_t(rows[row_idx].size()), '-').c_str());
    }
  }
  
  Table::Table(const std::string& title, uint32_t num_col, bool header)
  : title(title), num_cols(num_col), header(header) {
    table_entries.resize(num_col);
  }
  
} // namespace ikan

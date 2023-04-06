//
//  dump_table.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "dump_table.hpp"

namespace ikan {
  
#define TABLE_LOG(...) IK_CORE_INFO(module_name.c_str(), __VA_ARGS__);
  
  void Table::AddRow(const std::vector<std::string>& row) {
    for(uint32_t row_idx = 0; row_idx < (uint32_t)row.size(); row_idx++) {
      table_entries[row_idx].AddEntry(row[row_idx]);
    }
  }
  
  void Table::Dump(const std::string& module_name) {
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
    
    for (int row_idx = 0; row_idx < rows.size(); row_idx++) {
      if (row_idx == 0) TABLE_LOG("{0}", std::string(size_t(rows[row_idx].size()), '-').c_str());
      TABLE_LOG("{0} ", rows[row_idx].c_str());
      if (row_idx == 0) TABLE_LOG("{0}", std::string(size_t(rows[row_idx].size()), '-').c_str());
    }
  }
  
  Table::Table(uint32_t num_col) : num_cols(num_col) {
    table_entries.resize(num_col);
  }
  
} // namespace ikan

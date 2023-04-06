//
//  dump_table.h
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

namespace ikan {
  
  struct Table {
    struct TableColumn {
      uint32_t max_entry_width = 0;
      uint32_t total_entries = 0;
      std::vector<std::string> col_entries;
      
      void AddEntry(const std::string& entry) {
        col_entries.push_back(entry);
        max_entry_width = std::max(max_entry_width, (uint32_t)entry.size());
        total_entries++;
      }
    };
    
    Table(uint32_t num_col);

    void AddRow(const std::vector<std::string>& row);
    void Dump(const std::string& module_name = "");
    
    std::vector<TableColumn> table_entries;
    uint32_t num_cols = 0;
  };
  
} // namespace ikan

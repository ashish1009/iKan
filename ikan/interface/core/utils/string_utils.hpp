//
//  string_utils.hpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

namespace ikan {
  
  class StringUtils {
  public:
    /// This function returns the file name from path as string
    /// - Parameter file_path: Path of file for which file Name need to be extracted:
    /// - Important: e.g. : "folder1/folder2/file_name.extention" return "file_name"
    static std::string GetNameFromFilePath(const std::string& file_path);

    MAKE_PURE_STATIC(StringUtils);
  };
  
} // namespace ikan

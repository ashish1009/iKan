//
//  string_utils.cpp
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#include "string_utils.hpp"

namespace ikan {
  
  /// This function returns the file name from path as std::string
  ///                     lastSlash lastDot
  ///                        |        |
  /// e.g. : "folder1/folder2/fileName.extention"  returns "fileName"
  /// - Parameter file_path: path of file
  std::string StringUtils::GetNameFromFilePath(const std::string& file_path) {
    auto last_slash = file_path.find_last_of("/\\");
    last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
    
    auto last_dot = file_path.find_last_of('.');
    auto count = last_dot == std::string::npos ? file_path.size() - last_slash : last_dot - last_slash;
    
    return file_path.substr(last_slash, count);
  }
  
} // namespace ikan

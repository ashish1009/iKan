//
//  directory_manager.hpp
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace ikan {
  
  /// This class stores all the required directorys
  /// - Core Asset path, Client Asset path, Project path and so on...
  class DirectoryManager {
  public:
    /// This function sets the Base Client Asset path:
    /// - Parameter client_asset_path: Asset folder path for Client:
    /// - Important: This path should be relative to .exccutable
    static void SetClientAssetPath(const std::string& client_asset_path) {
      client_asset_path_ = client_asset_path;
      IK_WARN(LogModule::DirectoryManager, "Setting Client Asset path as | {0}", client_asset_path_);
    }
    /// This function sets the Workspace path where our engine library is present
    /// - Parameter workspace_path: Asset folder path for Client:
    /// - Important: This path should be relative to .exccutable
    static void SetWorkspacePath(const std::string& workspace_path) {
      workspace_path_ = workspace_path;
      IK_WARN(LogModule::DirectoryManager, "Setting Workspace path as | {0}", workspace_path_);
    }

    /// - Important: Returned path is relative to .exce and upto the folder name of 'Assets'
    static const std::string& GetCoreAssetBasePath() { return core_asset_path_; }
    /// This function returns the Client Asset Base path
    /// - Important: Returned path is relative to .exce and upto the folder name of 'Assets'
    static const std::string& GetClientAssetBasePath() { return client_asset_path_; }
    /// This function returns the Workspace
    /// - Important: Returned path is relative to .exce and upto the folder name of 'Assets'
    static const std::string& GetWorkspaceBasePath() { return workspace_path_; }

    /// This function returns the path relative to the executable form the path relative to workspace
    /// - Parameter path: Aasset path relative to workspace folder:
    static std::string ProjectPath(const std::string& path) { return workspace_path_ + path; }
    /// This function returns the path relative to the executable form the path relative to Core asset path
    /// - Parameter asset_path: Aasset path relative to asset folder:
    static std::string CoreAsset(const std::string& asset_path) { return core_asset_path_ + asset_path; }
    /// This function returns the path relative to client Asset path
    /// - Parameter asset_path: Aasset path relative to asset folder:
    static std::string ClientAsset(const std::string& asset_path) { return client_asset_path_ + asset_path; }

    MAKE_PURE_STATIC(DirectoryManager);
    
  private:
    inline static std::string workspace_path_ = "../../../";
    inline static std::string client_asset_path_ = workspace_path_ + "ikan/core_assets/";
    inline static const std::string core_asset_path_ = workspace_path_ + "ikan/core_assets/";
    
    friend class Application;
  };
  
} // namespace ikan

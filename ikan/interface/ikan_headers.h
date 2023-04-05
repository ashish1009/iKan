//
//  ikan_headers.h
//  ikan
//
//  Created by Ashish . on 27/03/23.
//

#pragma once

// This file includes any core headers that should be included in client side.
// Client should include just one file to compile Core engine library

// To remove documentation warning in entt::entity
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wformat-security"

// C++ Files
#include <iostream>
#include <map>

// Common Files
#include <ikan_common.h>

// Core Files
#include <core/debug/logger.hpp>

#include <core/utils/asserts.h>
#include <core/utils/directory_manager.h>

#include <core/events/event.h>
#include <core/events/application_event.h>
#include <core/events/key_event.h>
#include <core/events/mouse_event.h>

#include <core/core_application.hpp>
#include <core/window.hpp>

// Renderer Files
#include <renderer/utils/renderer.hpp>

// Imgui files
#include <imgui.h>

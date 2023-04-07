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
#include <fstream>
#include <dispatch/dispatch.h>

// Imgui files
#include <imgui.h>

// glm math library
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// Common Files
#include <ikan_common.h>

// Core Files
#include <core/debug/logger.hpp>
#include <core/debug/dump_table.hpp>

#include <core/utils/asserts.h>
#include <core/utils/directory_manager.h>
#include <core/utils/buffers.hpp>
#include <core/utils/string_utils.hpp>

#include <core/events/event.h>
#include <core/events/application_event.h>
#include <core/events/key_event.h>
#include <core/events/mouse_event.h>

#include <core/core_application.hpp>
#include <core/window.hpp>

// Renderer Files
#include <renderer/utils/renderer.hpp>
#include <renderer/utils/batch_2d_renderer.hpp>

#include <renderer/graphics/pipeline.hpp>
#include <renderer/graphics/renderer_buffer.hpp>
#include <renderer/graphics/shader.hpp>
#include <renderer/graphics/texture.hpp>
#include <renderer/graphics/framebuffer.hpp>

// Editor Files
#include <editor/property_grid.hpp>

// Imgui API
#include <imgui/imgui_api.hpp>

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

#include <core/events/event.h>

// Renderer Files
#include <renderer/utils/renderer.hpp>

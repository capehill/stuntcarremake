# Windows-Specific Features

This document describes features implemented specifically for the Windows DirectX build that are not available on the Linux/SDL builds.

## Dynamic Window Resizing

### Overview
The Windows build supports fully resizable game windows with intelligent scaling of all UI elements.

### Features

#### 1. Resizable Window with Aspect Ratio Lock
- Window can be freely resized by dragging edges or corners
- Aspect ratio automatically maintained during resize:
  - **Widescreen mode**: 16:10 ratio (800:480)
  - **Standard mode**: 4:3 ratio (640:480)
- Implemented via Win32 `WM_SIZING` message handler in `StuntCarRacer.cpp`

#### 2. Dynamic Font Scaling
- All fonts scale proportionally with window size
- Two font sizes automatically adjusted:
  - Small font: Base 15pt
  - Large font: Base 25pt
- Fonts recreated in `OnResetDevice()` when window resizes
- Scaling factor calculated by `GetTextScale()` helper function

#### 3. Dynamic Cockpit Scaling
- Cockpit overlay (wheels, dashboard, panels) scales with window
- Maintains proper proportions at any resolution
- Implementation in `Car.cpp::DrawCockpit()`
- Uses separate X and Y scale factors for accurate rendering

#### 4. Intelligent UI Positioning
- All text elements maintain proper screen positions
- Dashboard elements (boost, distance, lap time) scale correctly
- Base coordinate system allows consistent positioning across resolutions
- Speedbar dynamically adjusts length and position

### Implementation Details

#### Base Resolutions
```cpp
#define BASE_WIDTH_WIDESCREEN   800
#define BASE_WIDTH_STANDARD     640
#define BASE_HEIGHT             480
```

#### Scaling Calculation
```cpp
float GetTextScale() {
    long current_width, current_height;
    GetScreenDimensions(&current_width, &current_height);
    float base_width = wideScreen ? BASE_WIDTH_WIDESCREEN : BASE_WIDTH_STANDARD;
    return current_width / base_width;
}
```

#### Screen Dimensions
- **Windows**: Retrieved from DirectX backbuffer (`D3DSURFACE_DESC`)
- **Linux**: Hardcoded to base resolution (fixed 800x480 or 640x480)

### Code Organization

**Modified Files:**
- `StuntCarRacer.cpp`: Window message handling, font scaling, text positioning
- `StuntCarRacer.h`: Base resolution constants
- `Car.cpp`: Cockpit scaling implementation
- `Car.h`: Cockpit rendering constants

**Key Functions:**
- `GetScreenDimensions()`: Returns current screen width/height
- `GetTextScale()`: Calculates scaling factor for fonts/text
- `MsgProc()`: Handles `WM_SIZING` messages to preserve aspect ratio
- `OnResetDevice()`: Recreates fonts at correct scaled sizes
- `DrawCockpit()`: Scales cockpit overlay based on resolution

### Platform Compatibility

#### Windows (DirectX)
✅ Fully functional
- Window resizing works
- All scaling features active
- Uses DirectX backbuffer dimensions

#### Linux (OpenGL/SDL)
⚠️ Fixed resolution only
- Window resizing not implemented in SDL code
- `GetScreenDimensions()` returns hardcoded values
- Scaling functions present but return 1.0x (no scaling)
- Game runs at fixed 800x480 or 640x480
- **No breaking changes** - code is compatible but inactive

### Future Linux Support

To implement dynamic resizing on Linux, the following would be needed:
1. SDL window resize event handling
2. Dynamic SDL window dimension querying
3. SDL aspect ratio constraint implementation
4. OpenGL viewport reconfiguration on resize

The scaling logic is already in place in shared code, so Linux would benefit from the same font/cockpit scaling once SDL window resizing is implemented.

## DirectX Error Handling

### Overview
Enhanced error logging for DirectX operations to aid debugging.

### Implementation
- All DirectX resource creation calls checked for failures
- Vertex buffer lock operations validated
- Texture loading verified
- Errors logged via `OutputDebugStringW()` for debugging

**Examples:**
```cpp
if (FAILED(CreatePolygonVertexBuffer(pd3dDevice))) {
    OutputDebugStringW(L"ERROR: Failed to create polygon vertex buffer\n");
    return hr;
}
```

### Platform Compatibility
- **Windows**: Active - provides debugging information
- **Linux**: N/A - code not compiled (DirectX-specific)

## Summary

These Windows-specific features enhance the Windows DirectX build without affecting Linux/SDL builds:
- ✅ Shared codebase maintained
- ✅ No breaking changes to Linux
- ✅ Proper platform guards (`#ifndef linux`)
- ✅ Code quality improvements benefit both platforms

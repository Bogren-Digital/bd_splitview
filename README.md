# SplitView

This repository hosts a UI comparer widget, packaged as a JUCE user module. It's a practical testing tool for inspecting differences between UI components in a side-by-side fashion.

## How it Works

The split view renders two components side-by-side with a draggable divider. Both components are set to use the same size and position, ensuring that any differences in rendering are easily observable. The divider can be moved to adjust the visible area of each component, and it can be set to a fixed position if needed.

## Use Cases

Here are some demos of how this tool enhances our development workflow:

### Inspect UI layouts against reference images/mockups

![UI-Layout-Comparison](https://github.com/BD-Bence/bd_splitview_assets/raw/refs/heads/main/Screen%20Recording%202025-05-05%20at%2015.08.07.webp)

### Compare subtle differences between image processing algorithms

![Image-Processing-Comparison](https://github.com/BD-Bence/bd_splitview_assets/raw/refs/heads/main/Screen%20Recording%202025-05-05%20at%2015.32.46.webp)


## How to use

The comparer is easy to implement in your JUCE projects. Basic usage follows this pattern:

```cpp

// Create your component instances with different parameters based on what you want to compare
auto component1 = std::make_unique<YourComponent>(...);
auto component2 = std::make_unique<YourComponent>(...);

// Set up the comparer
SplitView splitView;
splitView.setDisplayedComponent(component1.get(), true); // This will be shown on the left
splitView.setDisplayedComponent(component2.get(), false); // This will be shown on the right
splitView.setButtonText(SplitView::ViewMode::A, "Original"); // Optional: Set button text for the left component
splitView.setButtonText(SplitView::ViewMode::B, "Modified"); // Optional: Set button text for the right component
addAndMakeVisible(splitView);
```

For a complete example in an audio plugin context, see `examples/EditorComparer.h`.

> Note that this example is not part of the module itself as it depends on `juce_audio_processors`, ensuring the UI comparer module remains usable in non-plugin projects. It's a header-only example that can be included in your project if needed.

## Integrating with your project

### Projucer Projects

- Clone this repository to your modules directory or add it as a submodule
- In Projucer, add the module path under "Modules"
- Add the module to your project with the ID `bd_splitview`
- Include the module header in your code: `#include <bd_splitview/bd_splitview.h>`

### CMake

Add the following to your `CMakeLists.txt`:

```cmake
# Add the module path. We'll take care of the rest
add_subdirectory(path/to/bd_splitview)

# Link to your target
target_link_libraries(YourTarget
    PRIVATE
        bd_splitview
        # other dependencies
)
```

## License

This project is licensed under the GPL-3.0 License - see the `LICENSE` file for details.

**Copyright 2025 Bogren Digital/FStreet Media**

## Contributing

We welcome contributions and feature requests! Please feel free to open issues or submit pull requests.

## Contact
For any inquiries, please reach out to:

Author: Bence Kovács

Email: bence@bogrendigital.com
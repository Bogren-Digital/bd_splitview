if (NOT COMMAND juce_add_module)
    message(FATAL_ERROR "JUCE must be added to your project before bd_splitview!")
endif ()

juce_add_module("${CMAKE_CURRENT_LIST_DIR}")
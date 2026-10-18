
cmake_minimum_required(VERSION 3.15)
project(Rockverb VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 6.1.6
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(Rockverb
    COMPANY_NAME "Rockverb Audio"
    BUNDLE_ID "com.rockverb.audio"
    PLUGIN_MANUFACTURER_CODE Rkva
    PLUGIN_CODE Rk01
    FORMATS VST3
    PRODUCT_NAME "Rockverb"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
)

target_sources(Rockverb PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_compile_definitions(Rockverb PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(Rockverb PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

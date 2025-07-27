add_executable(viewer viewer.cpp)

target_include_directories(viewer PUBLIC ${CMAKE_SOURCE_DIR}/include/viewer)
target_link_libraries(viewer Qt5::Widgets lepton_if)

# Optional: Copy binary into Docker context
add_custom_command(TARGET viewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:viewer> ${CMAKE_SOURCE_DIR}/../Lepton-UI/Docker/viewer
    COMMENT "✔️  Copied viewer to Lepton-UI Docker folder"
)

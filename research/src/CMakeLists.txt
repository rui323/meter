choreonoid_add_simple_controller(NrsMassTSaveController_iwata nrs_iwata.cpp)
choreonoid_add_simple_controller(NrsMassTSaveController_plate nrs_only_plate.cpp)

# OpenCVをリンク（plain 形式を統一）
target_link_libraries(NrsMassTSaveController_iwata ${OpenCV_LIBS})
# Qt5::Widgets も plain 形式に統一
target_link_libraries(NrsMassTSaveController_iwata Qt5::Widgets)

# OpenCVをリンク（plain 形式を統一）
target_link_libraries(NrsMassTSaveController_plate ${OpenCV_LIBS})
# Qt5::Widgets も plain 形式に統一
target_link_libraries(NrsMassTSaveController_plate Qt5::Widgets)

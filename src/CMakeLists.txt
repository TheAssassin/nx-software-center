###
# Dependencies
###

include(ExternalProject)

ExternalProject_Add(AppImageUpdate-Qt
    GIT_REPOSITORY https://github.com/AppImage/AppImageUpdate.git
    GIT_TAG rewrite
    CONFIGURE_COMMAND cd <SOURCE_DIR> && git submodule update --init --recursive
              COMMAND ${CMAKE_COMMAND} -G${CMAKE_GENERATOR} -DCMAKE_INSTALL_PREFIX=/usr
                      -DBUILD_QT_UI=ON -DBUILD_FLTK_UI=OFF -DUSE_SYSTEM_CURL=1 -DBUILD_CPR_TESTS=0 <SOURCE_DIR>
    BUILD_COMMAND make libappimageupdate-qt
    INSTALL_COMMAND make install DESTDIR=<INSTALL_DIR>
    UPDATE_COMMAND ""
)
ExternalProject_Get_Property(AppImageUpdate-Qt INSTALL_DIR)
set(AppImageUpdate-Qt_INSTALL_DIR ${INSTALL_DIR})

set(libappimageupdate-qt_PATH ${AppImageUpdate-Qt_INSTALL_DIR}/usr/lib/libappimageupdate-qt.so)
add_library(libappimageupdate-qt SHARED IMPORTED)
set_target_properties(libappimageupdate-qt PROPERTIES IMPORTED_LOCATION ${libappimageupdate-qt_PATH})
include_directories(${AppImageUpdate-Qt_INSTALL_DIR}/usr/include/)
install(FILES ${libappimageupdate-qt_PATH} DESTINATION lib)
add_dependencies(libappimageupdate-qt AppImageUpdate-Qt)


###
# Core
###

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Network)

set(CORE_SRC
    entities/Application.cpp
    entities/Repository.cpp
    entities/Source.cpp
    interactors/Interactor.h
    gateways/DownloadManager.h

    gateways/AppImageHubSource.cpp
    interactors/FetchApplicationsInteractor.cpp
    gateways/AppImageInstallLinksRegExParser.cpp
    gateways/SimpleDownloadManager.cpp
    entities/Registry.cpp
    entities/Registry.h
    entities/Executor.cpp
    entities/Executor.h
    interactors/InstallAppImageInteractor.cpp
    interactors/InstallAppImageInteractor.h
    gateways/FileDownload.cpp
    gateways/FileDownload.h
    gateways/Download.h
    gateways/Download.cpp
    gateways/ContentDownload.cpp
    gateways/ContentDownload.h
    gateways/CachedDownloadManager.h
    gateways/CachedDownloadManager.cpp
    gateways/CacheSource.cpp
    gateways/CacheSource.h
    interactors/TaskMetadata.h
    entities/RecordMetadata.h
    interactors/RemoveAppImageInteractor.cpp
    interactors/RemoveAppImageInteractor.h
    entities/Upgrader.cpp
    entities/Upgrader.h
    entities/Updater.cpp
    entities/Updater.h
    entities/Cache.cpp
    entities/Cache.h
    entities/ApplicationSerializer.cpp
    entities/ApplicationSerializer.h
    )

add_library(core OBJECT ${CORE_SRC})

target_compile_options(core PUBLIC -fPIC -fexceptions)
target_include_directories(core
    PUBLIC ${Qt5Core_INCLUDE_DIRS}
    PUBLIC ${Qt5Network_INCLUDE_DIRS}
    PUBLIC ${Qt5Gui_INCLUDE_DIRS}
    PUBLIC ${Qt5Qml_INCLUDE_DIRS}
)


###
# GUI
###

set(GUI_SRC
    ui/ApplicationListModel.h
    ui/ApplicationListModel.cpp
    ui/SearchControler.cpp
    ui/TasksController.cpp
    ui/TasksController.h
    ui/TaskListModel.cpp
    ui/TaskListModel.h
    ui/InstallController.h
    ui/InstallController.cpp
    ui/RegistryController.h
    ui/RegistryController.cpp
    ui/RegistryListModel.h
    ui/RegistryListModel.cpp
    ui/UninstallController.cpp
    ui/UninstallController.h
    ui/UpdaterController.cpp
    ui/UpdaterController.h
    ui/UpgraderController.cpp
    ui/UpgraderController.h
    ui/UpgradesListModel.h
    ui/UpgradesListModel.cpp
    ui/NotificationsController.cpp
    ui/NotificationsController.h
)

## add the qml.qrc file
qt5_add_resources(qml_QRC qml/files.qrc)

add_library(gui OBJECT ${GUI_SRC} ${qml_QRC})

target_compile_options(gui PUBLIC -fPIC -fexceptions)
target_include_directories(gui
    PUBLIC ${Qt5Core_INCLUDE_DIRS}
    PUBLIC ${Qt5Gui_INCLUDE_DIRS}
    PUBLIC ${Qt5Qml_INCLUDE_DIRS}
)


######
# Main
######

add_executable(nx_software_center
    main.cpp
    $<TARGET_OBJECTS:core>
    $<TARGET_OBJECTS:gui>)

target_link_libraries(nx_software_center
    PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Network
    libappimageupdate-qt
)

install(TARGETS nx_software_center ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})


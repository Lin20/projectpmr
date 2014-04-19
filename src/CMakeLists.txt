set ( PMR_SRCS
        Engine.cpp
        Map.cpp
        MapScene.cpp
        MenuCache.cpp
        OverworldEntity.cpp
        PaletteTexture.cpp
        Player.cpp
        ResourceCache.cpp
        Scene.cpp
        Startup.cpp
        StringConverter.cpp
        Textbox.cpp
        TextboxParent.cpp
        TileMap.cpp
        Tileset.cpp
        Utils.cpp
        )

add_executable(pmr ${PMR_SRCS})
target_link_libraries(pmr ${SFML_LIBRARIES})


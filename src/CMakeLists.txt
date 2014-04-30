set ( PMR_SRCS
        Engine.cpp
        ItemStorage.cpp
        Map.cpp
        MapScene.cpp
        MenuCache.cpp
        NPC.cpp
        OverworldEntity.cpp
        PaletteTexture.cpp
        Players.cpp
        ResourceCache.cpp
        Scene.cpp
        Script.cpp
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


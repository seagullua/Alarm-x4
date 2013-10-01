TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../../cocos2dx \
    ../../../cocos2dx/include \
    ../../../cocos2dx/kazmath/include \
    ../../../cocos2dx/platform/win32 \
    ../../../cocos2dx/platform \
    ../../../cocos2dx/platform/third_party/win32/OGLES \
    ../../../cocos2dx/platform/third_party/win32/ \
    ../../../cocos2dx/platform/third_party/win32/pthread \
    ../Classes \
    ../Classes/json \
    ../../../CocosDenshion/include

CONFIG(debug, debug|release) {
    LIBS += -L../../../Debug.win32/ -llibcocos2d
    LIBS += -L../../../Debug.win32/ -llibcurl_imp
    LIBS += -L../../../Debug.win32/ -lpthreadVCE2
    LIBS += -L../../../Debug.win32/ -llibCocosDenshion
    LIBS += -L../../../Debug.win32/ -lglew32
} else {
}

QMAKE_CXXFLAGS_DEBUG += -wd4267 -wd4251 -wd4244
QMAKE_CXXFLAGS_RELEASE += -wd4267 -wd4251 -wd4244

DEFINES += WIN32 \
            _DEBUG \
            _WINDOWS \
            COCOS2D_DEBUG="1" \
            _CRT_SECURE_NO_WARNINGS \
            _SCL_SECURE_NO_WARNINGS

SOURCES += \
    ../Classes/AppDelegate.cpp \
    main.cpp \
    ../Classes/Core/LabelFactory.cpp \
    ../Classes/Core/MTileManager.cpp \
    ../Classes/Core/MTile.cpp \
    ../Classes/ContentProvider/VKRssJokeProvider.cpp \
    ../Classes/Extension/CCHttpRequest.cpp \
    ../Classes/ContentProvider/JokesHolder.cpp \
    ../Classes/ContentProvider/LTaskManager.cpp \
    ../Classes/json/value.c \
    ../Classes/json/utf.c \
    ../Classes/json/strconv.c \
    ../Classes/json/strbuffer.c \
    ../Classes/json/pack_unpack.c \
    ../Classes/json/memory.c \
    ../Classes/json/load.c \
    ../Classes/json/hashtable.c \
    ../Classes/json/error.c \
    ../Classes/json/dump.c \
    ../Classes/ContentProvider/BingWallpaperProvider.cpp \
    ../Classes/ContentProvider/WallpaperHolder.cpp \
    ../Classes/Core/MTileGrid.cpp \
    ../Classes/Widgets/JokesWidget.cpp \
    ../Classes/Widgets/TimeWidget.cpp \
    ../Classes/Widgets/TurnOnAlarmWidget.cpp \
    ../Classes/Widgets/TurnOnAlarmMenuWidget.cpp \
    ../Classes/Core/MTileManager_Api.cpp \
    ../Classes/Core/MTileManager_MTileLayer.cpp \
    ../Classes/Core/MTileManager_WApi.cpp \
    ../Classes/Scripts/MainScript.cpp \
    ../Classes/Widgets/TimeSelector.cpp \
    ../Classes/Widgets/DialogWidget.cpp \
    ../Classes/Tiles/ClockTile.cpp \
    ../Classes/Widgets/AlarmTimeChooser.cpp \
    ../Classes/Tiles/SliderSelectorTile.cpp \
    ../Classes/Logic/Time.cpp \
    ../Classes/Logic/IntentParams.cpp \
    ../Classes/ScriptScene.cpp \
    ../Classes/Scripts/WakeUpScript.cpp \
    ../Classes/Logic/AlarmServer.cpp \
    ../Classes/Logic/DelayedActions.cpp \
    ../Classes/Logic/System.cpp \
    StorageEmulator.cpp \
    WinEmulator.cpp \
    ../Classes/Logic/Alarm.cpp \
    ../Classes/Extension/Platform.cpp \
    ../Classes/Widgets/Flow/FlowWidget.cpp \
    ../Classes/Widgets/Flow/FlowTable.cpp \
    ../Classes/Widgets/Flow/FlowRenderer.cpp \
    ../Classes/Widgets/Flow/FlowRendererNode.cpp \
    ../Classes/Core/GraphicsManager.cpp \
    ../Classes/Widgets/Flow/FlowGame.cpp \
    ../Classes/Widgets/Flow/FlowPointState.cpp \
    ../Classes/Widgets/Flow/Generator/FGenerator.cpp \
    ../Classes/Widgets/Flow/Generator/TwoShuffle.cpp \
    ../Classes/Widgets/Flow/Generator/ThreeBlockShuffle.cpp \
    ../Classes/Widgets/AlarmWidget.cpp \
    ../Classes/Tiles/CurrentTimeTile.cpp \
    ../Classes/Widgets/FlowHelperWidget.cpp \
    ../Classes/Tiles/FlowTile.cpp \
    ../Classes/Widgets/StartFlowGameWidget.cpp \
    ../Classes/ContentProvider/Weather/City.cpp \
    ../Classes/ContentProvider/Weather/Forecast.cpp \
    ../Classes/ContentProvider/Weather/WeatherCoUaProvider.cpp \
    ../Classes/ContentProvider/Weather/WeatherHolder.cpp \
    ../Classes/Widgets/WeatherWidget.cpp \
    ../Classes/Logic/PrefetchAction.cpp \
    ../Classes/Logic/MusicBox.cpp \
    ../Classes/Logic/TimeGuess.cpp \
    ../Classes/Logic/lang_en.cpp \
    ../Classes/Logic/lang_ru.cpp \
    ../Classes/Logic/lang_uk.cpp \
    ../Classes/Logic/Lang.cpp

HEADERS += \
    ../Classes/AppMacros.h \
    ../Classes/AppDelegate.h \
    main.h \
    ../Classes/Core/LabelFactory.h \
    ../Classes/Core/MTilePosition.h \
    ../Classes/Core/MTile.h \
    ../Classes/Core/MTileSize.h \
    ../Classes/Core/MTileManager.h \
    ../Classes/ContentProvider/Joke.h \
    ../Classes/ContentProvider/JokeProvider.h \
    ../Classes/ContentProvider/VKRssJokeProvider.h \
    ../Classes/Extension/CCHttpRequest.h \
    ../Classes/ContentProvider/JokesHolder.h \
    ../Classes/Core/Binary.h \
    ../Classes/ContentProvider/LTask.h \
    ../Classes/ContentProvider/LTaskManager.h \
    ../Classes/json/utf.h \
    ../Classes/json/strbuffer.h \
    ../Classes/json/jansson_private.h \
    ../Classes/json/jansson_config.h \
    ../Classes/json/jansson.h \
    ../Classes/json/hashtable.h \
    ../Classes/ContentProvider/Wallpaper.h \
    ../Classes/ContentProvider/WallpaperProvider.h \
    ../Classes/ContentProvider/BingWallpaperProvider.h \
    ../Classes/Core/file_exists.h \
    ../Classes/ContentProvider/WallpaperHolder.h \
    ../Classes/Core/Widget.h \
    ../Classes/Core/MTileGrid.h \
    ../Classes/Widgets/JokesWidget.h \
    ../Classes/Widgets/TimeWidget.h \
    ../Classes/Widgets/TurnOnAlarmWidget.h \
    ../Classes/Widgets/TurnOnAlarmMenuWidget.h \
    ../Classes/Core/impl/MTileManager_CreateTileWithDelay.h \
    ../Classes/Core/impl/MTileManager_AnimationFinalizer.h \
    ../Classes/Core/impl/MTileManager_TileDecorationChanger.h \
    ../Classes/Core/impl/MTileManager_MTileLayer.h \
    ../Classes/Core/Script.h \
    ../Classes/Scripts/MainScript.h \
    ../Classes/Core/impl/InverseAction.h \
    ../Classes/Widgets/TimeSelector.h \
    ../Classes/Core/SpaceRequest.h \
    ../Classes/Widgets/DialogWidget.h \
    ../Classes/Tiles/ClockTile.h \
    ../Classes/Widgets/AlarmTimeChooser.h \
    ../Classes/Logic/Time.h \
    ../Classes/Tiles/SliderSelectorTile.h \
    ../Classes/Core/Anchor.h \
    ../Classes/Core/Alignment.h \
    ../Classes/Logic/IntentParams.h \
    ../Classes/ScriptScene.h \
    ../Classes/Scripts/WakeUpScript.h \
    ../Classes/Logic/AlarmServer.h \
    ../Classes/Logic/ValueStorage.h \
    ../Classes/Logic/DelayedActions.h \
    ../Classes/Logic/System.h \
    StorageEmulator.h \
    ../Classes/Logic/Alarm.h \
    ../Classes/Extension/Platform.h \
    ../Classes/Widgets/Flow/FlowWidget.h \
    ../Classes/Widgets/Flow/FlowTable.h \
    ../Classes/Widgets/Flow/FlowPoint.h \
    ../Classes/Widgets/Flow/FlowPointPair.h \
    ../Classes/Widgets/Flow/FlowPointState.h \
    ../Classes/Widgets/Flow/FlowColor.h \
    ../Classes/Widgets/Flow/FlowRenderer.h \
    ../Classes/Widgets/Flow/FlowRendererNode.h \
    ../Classes/Core/GraphicsManager.h \
    ../Classes/Widgets/Flow/FlowGame.h \
    ../Classes/Widgets/Flow/Generator/FGenerator.h \
    ../Classes/Widgets/AlarmWidget.h \
    ../Classes/Tiles/CurrentTimeTile.h \
    ../Classes/Widgets/FlowHelperWidget.h \
    ../Classes/Tiles/FlowTile.h \
    ../Classes/Widgets/StartFlowGameWidget.h \
    ../Classes/ContentProvider/Weather/City.h \
    ../Classes/ContentProvider/Weather/Forecast.h \
    ../Classes/ContentProvider/Weather/WeatherProvider.h \
    ../Classes/ContentProvider/Weather/WeatherCoUaProvider.h \
    ../Classes/ContentProvider/Weather/WeatherHolder.h \
    ../Classes/Widgets/WeatherWidget.h \
    ../Classes/Logic/Location.h \
    ../Classes/Logic/PrefetchAction.h \
    ../Classes/Logic/MusicBox.h \
    ../Classes/Logic/TimeGuess.h \
    ../Classes/Logic/Translations.h \
    ../Classes/Logic/Lang.h

OTHER_FILES +=


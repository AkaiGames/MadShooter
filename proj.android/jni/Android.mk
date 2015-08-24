LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LUA_FILES  := $(wildcard $(LOCAL_PATH)/../../Classes/lua/*.c)
LUA_FILES  := $(LUA_FILES:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := $(LUA_FILES)
LOCAL_SRC_FILES += ../../$(FILE_LIST:$(LOCAL_PATH)/../../Classes/lua/%=%) \
				   hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/lua/ \
                   ../../Classes/Game.cpp \
                   ../../Classes/Splash.cpp \
                   ../../Classes/MenuScene.cpp \
                   ../../Classes/NewGameSinglePlayerScene.cpp \
                   ../../Classes/HUDLayer.cpp \
                   ../../Classes/MobileControlLayer.cpp \
                   ../../Classes/Ship.cpp \
                   ../../Classes/Player.cpp \
                   ../../Classes/Bullet.cpp \
                   ../../Classes/Bot.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

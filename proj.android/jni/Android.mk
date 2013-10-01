LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := hellocpp_shared
LOCAL_MODULE_FILENAME := libhellocpp
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/Core/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Extension/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/ContentProvider/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/ContentProvider/Weather/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Widgets/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Widgets/Flow/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Scripts/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Logic/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Tiles/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/json/*.c)

LOCAL_SRC_FILES := hellocpp/main.cpp

LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../../../cocos2dx/platform/third_party/android/prebuilt/libcurl/include \
$(LOCAL_PATH)/../../Classes/json


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_curl_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)

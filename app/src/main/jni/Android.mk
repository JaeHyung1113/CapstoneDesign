LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS) 				# LOCAL_XXX로 되어 있는 변수들을 자동으로 삭제하는 변수, 모듈을 설명하기 전에 선언해야 함

#opencv library
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include C:\Users\hwc11\AndroidStudioProjects\CapstoneDesign\opencv\native\jni\OpenCV.mk	# OPENCV PATH Setting

LOCAL_MODULE := test      	 # 빌드하려는 모듈의 이름 지정 ex) System.loadLibrary
LOCAL_SRC_FILES := test.cpp         # 모듈에 빌드할 C, C++ 소스파일
LOCAL_LDLIBS := -llog                # 공유 라이브러리나 실행 파일을 빌드할 때 사용할 추카 링커 목록

#LOCAL_MODULE:= loadCascade
#FILES := load_cascade.cpp
#LOCAL_SRC_FILES := $(FILES)
#LOCAL_LDLIBS := -llog

LOCAL_MODULE := detect_skin
LOCAL_SRC_FILES := skin.cpp
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)      # LOCAL_XXX 변수에서 제공한 모듈의 모든 정보를 수집하는 빌드 스크립트를 가리키고
                                     # 나열한 소스에서 타겟 공유 라이브러리의 빌드 방법을 결정
APP_OPTIM := debug
APP_ABI := arm64-v8a armeabi-v7a x86 x86_64	# ABI용 코드 생성
APP_PLATFORM := android-21					# 애플리케이션이 빌드된 Android API 수준

APP_STL := c++_static						# 애플리케이션에 사용할 C++ 표준 라이브러리
APP_CPPFLAGS := -frtti -fexceptions			# 모든 C++ 컴파일을 위해 전달되는 플래그
NDK_TOOLCHAIN_VERSION := clang

APP_BUILD_SCRIPT := C:\Users\hwc11\AndroidStudioProjects\CapstoneDesign\app\src\main\jni\Android.mk		# Android.mk 파일의 절대 경로로 설정
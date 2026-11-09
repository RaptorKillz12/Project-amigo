#include "amigo/runtime.hpp"

#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_projectamigo_runtime_AmigoNative_nativeVersion(JNIEnv* env, jclass) {
    return env->NewStringUTF(amigo::Runtime::version());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_projectamigo_runtime_AmigoNative_nativeInspectData(
    JNIEnv* env, jclass, jstring directory) {
    if (directory == nullptr) {
        return env->NewStringUTF("ERROR: No data directory was provided.");
    }

    const char* chars = env->GetStringUTFChars(directory, nullptr);
    if (chars == nullptr) {
        return env->NewStringUTF("ERROR: Unable to read the data directory path.");
    }

    const std::string path(chars);
    env->ReleaseStringUTFChars(directory, chars);

    const auto report = amigo::Runtime::inspect_data_directory(path);
    if (report.status != amigo::RuntimeStatus::ok) {
        return env->NewStringUTF(report.message.c_str());
    }

    const auto summary = "OK: " + std::to_string(report.files) +
        " files, " + std::to_string(report.directories) +
        " directories, " + std::to_string(report.total_bytes) + " bytes";
    return env->NewStringUTF(summary.c_str());
}

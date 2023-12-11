# AnkiLink

 PC and Android Cross-Platform Anki Cards

# Build

## Windows

Building with make, using MinGW as an example：  

1. Install MinGW
2. Install CMake
3. cd exe
4. mkdir build
5. cd build
6. cmake -G "MinGW Makefiles" ..
7. mingw32-make"

## Android

Use Android Studio import and build apk.

# Usage

Currently, only file transfer is implemented.

1. **File Preparation:** Prepare the Markdown file.
2. **Port Forwarding:** Connect the device and enter the command: `adb forward tcp:50000 tcp:50000`.
3. **Application Setup:** On the Android side, build the APK and install it on the phone. On the Windows side, build the EXE file.
4. **Initiate Transfer:** Click the middle button in the Android APK to start the server. On Windows, open PowerShell and enter the command `.\AnkiLink.exe client 127.0.0.1 50000` to choose and initiate file transfer.
5. **File Verification:** Use `adb shell` on the Android device to confirm the status of file transfer in the private directory.

# About

I have a strong passion for programming. I initially learned C++, but I've never had the opportunity to work on a C++ project, which has always been a source of regret for me. Due to professional requirements, I need to acquire proficiency in C++. Therefore, I would like to create a functional learning project.

Given my age and the multitude of subjects I wish to study, I have divided my learning journey into three parts:

1. **Understanding:** I plan to employ the Feynman learning technique and document the knowledge I gain by writing blog posts (to be implemented in the future).
2. **Memorization:** Computer programming can be likened to a language with its own syntax, and it's as mathematical as it is linguistic. To aid in retention, I intend to structure it in a format similar to "English words" and use Anki flashcards for effective memorization.
3. **Practice:** I will work on practical projects to gain hands-on engineering experience.

This project aims to take the blog content, broken down into smaller knowledge points, and transform it into Anki flashcards that can be displayed on a mobile device. While I am aware that the AnkiDroid project already offers this functionality, this project is primarily an exercise in learning and aims to create a usable tool from scratch.

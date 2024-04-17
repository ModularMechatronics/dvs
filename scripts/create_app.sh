
mkdir -p AppOutput.app/Contents/MacOS
mkdir -p AppOutput.app/Contents/Resources
mkdir -p AppOutput.app/Contents/Resources/shaders
mkdir -p AppOutput.app/Contents/Resources/fonts
mkdir -p AppOutput.app/Contents/Resources/images

touch AppOutput.app/Contents/Info.plist

/usr/libexec/PlistBuddy -c "Set :CFBundleIconFile string ./xcode_working_project/build/main_application/Debug/dvs.app/Contents/Resources/AppIcon.icns" AppOutput.app/Contents/Info.plist

./xcode_working_project/build/main_application/Debug/dvs.app/Contents/Resources/AppIcon.icns

